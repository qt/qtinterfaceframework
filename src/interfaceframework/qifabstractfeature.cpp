// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qifabstractfeature.h"
#include "qifabstractfeature_p.h"
#include "qtifglobal_p.h"
#include "qtinterfaceframeworkmodule.h"
#include "qifconfiguration_p.h"

#include "qifservicemanager.h"
#include "qifservicemanager_p.h"
#include "qifserviceobject.h"

#include <QDebug>
#include <QMetaEnum>

using namespace Qt::StringLiterals;

QT_BEGIN_NAMESPACE

QIfAbstractFeaturePrivate::QIfAbstractFeaturePrivate(const QString &interfaceName, QIfAbstractFeature *parent)
    : QObjectPrivate()
    , q_ptr(parent)
    , m_interface(interfaceName)
    , m_serviceObject(nullptr)
    , m_discoveryMode(QIfAbstractFeature::AutoDiscovery)
    , m_discoveryResult(QIfAbstractFeature::NoResult)
    , m_backendUpdatesEnabled(true)
    , m_asynchronousBackendLoading(false)
    , m_currentServiceHandleIndex(-1)
    , m_currentSearch(QIfServiceManager::IncludeProductionBackends)
    , m_error(QIfAbstractFeature::NoError)
    , m_qmlCreation(false)
    , m_isInitialized(false)
    , m_isConnected(false)
{
    QtInterfaceFrameworkModule::registerTypes();

    qRegisterMetaType<QIfAbstractFeature::Error>();
    qRegisterMetaType<QIfAbstractFeature::DiscoveryMode>();
    qRegisterMetaType<QIfAbstractFeature::DiscoveryResult>();
}

void QIfAbstractFeaturePrivate::initialize()
{
}

/*!
    \internal Returns the backend object retrieved from calling interfaceInstance() with the
    interfaceName of this private class.

    For most classes, this is the sane default and it provides a convenient way to get the backend
    interface and also allow for manually overwriting it with something else.

    If the derived class needs to connect to a different interface than the one defined by
    \c interfaceName or to an additional interface, it can still manually ask for the required
    \c interfaceInstance using the QIfServiceObject directly.
*/
QIfFeatureInterface *QIfAbstractFeaturePrivate::backend() const
{
    Q_Q(const QIfAbstractFeature);
    if (m_serviceObject)
        return m_serviceObject->interfaceInstance(q->interfaceName());
    return nullptr;
}

QIfAbstractFeaturePrivate *QIfAbstractFeaturePrivate::get(QIfAbstractFeature *q)
{
    return static_cast<QIfAbstractFeaturePrivate *>(q->d_ptr.data());
}

void QIfAbstractFeaturePrivate::setDiscoveryResult(QIfAbstractFeature::DiscoveryResult discoveryResult)
{
    if (m_discoveryResult == discoveryResult)
        return;

    m_discoveryResult = discoveryResult;
    Q_Q(QIfAbstractFeature);
    emit q->discoveryResultChanged(discoveryResult);
}

void QIfAbstractFeaturePrivate::onInitializationDone()
{
    if (m_isInitialized)
        return;

    m_isInitialized = true;
    Q_Q(QIfAbstractFeature);
    emit q->isInitializedChanged(true);
}

void QIfAbstractFeaturePrivate::serviceObjectDestroyed()
{
    Q_Q(QIfAbstractFeature);
    m_serviceObject = nullptr;
    m_isInitialized = false;
    emit q->isInitializedChanged(m_isInitialized);
    q->clearServiceObject();
    emit q->serviceObjectChanged();
}

void QIfAbstractFeaturePrivate::loadServiceObject(QIfServiceManager::SearchFlag searchFlag)
{
    QIfServiceManager *serviceManager = QIfServiceManager::instance();
    m_currentSearch = searchFlag;

    if (m_currentServiceHandleIndex == -1) {
        m_serviceHandles = serviceManager->findServiceHandleByInterface(m_interface, searchFlag, m_preferredBackends);
        if (!m_serviceHandles.isEmpty())
            m_currentServiceHandleIndex = 0;
    }

    if (m_serviceHandles.isEmpty()) {
        onServiceObjectFailure();
        return;
    }

    qCDebug(qLcIfServiceManagement) << "Loading first ServiceObject";
    auto currentHandle = m_serviceHandles.at(m_currentServiceHandleIndex);
    if (currentHandle.isLoaded())
        onServiceObjectLoaded(currentHandle);
    else
        serviceManager->loadServiceObject(currentHandle, m_asynchronousBackendLoading);
}

