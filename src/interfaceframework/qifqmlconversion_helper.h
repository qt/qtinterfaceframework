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

#ifndef QIFQMLCONVERSION_HELPER_H
#define QIFQMLCONVERSION_HELPER_H

#include <QtInterfaceFramework/qtifglobal.h>

#include <QtCore/QMetaEnum>
#include <QtCore/QVariant>
#include <QtCore/QVector>
#include <QtCore/QtDebug>

QT_BEGIN_NAMESPACE

Q_QTINTERFACEFRAMEWORK_EXPORT void qtif_qmlOrCppWarning(const QObject *obj, const char *errorString);
Q_QTINTERFACEFRAMEWORK_EXPORT void qtif_qmlOrCppWarning(const QObject *obj, const QString& errorString);

Q_QTINTERFACEFRAMEWORK_EXPORT QVariant qtif_convertFromJSON(const QVariant &val);

template <typename T>  QVariant qtif_convertValue(const T &val)
{
    QVariant var;
    int userType = qMetaTypeId<T>();
    QMetaType metaType(userType);
    const QMetaObject *mo = metaType.metaObject();
    QString enumName = QString::fromLocal8Bit(metaType.name()).split(QStringLiteral("::")).last();
    if (mo) {
        QMetaEnum mEnum = mo->enumerator(mo->indexOfEnumerator(enumName.toLocal8Bit().constData()));
        if (mEnum.isValid())
            var = QVariant::fromValue<T>(val).toInt();
    }

    if (!var.isValid())
        var = QVariant::fromValue<T>(val);

    return var;
}

template <typename T> QVariantList qtif_convertAvailableValues(const QVector<T> &aValues)
{
    QVariantList list;
    list.reserve(aValues.size());
    for (const T &val : aValues) {
        //As QML doesn't support Enums in Lists we need to convert it to int
        list.append(qtif_convertValue<T>(val));
    }
    return list;
}

template <class T> const T *qtif_gadgetFromVariant(const QObject *obj, const QVariant &var)
{
    if (Q_UNLIKELY(!var.isValid())) {
        qtif_qmlOrCppWarning(obj, "The passed QVariant is undefined");
        return nullptr;
    }

    const void *data = var.constData();

    QMetaType type(var.userType());
    if (Q_UNLIKELY(!type.flags().testFlag(QMetaType::IsGadget))) {
        qtif_qmlOrCppWarning(obj, "The passed QVariant needs to use the Q_GADGET macro");
        return nullptr;
    }

    const QMetaObject *mo = type.metaObject();
    while (mo) {
        if (mo->className() == T::staticMetaObject.className())
            return reinterpret_cast<const T*>(data);
        mo = mo->superClass();
    }

    qtif_qmlOrCppWarning(obj, QLatin1String("The passed QVariant is not derived from ") + QLatin1String(T::staticMetaObject.className()));

    return nullptr;
}


QT_END_NAMESPACE

#endif // QIFQMLCONVERSION_HELPER_H
