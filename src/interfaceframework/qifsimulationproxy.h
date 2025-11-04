// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QIFSIMULATIONPROXY_H
#define QIFSIMULATIONPROXY_H

#include <QtInterfaceFramework/QtInterfaceFrameworkModule>

#include <QtCore/QObject>
#include <QtCore/QLoggingCategory>
#include <QtCore/QVariant>
#include <QtCore/QMetaObject>
#include <QtQml/QQmlParserStatus>

QT_BEGIN_NAMESPACE

Q_DECLARE_LOGGING_CATEGORY(qLcIfSimulationEngine)
Q_DECLARE_EXPORTED_LOGGING_CATEGORY(qLcIfRecGuard,
                                    Q_QTINTERFACEFRAMEWORK_EXPORT)

class QIfSimulationEngine;

// The classes here can't be moved to a private header as they are used in macros in the user code
// They are still considered private as they shouldn't be used directly by the user.
namespace qtif_private {

    // This is needed as QVariant doesn't support returning void
    // It is used to cast the variant to the needed return type and use it in the return statement.
    template <typename T> struct QIfReturnValueHelper {
        static T value(const QVariant &var)
        {
            return var.value<T>();
        }
    };

    template <> struct QIfReturnValueHelper <void> {
        static void value(const QVariant &var)
        {
            Q_UNUSED(var);
            return;
        }
    };

    class Q_QTINTERFACEFRAMEWORK_EXPORT QIfSimulationProxyBase : public QObject, public QQmlParserStatus
    {
        Q_INTERFACES(QQmlParserStatus)

    public:
        QIfSimulationProxyBase(QMetaObject *staticMetaObject, QObject *instance, const QHash<int, int> &methodMap, QObject *parent=nullptr);

        virtual const QMetaObject *metaObject() const override;
        virtual void *qt_metacast(const char *classname) override;
        virtual int qt_metacall(QMetaObject::Call call, int methodId, void **a) override;

        void classBegin() override;
        void componentComplete() override;

        typedef void (*StaticMetacallFunction)(QObject *, QMetaObject::Call, int, void **);
        static QMetaObject buildObject(const QMetaObject *metaObject, QHash<int, int> &methodMap, QIfSimulationProxyBase::StaticMetacallFunction metaCallFunction);

        bool callQmlMethod(const char* function,
                          QGenericReturnArgument ret,
                          QGenericArgument val0 = QGenericArgument(nullptr),
                          QGenericArgument val1 = QGenericArgument(),
                          QGenericArgument val2 = QGenericArgument(),
                          QGenericArgument val3 = QGenericArgument(),
                          QGenericArgument val4 = QGenericArgument(),
                          QGenericArgument val5 = QGenericArgument(),
                          QGenericArgument val6 = QGenericArgument(),
                          QGenericArgument val7 = QGenericArgument(),
                          QGenericArgument val8 = QGenericArgument(),
                          QGenericArgument val9 = QGenericArgument());

        template<typename... Ts>
        bool callQmlMethod(const char* function, QVariant &returnValue, Ts... args)
        {
            return QIfSimulationProxyBase::callQmlMethod(function, QReturnArgument<QVariant>("QVariant", returnValue), QArgument<QVariant>("QVariant", QVariant::fromValue(args))...);
        }

    protected:
        void setup(QIfSimulationEngine *engine);

    private:
        bool m_noSimulationEngine;
        QObject *m_instance;
        QMetaObject *m_staticMetaObject;
        QHash<int, int> m_methodMap;
    };