void QIfAbstractFeaturePrivate::onServiceObjectLoaded(QIfServiceObjectHandle handle)
{
    Q_Q(QIfAbstractFeature);

    if (m_currentServiceHandleIndex < 0 || m_currentServiceHandleIndex >= m_serviceHandles.count())
        return;

    auto currentHandle = m_serviceHandles.at(m_currentServiceHandleIndex);
    if (currentHandle != handle)
        return;
    qCDebug(qLcIfServiceManagement) << "ServiceObject loaded";

    QIfServiceObject *object = currentHandle.serviceObject();
    qCDebug(qLcIfServiceManagement) << "Trying to use" << object << "Supported Interfaces:" << object->interfaces();
    if (q->setServiceObject(object)) {
        if (Q_UNLIKELY(m_serviceHandles.count() > 1)) {
            qWarning().nospace() << "There is more than one backend implementing " << m_interface
                                 << ". Using the first one (enable \"" << qLcIfServiceManagement().categoryName()
                                 << "\" logging to see which are found)";
        }
        if (m_currentSearch == QIfServiceManager::IncludeProductionBackends)
            setDiscoveryResult(QIfAbstractFeature::ProductionBackendLoaded);
        else
            setDiscoveryResult(QIfAbstractFeature::SimulationBackendLoaded);
        m_currentServiceHandleIndex = -1;
        m_serviceHandles.clear();
    } else {
        if (++m_currentServiceHandleIndex < m_serviceHandles.count()) {
            // Try next ServiceObject
            qCDebug(qLcIfServiceManagement) << "Loading next ServiceObject";
            loadServiceObject(m_currentSearch);
        } else {
            onServiceObjectFailure();
        }
    }
}

void QIfAbstractFeaturePrivate::onServiceObjectFailure()
{
    // No available ServiceObjects left
    if (m_currentSearch == QIfServiceManager::IncludeProductionBackends)
        qWarning() << "There is no production backend implementing" << m_interface << ".";
    else
        qWarning() << "There is no simulation backend implementing" << m_interface << ".";

    if (m_currentSearch == QIfServiceManager::IncludeProductionBackends && m_discoveryMode == QIfAbstractFeature::AutoDiscovery) {
        // Try again only with simulation backends
        m_currentServiceHandleIndex = -1;
        loadServiceObject(QIfServiceManager::IncludeSimulationBackends);
        return;
    }

    qWarning() << "No suitable ServiceObject found.";
    m_currentServiceHandleIndex = -1;
    m_serviceHandles.clear();
    setDiscoveryResult(QIfAbstractFeature::ErrorWhileLoading);
}

/*!
    \class QIfAbstractFeature
    \inmodule QtInterfaceFramework
    \brief The QIfAbstractFeature is the base class for all QtInterfaceFramework Features.

    QIfAbstractFeature is the base class for the front-facing API towards the developer. Subclass
    QIfAbstractFeature to create an API for your feature.

    QIfAbstractFeature provides you with auto discovery: a way to automatically connect to a
    backend that implements the required interface. To discover a backend, use the
    startAutoDiscovery() function. Once auto discovery is complete, use the isValid() function to
    check whether a suitable backend was found.

    The auto discovery gives you an easy way to automatically connect to the correct backend
    implementation and can be controlled by the \c discoveryMode property. The \c discoveryMode is
    set to \c AutoDiscovery by default, which indicates that a production backend is always
    preferred over a simulation backend.

    Alternatively, it's also possible to use QIfServiceManager to retrieve all backends. Then,
    manually search for the right backend and call setServiceObject() to connect it to the
    QIfAbstractFeature.

    \section1 Write a Subclass

    Your QIfAbstractFeature subclass must provide implementations for the following functions:

    \list
    \li acceptServiceObject()
    \li connectToServiceObject()
    \li disconnectFromServiceObject()
    \li clearServiceObject()
    \endlist

    Once a QIfServiceObject has been set, either via startAutoDiscovery() or setServiceObject(),
    call the acceptServiceObject() function to make sure that the feature you've implemented
    can work with the QIfServiceObject and, in turn, the QIfServiceObject provides the required
    interface.

    If the interface provides signals, you need to make all the connect statements in
    connectToServiceObject(); then disconnect them in disconnectFromServiceObject().

    clearServiceObject() is called once the feature doesn't have a connection to a ServiceObject
    anymore and needs to reset its state to feasible defaults.
*/

