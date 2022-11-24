/****************************************************************************
**
** Copyright (C) 2021 The Qt Company Ltd.
** Copyright (C) 2019 Luxoft Sweden AB
** Copyright (C) 2018 Pelagicore AG
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtInterfaceFramework module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or (at your option) the GNU General
** Public license version 3 or any later version approved by the KDE Free
** Qt Foundation. The licenses are as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-2.0.html and
** https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <qifqmlconversion_helper.h>

#include <QtQml>
#include <private/qv4engine_p.h>
#include <private/qv4errorobject_p.h>
#include <private/qv4scopedvalue_p.h>

QT_BEGIN_NAMESPACE

namespace qtif_helper {
    static const QString valueLiteral = QStringLiteral("value");
    static const QString typeLiteral = QStringLiteral("type");
}

using namespace qtif_helper;

void qtif_qmlOrCppWarning(const QObject *obj, const char *errorString)
{
    qtif_qmlOrCppWarning(obj, QLatin1String(errorString));
}

void qtif_qmlOrCppWarning(const QObject *obj, const QString &errorString)
{
    //If the object is not part of a javascript engine, print a normal warning
    QJSEngine *jsEngine = qjsEngine(obj);
    if (Q_UNLIKELY(!jsEngine)) {
        qWarning("%s", qPrintable(errorString));
        return;
    }

    //Try to get more information about the current line of execution
    QV4::ExecutionEngine *v4 = jsEngine->handle();
    QV4::Scope scope(v4);
    QV4::Scoped<QV4::ErrorObject> error(scope);
    QV4::StackTrace trace = v4->stackTrace(1);
    if (!!error)
        trace = *error->d()->stackTrace;

    //If we don't have any information, let qmlWarning use its magic and find something
    if (trace.isEmpty()) {
        qmlWarning(obj) << errorString;
        return;
    }

    v4->throwError(errorString);
}

/*!
    \relates QIfSimulationEngine

    Converts \a value from JSON to valid C++ types.

    The provided JSON value needs to follow the \l{IfSimulatorDataFormat}{IfSimulator Data
    Format}.
*/
QVariant qtif_convertFromJSON(const QVariant &value)
{
    QVariant val = value;
    // First try to convert the values to a Map or a List
    // This is needed as it could also store a QStringList or a Hash
    if (val.canConvert(QMetaType::fromType<QVariantMap>()))
        val.convert(QMetaType::fromType<QVariantMap>());
    if (val.metaType() != QMetaType::fromType<QString>() && val.canConvert(QMetaType::fromType<QVariantList>()))
        val.convert(QMetaType::fromType<QVariantList>());

    if (val.metaType() == QMetaType::fromType<QVariantMap>()) {
        const QVariantMap map = val.toMap();
        if (map.contains(typeLiteral) && map.contains(valueLiteral)) {
            const QString type = map.value(typeLiteral).toString();
            const QVariant value = map.value(valueLiteral);

            if (type == QStringLiteral("enum")) {
                QString enumValue = value.toString();
                const int lastIndex = enumValue.lastIndexOf(QStringLiteral("::"));
                const QString className = enumValue.left(lastIndex) + QStringLiteral("*");
                enumValue = enumValue.right(enumValue.count() - lastIndex - 2);
                QMetaType metaType = QMetaType::fromName(className.toLatin1());
                const QMetaObject *mo = metaType.metaObject();
                if (Q_UNLIKELY(!mo)) {
                    qWarning() << "Couldn't retrieve MetaObject for enum parsing:" << map;
                    qWarning("Please make sure %s is registered in Qt's meta-type system: qRegisterMetaType<%s>()",
                             qPrintable(className), qPrintable(className));
                    return QVariant();
                }

                for (int i = mo->enumeratorOffset(); i < mo->enumeratorCount(); ++i) {
                    QMetaEnum me = mo->enumerator(i);
                    bool ok = false;
                    int value = me.keysToValue(enumValue.toLatin1(), &ok);
                    if (ok) {
                        return QVariant(QMetaType::fromName((QLatin1String(me.scope()) + QStringLiteral("::") + QLatin1String(me.enumName())).toLatin1()), &value);
                    }
                }
                qWarning() << "Couldn't parse the enum definition" << map;
                return QVariant();
            } else {
                QMetaType metaType = QMetaType::fromName(type.toLatin1());
                const QMetaObject *mo = metaType.metaObject();
                if (Q_UNLIKELY(!mo)) {
                    qWarning() << "Couldn't retrieve MetaObject for struct parsing:" << map;
                    qWarning("Please make sure %s is registered in Qt's meta-type system: qRegisterMetaType<%s>()",
                             qPrintable(type), qPrintable(type));
                    return QVariant();
                }

                void *gadget = metaType.create();
                if (!Q_UNLIKELY(gadget)) {
                    qWarning("Couldn't create a new instance of %s", metaType.name());
                    return QVariant();
                }

                /*  Left here for debugging
                    for (int i = mo->methodOffset(); i < mo->methodCount(); ++i)
                        qDebug() << mo->method(i).methodSignature();
                */

                int moIdx = mo->indexOfMethod("fromJSON(QVariant)");
                if (Q_UNLIKELY(moIdx == -1)) {
                    qWarning("Couldn't find method: %s::fromJSON(QVariant)\n"
                             "If your are using code created by the ifcodegen, please regenerate"
                             "your frontend code. See AUTOSUITE-1374 for why this is needed",
                             metaType.name());
                    return QVariant();
                }

                mo->method(moIdx).invokeOnGadget(gadget, Q_ARG(QVariant, QVariant(value)));
                return QVariant(metaType, gadget);
            }
        }

        QVariantMap convertedValues;
        for (auto i = map.constBegin(); i != map.constEnd(); ++i)
            convertedValues.insert(i.key(), qtif_convertFromJSON(i.value()));
        return convertedValues;
    } else if (val.metaType() == QMetaType::fromType<QVariantList>()) {
        QVariantList values = val.toList();
        for (auto i = values.begin(); i != values.end(); ++i)
            *i = qtif_convertFromJSON(*i);
        return values;
    }

    return val;
}

QT_END_NAMESPACE