    template <typename T> class QIfSimulationProxy: public QIfSimulationProxyBase
    {
    public:
        Q_DISABLE_COPY_MOVE(QIfSimulationProxy)
        QIfSimulationProxy(QObject *p=nullptr)
            : QIfSimulationProxyBase(&staticMetaObject, m_instance, methodMap(), p)
        {
            Q_ASSERT_X(m_instance, "QIfSimulationProxy()", "QIfSimulationProxy::registerInstance needs to be called first");
        }

        ~QIfSimulationProxy() override
        {
            proxies.removeAll(this);
        }

        void classBegin() override
        {
            QIfSimulationProxyBase::setup(m_engine);
            proxies.append(this);
        }

        // Function is used from QML when reading a property. The static QMetaObject has this function set
        // as the handler for all static meta calls
        static void qt_static_metacall(QObject *obj, QMetaObject::Call call, int methodId, void **a)
        {
            if (!obj)
                return;
            Q_ASSERT_X(m_instance, "qt_static_metacall()", "QIfSimulationProxy::registerInstance needs to be called first");
            // As the class acts as a proxy, forward all calls here to the registered instance
            // The methodIds start at 0 for the first property of this class. We need to add the
            // offset to get the absolute property index for the normal qt_metacall
            if (call == QMetaObject::ReadProperty || call == QMetaObject::WriteProperty) {
                obj->qt_metacall(call, methodId + staticMetaObject.propertyOffset(), a);
                return;
            }

            obj->qt_metacall(call, methodId, a);
        }

        static void registerInstance(QIfSimulationEngine *engine, T *instance)
        {
            Q_ASSERT_X(staticMetaObject.d.data, "registerInstance", "QIfSimulationProxy::buildMetaObject needs to be called first");

            m_engine = engine;
            m_instance = instance;
        }

        static QHash<int, int> &methodMap()
        {
            static QHash<int, int> map;
            return map;
        }

        static void buildMetaObject()
        {
            if (!staticMetaObject.d.data)
                staticMetaObject = QIfSimulationProxy<T>::buildObject(&T::staticMetaObject, QIfSimulationProxy<T>::methodMap(), &QIfSimulationProxy<T>::qt_static_metacall);
        }

        static QMetaObject staticMetaObject;
        static QList<QIfSimulationProxy<T> *> proxies;
        static QByteArray recursionGuard;

    private:
        static QIfSimulationEngine *m_engine;
        static T *m_instance;
    };

    template <typename T> QMetaObject QIfSimulationProxy<T>::staticMetaObject = QMetaObject();
    template <typename T> T *QIfSimulationProxy<T>::m_instance = nullptr;
    template <typename T> QIfSimulationEngine *QIfSimulationProxy<T>::m_engine = nullptr;
    template <typename T> QList<QIfSimulationProxy<T> *> QIfSimulationProxy<T>::proxies =  QList<QIfSimulationProxy<T> *>();
    template <typename T> QByteArray QIfSimulationProxy<T>::recursionGuard = QByteArray();

    template <class T> class RecursionGuard
    {
    public:
        Q_DISABLE_COPY_MOVE(RecursionGuard)

        RecursionGuard() {
            m_savedValue = qtif_private::QIfSimulationProxy<T>::recursionGuard;
        }

        ~RecursionGuard() {
            qCDebug(qLcIfRecGuard, "Reset recursion guard to: %s", m_savedValue.constData());
            // When the recursion guards gets destroyed we want to restore the previous value.
            // This helps to prevent recursions also for the following script call.
            qtif_private::QIfSimulationProxy<T>::recursionGuard = m_savedValue;
        }

        bool trySet(const QByteArray &value) {
            // Try to set the recursion guard to a new value
            // If the guard is already at that value, this calls fails and QIF_SIMULATION_TRY_CALL_FUNC
            // can act accordingly and not call the QML function again.
            if (qtif_private::QIfSimulationProxy<T>::recursionGuard == value) {
                qCDebug(qLcIfRecGuard, "Prevent recursion calling: %s", value.constData());
                return false;
            }

            qCDebug(qLcIfRecGuard, "Update recursion guard to: %s", value.constData());
            qtif_private::QIfSimulationProxy<T>::recursionGuard = value;
            return true;
        }

        void saveAndRelease() {
            // If the recursion is prevented this function can be used to save the current
            // guard for later and reset the guard.
            // Reseting the guard is important as a signal might be emitted after this call.
            // If the slots for those signals are executed directly, new calls to the simulation
            // (via the QtIF frontend API) are allowed.
            qCDebug(qLcIfRecGuard, "Disable recursion guard and saving current value");
            m_savedValue = qtif_private::QIfSimulationProxy<T>::recursionGuard;
            qtif_private::QIfSimulationProxy<T>::recursionGuard.clear();
        }

        private:
            QByteArray m_savedValue;
    };
}


#define QIF_SIMULATION_TRY_CALL_FUNC(instance_type, function, ret_func, ...) \
qtif_private::RecursionGuard<instance_type> _guard; \
if (_guard.trySet(function)) { \
    for (auto _qif_instance : qtif_private::QIfSimulationProxy<instance_type>::proxies) { \
        QVariant return_value; \
        if (_qif_instance->callQmlMethod(function, return_value, ##__VA_ARGS__)) { \
            ret_func; \
        } \
    } \
} else { \
    _guard.saveAndRelease(); \
} \


#define QIF_SIMULATION_TRY_CALL(instance_type, function, ret_type, ...) \
QIF_SIMULATION_TRY_CALL_FUNC(instance_type, function, return qtif_private::QIfReturnValueHelper<ret_type>::value(return_value);, ##__VA_ARGS__) \

QT_END_NAMESPACE

#endif // QIFSIMULATIONPROXY_H