/*!
    \enum QIfAbstractFeature::Error

    \value NoError
           No error
    \value PermissionDenied
           Permission for the operation is denied
    \value InvalidOperation
           Operation is invalid
    \value Timeout
           Operation timeout
    \value InvalidZone
           Zone is not available for the operation
    \value Unknown
           Unknown error
*/

/*!
    \enum QIfAbstractFeature::DiscoveryMode

    \value InvalidAutoDiscovery
           An invalid value. This value is used when the enum is created from a string value
           (e.g. when parsed from a ini file) and it failed to parse the string.
    \value NoAutoDiscovery
           No auto discovery is done and the ServiceObject needs to be set manually.
    \value AutoDiscovery
           The feature first tries to find a production backend with a matching interface. If it's
           not available, then the feature falls back to a simulation backend.
    \value LoadOnlyProductionBackends
           The feature tries to load a production backend with a matching interface only.
    \value LoadOnlySimulationBackends
           The feature tries to load a simulation backend with a matching interface only.
*/

/*!
    \enum QIfAbstractFeature::DiscoveryResult

    \value NoResult
           Indicates that no auto discovery was started because the feature already has a valid
           ServiceObject assigned.
    \value ErrorWhileLoading
           An error has occurred while searching for a backend with a matching interface.
    \value ProductionBackendLoaded
           A production backend was loaded, as a result of auto discovery.
    \value SimulationBackendLoaded
           A simulation backend was loaded, as a result of auto discovery.
*/

/*!
    \qmltype AbstractFeature
    \qmlabstract
    \instantiates QIfAbstractFeature
    \inqmlmodule QtInterfaceFramework

    \brief The AbstractFeature is not directly accessible. The QML type provides
    base QML properties for the feature, like autoDiscovery and isValid.

    Once the AbstractFeature is instantiated by QML the autoDiscovery will be started automatically.
    To disable this behavior the discoveryMode can be set to \c NoDiscovery;
*/

/*!
    \fn void QIfAbstractFeature::clearServiceObject()

    This method is expected to be implemented by any class subclassing QIfAbstractFeature.

    Called when no service object is available. The implementation is expected to set all
    properties to safe defaults and forget all links to the previous service object.

    \note You must emit the corresponding change signals for these properties, so that the feature
    is informed about the state change. This makes it possible for the implemented class to connect
    to a new service object afterwards.

    There is no need to disconnect from the service object. If it still exists, it is guaranteed
    that \l disconnectFromServiceObject is called first.

    \sa acceptServiceObject(), connectToServiceObject(), disconnectFromServiceObject()
*/

/*!
    Constructs an abstract feature.

    The \a parent argument is passed on to the \l QObject constructor.

    The \a interfaceName argument is used to locate suitable service objects.
*/
QIfAbstractFeature::QIfAbstractFeature(const QString &interfaceName, QObject *parent)
    : QObject(*new QIfAbstractFeaturePrivate(interfaceName, this), parent)
{
    Q_D(QIfAbstractFeature);
    d->initialize();

    QObject::connect(QIfServiceManager::instance(), &QIfServiceManager::serviceObjectLoaded, this, [d](QIfServiceObjectHandle handle) {
        d->onServiceObjectLoaded(handle);
    });
}

QIfAbstractFeature::~QIfAbstractFeature()
{
    Q_D(QIfAbstractFeature);
    if (!d->m_configurationId.isEmpty())
        QIfConfigurationManager::instance()->removeAbstractFeature(d->m_configurationId, this);
}

