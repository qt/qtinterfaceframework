// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qifsimulationproxy.h"
#include "qifsimulationengine.h"

#include <QDebug>
#include <QQmlInfo>

#include <private/qmetaobjectbuilder_p.h>

using namespace Qt::StringLiterals;

QT_BEGIN_NAMESPACE

Q_LOGGING_CATEGORY(qLcIfSimulationEngine, "qt.if.simulationengine")
Q_LOGGING_CATEGORY(qLcIfRecGuard, "qt.if.simulationengine.recursionguard")

namespace qtif_private {

QIfSimulationProxyBase::QIfSimulationProxyBase(QMetaObject *staticMetaObject, QObject *instance, const QHash<int, int> &methodMap, QObject *parent)
    : QObject(parent)
    , m_noSimulationEngine(false)
    , m_instance(instance)
    , m_staticMetaObject(staticMetaObject)
    , m_methodMap(methodMap)
{
}

const QMetaObject *QIfSimulationProxyBase::metaObject() const
{
    // Copied from moc_ class code
    // A dynamicMetaObject is created when the type is used from QML and new functions/properties
    // are added. This makes sure that we can access these from C++ as well
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : m_staticMetaObject;
}

void *QIfSimulationProxyBase::qt_metacast(const char *classname)
{
    if (!classname)
        return nullptr;
    return m_instance->qt_metacast(classname);
}

int QIfSimulationProxyBase::qt_metacall(QMetaObject::Call call, int methodId, void **a)
{
    if (m_noSimulationEngine)
        return -1;

    // Special handling for the artificial "Base" property.
    // All other properties are forwarded to the qt_metacall generated by moc which translates the
    // absolute id's back to relative ones
    if (call == QMetaObject::ReadProperty || call == QMetaObject::WriteProperty) {
        int basePropertyIndex = m_staticMetaObject->indexOfProperty("Base");
        if (methodId == basePropertyIndex) {
            void *_v = a[0];
            *reinterpret_cast< QObject**>(_v) = m_instance;
            return -1;
        }
    }

    if (call == QMetaObject::InvokeMetaMethod) {
        // The qt_metacall is only used for the dynamic calling of methods and signatures
        // e.g. a signal or method called from QML
        // But it is also called by QObject::activate when a signal is emitted. In case the signal
        // is connected, the slot is called directly and qt_metacall is NOT called.
        // QObject::activate will call qt_metacall for all signatures for the signal which are not
        // connected.
        // If qt_metacall gets called and the call is from our proxied instance, the signal name
        // matches and the signal arguments match as well (either completely or just partially)
        // The call got forwarded from the QObject::activate of the proxied instance. In this
        // case we don't forward the call again to the proxied instance, but handle it ourself.
        auto tryHandleCall = [=]()
        {
            if (sender() != m_instance)
                return false;

            QMetaMethod senderSignal = sender()->metaObject()->method(senderSignalIndex());
            QMetaMethod receiverMethod = m_staticMetaObject->method(methodId);

            if (senderSignal.name() != receiverMethod.name())
                return false;

            if (senderSignal.parameterCount() < receiverMethod.parameterCount())
                return false;

            for (int pI = 0; pI < receiverMethod.parameterCount(); pI++) {
                if (senderSignal.parameterMetaType(pI) != receiverMethod.parameterMetaType(pI))
                    return false;
            }

//            qDebug() << "HANDLE CALL" << methodId << m_staticMetaObject->method(methodId).name() << sender() << m_instance;
            // The static MetaObject uses local ids, so we need to subtract the offset
            QMetaObject::activate(this, m_staticMetaObject, methodId - m_staticMetaObject->methodOffset(), a);
            return true;
        };

        if (tryHandleCall()) {
            return 0;
        } else {
            // Forward the signal to the proxied instance.

            // As we don't derive from the MetaObject of m_instance, we need to use the methodMap to
            // translate our methodId to the methodId for m_instance
//            qDebug() << "FORWARD CALL" << methodId << m_staticMetaObject->method(methodId).name() << sender() << m_instance;
            int ret = m_instance->qt_metacall(call, m_methodMap.key(methodId), a);
            return ret;
        }
    }
    return m_instance->qt_metacall(call, methodId, a);
}

void QIfSimulationProxyBase::classBegin()
{
}

void QIfSimulationProxyBase::componentComplete()
{
    setProperty("Base", QVariant::fromValue(m_instance));
}

QMetaObject QIfSimulationProxyBase::buildObject(const QMetaObject *metaObject, QHash<int, int> &methodMap, QIfSimulationProxyBase::StaticMetacallFunction metaCallFunction)
{
    QMetaObjectBuilder builder;
    const QString name = QString(u"QIfSimulationProxy_%1"_s).arg(QLatin1String(metaObject->className()));
    builder.setClassName(qPrintable(name));
    builder.setSuperClass(&QObject::staticMetaObject);
    builder.setStaticMetacallFunction(metaCallFunction);

    // Build the MetaObject ourself
    // This is needed as QML uses the static_metacall for reading the properties and every QMetaObject
    // has its own set. But as we need to intercept this to forward it to the registered instance, we
    // build our MetaObject completely and have one static_metacall function for all properties
    const QMetaObject *mo = metaObject;

    //Search for the QObject base class to know which offset we need to start building from
    const QMetaObject *superClass = mo->superClass();
    while (qstrcmp(superClass->className(), "QObject") != 0) {
        superClass = superClass->superClass();
    }
    const int methodOffset = superClass->methodCount();
    const int propertyOffset = superClass->propertyCount();

    //Fill the mapping for all QObject methods.
    for (int i=0; i<methodOffset; ++i)
        methodMap.insert(i, i);

    //Add all signals
    qCDebug(qLcIfSimulationEngine) << "Signal Mapping: Original -> Proxy";
    for (int index = methodOffset; index < mo->methodCount(); ++index) {
        QMetaMethod mm = mo->method(index);
        if (mm.methodType() == QMetaMethod::Signal) {
            auto mb = builder.addMethod(mm);
            qCDebug(qLcIfSimulationEngine) << index << "->" << methodOffset + mb.index();
            methodMap.insert(index, methodOffset + mb.index());
        }
    }

    //Add all other methods
    qCDebug(qLcIfSimulationEngine) << "Method Mapping: Original -> Proxy";
    for (int index = methodOffset; index < mo->methodCount(); ++index) {
        QMetaMethod mm = mo->method(index);
        if (mm.methodType() != QMetaMethod::Signal) {
            auto mb = builder.addMethod(mm);
            qCDebug(qLcIfSimulationEngine) << index << "->" << methodOffset + mb.index();
            methodMap.insert(index, methodOffset + mb.index());
        }
    }

    //Add all properties
    for (int index = propertyOffset; index < mo->propertyCount(); ++index) {
        QMetaProperty prop = mo->property(index);
        builder.addProperty(prop);
    }
    //Add a Base property which works like a attached property
    builder.addProperty("Base", "QObject *");

    //Debugging output
    if (qLcIfSimulationEngine().isDebugEnabled()) {
        qCDebug(qLcIfSimulationEngine) << "Original Object:";
        for (int i=0; i < mo->methodCount(); i++) {
            QMetaMethod method = mo->method(i);
            qCDebug(qLcIfSimulationEngine) << "method: " << method.methodIndex() << method.methodSignature();
        }
        for (int i=0; i < mo->propertyCount(); i++) {
            QMetaProperty prop = mo->property(i);
            qCDebug(qLcIfSimulationEngine) << "property:" << prop.propertyIndex() << prop.name();
            QMetaMethod method = prop.notifySignal();
            qCDebug(qLcIfSimulationEngine) << "signal: " << method.methodIndex() << method.methodSignature();
        }

        qCDebug(qLcIfSimulationEngine) << "Proxy Object:";
        mo = builder.toMetaObject();
        for (int i=0; i < mo->methodCount(); i++) {
            QMetaMethod method = mo->method(i);
            qCDebug(qLcIfSimulationEngine) << "method: " << method.methodIndex() << method.methodSignature();
        }
        for (int i=0; i < mo->propertyCount(); i++) {
            QMetaProperty prop = mo->property(i);
            qCDebug(qLcIfSimulationEngine) << "property:" << prop.propertyIndex() << prop.name();
            QMetaMethod method = prop.notifySignal();
            qCDebug(qLcIfSimulationEngine) << "signal: " << method.methodIndex() << method.methodSignature();
        }
    }

    return *builder.toMetaObject();
}

bool QIfSimulationProxyBase::callQmlMethod(const char *function, QGenericReturnArgument ret, QGenericArgument val0, QGenericArgument val1, QGenericArgument val2, QGenericArgument val3, QGenericArgument val4, QGenericArgument val5, QGenericArgument val6, QGenericArgument val7, QGenericArgument val8, QGenericArgument val9)
{
    if (m_noSimulationEngine)
        return false;

    bool functionExecuted = false;
    const QMetaObject *mo = metaObject();

    // Only invoke the functions declared in QML.
    // Once a function/property is added to a type a new MetaObject gets created which contains
    // _QML in the name.
    // _QML_ For a C++ type registered to QML
    // _QMLTYPE_ For a QML type derived from a C++ type

    // qDebug() << "METAOBJECT" << mo->className();

    while (mo && !functionExecuted && QString::fromLatin1(mo->className()).contains(QLatin1String("_QML"))) {
        for (int i=mo->methodOffset(); i<mo->methodCount(); i++) {
            //qDebug() << "CHECKING FOR: " << function << mo->method(i).name();
            if (mo->method(i).name() != function)
                continue;
            //qDebug() << "EXECUTING";
            functionExecuted = QMetaObject::invokeMethod(this, function, ret, val0, val1, val2, val3, val4, val5, val6, val7, val8, val9);
            break;
        }
        mo = mo->superClass();
    }
    return functionExecuted;
}

void QIfSimulationProxyBase::setup(QIfSimulationEngine *engine)
{
    if (engine != qmlEngine(this)) {
        qmlWarning(this) << "QIfSimulationProxy can only be used in the same Engine it is registered in";
        m_noSimulationEngine = true;
        return;
    }

    // Connect all signals from the instance to the signals of this metaobject.
    // This is needed to relay the signals from the instance to this instance and to QML
    const QMetaObject *mo = m_instance->metaObject();
    for (int i=0; i<mo->methodCount(); i++) {
        QMetaMethod mm = mo->method(i);
        if (mm.methodType() != QMetaMethod::Signal)
            continue;
        connect(m_instance, mm, this, m_staticMetaObject->method(m_methodMap.value(i)));
    }
}

} //namespace

QT_END_NAMESPACE