/*!
    \qmlproperty ServiceObject AbstractFeature::serviceObject
    \brief Sets the service object for the feature.

    As Features only expose the front API facing the developer, a service object implementing the
    actual function is required. This is usually retrieved through the auto discovery mechanism.

    The setter for this property returns false if the \c QIfServiceObject is already set to this
    particular instance or the QIfServiceObject isn't accepted by the feature.

    \sa discoveryMode
*/
/*!
    \property QIfAbstractFeature::serviceObject
    \brief Sets the service object for the feature.

    As Features only expose the front API facing the developer, a service object implementing the
    actual function is required. This is usually retrieved through the auto discovery mechanism.

    The setter for this property returns false if the \c QIfServiceObject is already set to this
    particular instance or the QIfServiceObject isn't accepted by the feature.

    \sa discoveryMode
*/
bool QIfAbstractFeature::setServiceObject(QIfServiceObject *so)
{

    Q_D(QIfAbstractFeature);
    if (d->m_serviceObject == so)
        return false;

    bool serviceObjectIsSet = d->m_serviceObject;
    if (d->m_serviceObject) {
        disconnectFromServiceObject(d->m_serviceObject);
        QObjectPrivate::disconnect(d->m_serviceObject, &QObject::destroyed, d, &QIfAbstractFeaturePrivate::serviceObjectDestroyed);
    }

    d->m_serviceObject = nullptr;

    //We only want to call clearServiceObject if we are sure that the serviceObject changes
    if (!so) {
        clearServiceObject();
    } else if (Q_UNLIKELY(so && !acceptServiceObject(so))) {
        qWarning("ServiceObject is not accepted");
        clearServiceObject();

        if (serviceObjectIsSet) {
            emit serviceObjectChanged();
            emit isValidChanged(isValid());
        }
        return false;
    }

    d->m_serviceObject = so;
    emit serviceObjectChanged();
    emit isValidChanged(isValid());

    if (so) {
        if (d->m_backendUpdatesEnabled) {
            connectToServiceObject(d->m_serviceObject);
            if (!d->m_isConnected) {
                qCritical() << this <<
                          "accepted the given QIfServiceObject, but didn't connect to it completely"
                          ", as QIfAbstractFeature::connectToServiceObject wasn't called.";
                return false;
            }
        }
        QObjectPrivate::connect(so, &QObject::destroyed, d, &QIfAbstractFeaturePrivate::serviceObjectDestroyed);
    }

    return true;
}

/*!
    \qmlproperty enumeration AbstractFeature::discoveryMode
    \brief Holds the mode that is used for the autoDiscovery

    Available values are:
    \value NoAutoDiscovery
           No auto discovery is done and the ServiceObject needs to be set manually.
    \value AutoDiscovery
           Tries to find a production backend with a matching interface and falls back to a
           simulation backend if not found.
    \value LoadOnlyProductionBackends
           Only tries to load a production backend with a matching interface.
    \value LoadOnlySimulationBackends
           Only tries to load a simulation backend with a matching interface.

    If necessary, auto discovery is started once the feature creation is completed.

    \note If you change this property after the feature is instantiated, make sure to call
    startAutoDiscovery() to search for a new service object.
*/

/*!
    \property QIfAbstractFeature::discoveryMode
    \brief Holds the mode that is used for the autoDiscovery

    \note If you change this property after the feature is instantiated, make sure to call
    startAutoDiscovery() to search for a new service object.
*/
void QIfAbstractFeature::setDiscoveryMode(QIfAbstractFeature::DiscoveryMode discoveryMode)
{
    Q_D(QIfAbstractFeature);
    if (d->m_discoveryMode == discoveryMode || discoveryMode == QIfAbstractFeature::InvalidAutoDiscovery)
        return;

    d->m_discoveryMode = discoveryMode;
    emit discoveryModeChanged(discoveryMode);
}

/*!
    \internal
    \overload
*/
void QIfAbstractFeature::classBegin()
{
    Q_D(QIfAbstractFeature);
    d->m_qmlCreation = true;
}

/*!
    Invoked automatically when used from QML. Calls \l startAutoDiscovery().
*/
void QIfAbstractFeature::componentComplete()
{
    Q_D(QIfAbstractFeature);
    d->m_qmlCreation = false;

    if (!d->m_configurationId.isEmpty())
        QIfConfigurationManager::instance()->addAbstractFeature(d->m_configurationId, this);

    startAutoDiscovery();
}

/*!
    Returns the interface name this feature is implementing.

    When the feature discovers a matching backend, this interface's name needs to be supported by
    the service object that the feature is connecting to.

    \sa acceptServiceObject
*/
QString QIfAbstractFeature::interfaceName() const
{
    Q_D(const QIfAbstractFeature);
    return d->m_interface;
}

QIfServiceObject *QIfAbstractFeature::serviceObject() const
{
    Q_D(const QIfAbstractFeature);
    return d->m_serviceObject;
}

QIfAbstractFeature::DiscoveryMode QIfAbstractFeature::discoveryMode() const
{
    Q_D(const QIfAbstractFeature);
    return d->m_discoveryMode;
}

/*!
    \qmlproperty enumeration AbstractFeature::discoveryResult
    \brief The result of the last autoDiscovery

    Available values are:
    \value NoResult
           Indicates that no auto discovery was started because the feature has already assigned a
           valid ServiceObject.
    \value ErrorWhileLoading
           An error has happened while searching for a backend with a matching interface.
    \value ProductionBackendLoaded
           A production backend was loaded, as a result of auto discovery.
    \value SimulationBackendLoaded
           A simulation backend was loaded, as a result of auto discovery.
*/

/*!
    \property QIfAbstractFeature::discoveryResult
    \brief The result of the last autoDiscovery

    \sa startAutoDiscovery()
*/
QIfAbstractFeature::DiscoveryResult QIfAbstractFeature::discoveryResult() const
{
    Q_D(const QIfAbstractFeature);
    return d->m_discoveryResult;
}

/*!
    \qmlproperty string AbstractFeature::configurationId
    \brief Holds the id to determine which configuration this feature belongs to.

    Once the id has been set, it is possible to change certain values using the
    \l InterfaceFrameworkConfiguration API.

    \note Values set in the matching \l InterfaceFrameworkConfiguration can override the initial values
    set during the component creation.

    \sa InterfaceFrameworkConfiguration
*/

/*!
    \property QIfAbstractFeature::configurationId
    \brief Holds the id to determine which configuration this feature belongs to.

    Once the id has been set, it is possible to change certain values using the
    \l QIfConfiguration API.

    \note Values set in the matching \l QIfConfiguration can override the initial values
    set during the component creation.

    \sa QIfConfiguration
*/
QString QIfAbstractFeature::configurationId() const
{
    Q_D(const QIfAbstractFeature);
    return d->m_configurationId;
}

void QIfAbstractFeature::setConfigurationId(const QString &configurationId)
{
    Q_D(QIfAbstractFeature);
    if (d->m_configurationId == configurationId)
        return;

    if (!d->m_configurationId.isEmpty())
        QIfConfigurationManager::instance()->removeAbstractFeature(d->m_configurationId, this);

    d->m_configurationId = configurationId;

    if (!configurationId.isEmpty() && !d->m_qmlCreation)
        QIfConfigurationManager::instance()->addAbstractFeature(configurationId, this);

    emit configurationIdChanged(configurationId);
}

/*!
    \qmlproperty list<string> AbstractFeature::preferredBackends
    \brief Holds a list of wildcards to load the preferred backend during auto discovery.

    The auto discovery mechanism will automatically search for backends which provide a matching
    interface for this feature implementation. See \l startAutoDiscovery() for more information.

    In case multiple backends implement the same interface, the list of wildcards can be used
    to determine the correct one to load.

    The wildcards are applied in order to the found backends. If the wildcard matches some backends
    those backends will be loaded, otherwise the next wildcard is used.

    For example: Given an AbstractFeature with two backends, backend_mqtt.so and backend_qtro.so,
    the property can be set to \c "*_mqtt*" to always select the backend_mqtt.so backend when
    available.
*/

/*!
    \property QIfAbstractFeature::preferredBackends
    \brief Holds a list of wildcards to load the preferred backend during auto discovery.

    The auto discovery mechanism will automatically search for backends which provide a matching
    interface for this feature implementation. See \l startAutoDiscovery() for more information.

    In case multiple backends implement the same interface, the list of wildcards can be used
    to determine the correct one to load.

    The wildcards are applied in order to the found backends. If the wildcard matches some backends
    those backends will be loaded, otherwise the next wildcard is used.

    For example: Given an AbstractFeature with two backends, backend_mqtt.so and backend_qtro.so,
    the property can be set to \c "*_mqtt*" to always select the backend_mqtt.so backend when
    available.
*/
QStringList QIfAbstractFeature::preferredBackends() const
{
    Q_D(const QIfAbstractFeature);
    return d->m_preferredBackends;
}

void QIfAbstractFeature::setPreferredBackends(const QStringList &preferredBackends)
{
    Q_D(QIfAbstractFeature);
    if (d->m_preferredBackends == preferredBackends)
        return;
    d->m_preferredBackends = preferredBackends;
    emit preferredBackendsChanged(preferredBackends);
}

/*!
    \qmlproperty bool AbstractFeature::backendUpdatesEnabled
    \brief This property holds whether backend updates are enabled

    By default, this property is \c true.

    setBackendUpdatesEnabled() is normally used to disable updates for a short period of time, for instance
    to skip expensive updates while the application is currently not visible on the screen.

    This is especially useful when backend upates are triggered over an IPC and received by multiple
    applications. By disabling updates, the application can avoid unnecessary updates.

    A change to this property will cause the connectToServiceObject() and disconnectFromServiceObject()
    functions to be called, depending on the new value.
*/

/*!
    \property QIfAbstractFeature::backendUpdatesEnabled
    \brief This property holds whether backend updates are enabled

    By default, this property is \c true.

    setBackendUpdatesEnabled() is normally used to disable updates for a short period of time, for instance
    to skip expensive updates while the application is currently not visible on the screen.

    This is especially useful when backend upates are triggered over an IPC and received by multiple
    applications. By disabling updates, the application can avoid unnecessary updates.

    A change to this property will cause the connectToServiceObject() and disconnectFromServiceObject()
    functions to be called, depending on the new value.
*/
bool QIfAbstractFeature::backendUpdatesEnabled() const
{
    Q_D(const QIfAbstractFeature);
    return d->m_backendUpdatesEnabled;
}

void QIfAbstractFeature::setBackendUpdatesEnabled(bool newBackendUpdatesEnabled)
{
    Q_D(QIfAbstractFeature);
    if (d->m_backendUpdatesEnabled == newBackendUpdatesEnabled)
        return;
    d->m_backendUpdatesEnabled = newBackendUpdatesEnabled;
    if (d->m_serviceObject) {
        if (d->m_backendUpdatesEnabled)
            connectToServiceObject(d->m_serviceObject);
        else
            disconnectFromServiceObject(d->m_serviceObject);
    }
    emit backendUpdatesEnabledChanged(newBackendUpdatesEnabled);
}

bool QIfAbstractFeature::asynchronousBackendLoading() const
{
    Q_D(const QIfAbstractFeature);
    return d->m_asynchronousBackendLoading;
}

void QIfAbstractFeature::setAsynchronousBackendLoading(bool asynchronousBackendLoading)
{
    Q_D(QIfAbstractFeature);

    if (d->m_asynchronousBackendLoading == asynchronousBackendLoading)
        return;
    d->m_asynchronousBackendLoading = asynchronousBackendLoading;
    emit asynchronousBackendLoadingChanged(asynchronousBackendLoading);
}

/*!
    Sets \a error with the \a message.

    Emits errorChanged() signal.

    \sa QIfAbstractZonedFeature::Error
*/
void QIfAbstractFeature::setError(QIfAbstractFeature::Error error, const QString &message)
{
    Q_D(QIfAbstractFeature);
    d->m_error = error;
    d->m_errorMessage = errorText() + u" "_s + message;
    if (d->m_error == QIfAbstractFeature::NoError)
        d->m_errorMessage.clear();
    emit errorChanged(d->m_error, d->m_errorMessage);
}

/*!
    Returns the last error code.

    \sa QIfAbstractFeature::Error
*/
QIfAbstractFeature::Error QIfAbstractFeature::error() const
{
    Q_D(const QIfAbstractFeature);
    return d->m_error;
}


/*!
    \qmlproperty string QIfAbstractFeature::error

    Last error message of the feature. Empty if no error.
*/
/*!
    \property QIfAbstractFeature::error

    Last error message of the feature. Empty if no error.
*/
QString QIfAbstractFeature::errorMessage() const
{
    Q_D(const QIfAbstractFeature);
    return d->m_errorMessage;
}

/*!
    Returns the current error code converted from QIfAbstractFeature::Error to QString

    \sa error
*/
QString QIfAbstractFeature::errorText() const
{
    Q_D(const QIfAbstractFeature);
    if (d->m_error == QIfAbstractFeature::NoError)
        return QString();
    QMetaEnum metaEnum = QMetaEnum::fromType<QIfAbstractFeature::Error>();
    return QLatin1String(metaEnum.valueToKey(d->m_error));
}


/*!
    \qmlmethod enumeration AbstractFeature::startAutoDiscovery()

    Performs an automatic discovery attempt.

    The feature tries to locate a single ServiceObject that implements the required interface.

    If no ServiceObject is found, the feature remains invalid. If more than one ServiceObject
    is found, the \b first instance is used.

    This function returns either the type of the backend that was loaded; or an error.

    If the \c discoveryMode is set to QIfAbstractFeature::NoAutoDiscovery, this function does
    nothing and returns QIfAbstractFeature::NoResult.

    Return values are:
    \value NoResult
           Indicates that no auto discovery was started because the feature already has
           a valid ServiceObject assigned.
    \value ErrorWhileLoading
           Indicates an error has occurred while searching for a backend with a matching
           interface.
    \value ProductionBackendLoaded
           A production backend was loaded, as a result of auto discovery.
    \value SimulationBackendLoaded
           A simulation backend was loaded, as a result of auto discovery.

    \sa {Dynamic Backend System} QIfServiceManager
*/

/*!
    \brief Performs an automatic discovery attempt.

    The feature will try to locate a single service object implementing the required interface.

    If no service object is found, the feature will stay invalid. If more than one service object
    is found, the first instance is used.

    Either the type of the backend which was loaded or an error is returned.

    If the discoveryMode is set to QIfAbstractFeature::NoAutoDiscovery this function will
    do nothing and return QIfAbstractFeature::NoResult.

    \sa discoveryMode() {Dynamic Backend System}
*/
QIfAbstractFeature::DiscoveryResult QIfAbstractFeature::startAutoDiscovery()
{
    Q_D(QIfAbstractFeature);

    // No need to discover a new backend when we already have one
    if (d->m_serviceObject || d->m_discoveryMode == QIfAbstractFeature::NoAutoDiscovery) {
        d->setDiscoveryResult(NoResult);
        return NoResult;
    }

    if (d->m_discoveryMode == QIfAbstractFeature::AutoDiscovery ||  d->m_discoveryMode == QIfAbstractFeature::LoadOnlyProductionBackends)
        d->loadServiceObject(QIfServiceManager::IncludeProductionBackends);
    else
        d->loadServiceObject(QIfServiceManager::IncludeSimulationBackends);

    if (d->m_asynchronousBackendLoading)
        return NoResult;

    return d->m_discoveryResult;
}

QIfAbstractFeature::QIfAbstractFeature(QIfAbstractFeaturePrivate &dd, QObject *parent)
    : QObject(dd, parent)
{
    Q_D(QIfAbstractFeature);
    d->initialize();

    QObject::connect(QIfServiceManager::instance(), &QIfServiceManager::serviceObjectLoaded, this, [d](QIfServiceObjectHandle handle) {
        d->onServiceObjectLoaded(handle);
    });
}

/*!
    This method is expected to be implemented by any class subclassing QIfAbstractFeature.

    The method should return \c true if the given \a serviceObject is accepted and
    can be used, otherwise \c false.

    If the object is accepted, \l connectToServiceObject is called to actually connect to the
    service object.

    The default implementation accepts the \a serviceObject if it implements the interface
    returned by interfaceName();

    \sa connectToServiceObject(), disconnectFromServiceObject(), clearServiceObject()
*/
bool QIfAbstractFeature::acceptServiceObject(QIfServiceObject *serviceObject)
{
    return serviceObject->interfaces().contains(interfaceName());
}

/*!
    This method is expected to be implemented by any class subclassing QIfAbstractFeature.

    The implementation should connect to the \a serviceObject, and set up all
    properties to reflect the state of the service object.

    There is no previous service object connected, as this function call is always preceded by a call to
    \l disconnectFromServiceObject or \l clearServiceObject.

    It is safe to assume that the \a serviceObject, has always been accepted through the
    \l acceptServiceObject method prior to being passed to this method.

    The default implementation connects to the signals offered by QIfFeatureInterface and calls
    QIfFeatureInterface::initialize() afterwards.

    When reimplementing please keep in mind to connect all signals before calling this function. e.g.

    \code
    void SimpleFeature::connectToServiceObject(QIfServiceObject *serviceObject)
    {
        SimpleFeatureBackendInterface *backend = backend(serviceObject);
        if (!backend)
            return;

        // connect your signals
        connect(backend, &SimpleFeatureBackendInterface::propertyChanged,
                this, &SimpleFeature::onPropertyChanged);

        // connects the base signals and call initialize()
        QIfAbstractFeature::connectToServiceObject(serviceObject);

        // Additional initialization functions can be added here
    }
    \endcode

    \sa acceptServiceObject(), disconnectFromServiceObject(), clearServiceObject()
*/
void QIfAbstractFeature::connectToServiceObject(QIfServiceObject *serviceObject)
{
    Q_D(QIfAbstractFeature);
    Q_ASSERT(serviceObject);
    QIfFeatureInterface *backend = d->backend();

    if (backend) {
        connect(backend, &QIfFeatureInterface::errorChanged, this, &QIfAbstractFeature::onErrorChanged);
        QObjectPrivate::connect(backend, &QIfFeatureInterface::initializationDone,
                                d, &QIfAbstractFeaturePrivate::onInitializationDone);
        backend->initialize();
    }

    d->m_isConnected = true;
}

/*!
    This method disconnects all connections to the \a serviceObject.

    There is no need to reset internal variables to safe defaults. A call to this function is
    always followed by a call to \l connectToServiceObject or \l clearServiceObject.

    The default implementation disconnects all signals from the serviceObject to this instance.

    Most of the times you don't have to reimplement this method. A reimplementation is only needed
    if multiple interfaces have been connected before or special cleanup calls need to be done
    to the backend before disconnecting as well.
    If you need to reimplement this function, please make sure to use the interfaceName() method to
    retrieve the backend instance and not hardcode it to a particular interfaceName, as otherwise
    the disconnect calls don't work anymore with derived interfaces.

    \sa acceptServiceObject(), connectToServiceObject(), clearServiceObject()
*/
void QIfAbstractFeature::disconnectFromServiceObject(QIfServiceObject *serviceObject)
{
    Q_D(QIfAbstractFeature);
    Q_ASSERT(serviceObject);
    QObject *backend = d->backend();

    if (backend)
        disconnect(backend, nullptr, this, nullptr);

    d->m_isInitialized = false;
    emit isInitializedChanged(false);
    d->m_isConnected = false;
}

/*!
    \qmlproperty bool AbstractFeature::isValid
    \brief Indicates whether the feature is ready for use.

    The property is \c true if the feature is ready to be used, otherwise \c false. Not being
    ready usually indicates that no suitable service object could be found, or that automatic
    discovery has not been triggered.

    The backend still might not have sent all properties yet and is not fully initialized.
    Use isInitialized instead to know when the feature holds all correct values.

    \sa QIfServiceObject, discoveryMode, isInitialized
*/
/*!
    \property QIfAbstractFeature::isValid
    \brief Indicates whether the feature is ready to use.

    The property is \c true if the feature has an associated service object, otherwise \c false.
    Not being valid usually indicates that no suitable service object could be found, or that
    automatic discovery has not been triggered.

    The backend still might not have sent all properties yet and is not fully initialized.
    Use isInitialized instead to know when the feature holds all correct values.

    \sa QIfServiceObject, discoveryMode, isInitialized
*/
bool QIfAbstractFeature::isValid() const
{
    Q_D(const QIfAbstractFeature);
    return d->m_serviceObject != nullptr;
}

/*!
    \qmlproperty bool AbstractFeature::isInitialized
    \brief Indicates whether the feature has been initialized with all the values from the backend.

    The property is \c true once the backend sends the QIfFeatureInterface::initializationDone signal
    to indicate that all values have now been initialized with values from the backend.

    \sa isValid, QIfFeatureInterface::initializationDone
*/
/*!
    \property QIfAbstractFeature::isInitialized
    \brief Indicates whether the feature has been initialized with all the values from the backend.

    The property is \c true once the backend sends the QIfFeatureInterface::initializationDone signal
    to indicate that all values have now been initialized with values from the backend.

    \sa isValid, QIfFeatureInterface::initializationDone
*/
bool QIfAbstractFeature::isInitialized() const
{
    Q_D(const QIfAbstractFeature);
    return d->m_isInitialized;
}

/*!
    Updates \a error and \a message from the backend.

    Use this slot when you implement a new feature to report generic errors.
*/
void QIfAbstractFeature::onErrorChanged(QIfAbstractFeature::Error error, const QString &message)
{
    setError(error, message);
}


QT_END_NAMESPACE

#include "moc_qifabstractfeature.cpp"
