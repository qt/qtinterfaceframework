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

#include "qifabstractfeaturelistmodel.h"
#include "qifabstractfeaturelistmodel_p.h"

QT_BEGIN_NAMESPACE

QIfHelperFeature::QIfHelperFeature(const QString &interfaceName, QIfAbstractFeatureListModel *model)
    : QIfAbstractFeature(interfaceName)
    , m_model(model)
{
}

bool QIfHelperFeature::acceptServiceObject(QIfServiceObject *so)
{
    return m_model->acceptServiceObject(so);
}

bool QIfHelperFeature::acceptServiceObjectDefaultImpl(QIfServiceObject *so)
{
    return QIfAbstractFeature::acceptServiceObject(so);
}

void QIfHelperFeature::connectToServiceObject(QIfServiceObject *so)
{
    m_model->connectToServiceObject(so);
}

void QIfHelperFeature::connectToServiceObjectDefaultImpl(QIfServiceObject *so)
{
    QIfAbstractFeature::connectToServiceObject(so);
}

void QIfHelperFeature::disconnectFromServiceObject(QIfServiceObject *so)
{
    m_model->disconnectFromServiceObject(so);
}

void QIfHelperFeature::disconnectFromServiceObjectDefaultImpl(QIfServiceObject *so)
{
    QIfAbstractFeature::disconnectFromServiceObject(so);
}

void QIfHelperFeature::clearServiceObject()
{
    m_model->clearServiceObject();
}

QIfAbstractFeaturePrivate *QIfHelperFeature::ifPrivate()
{
    Q_D(QIfAbstractFeature);
    return d;
}

const QIfAbstractFeaturePrivate *QIfHelperFeature::ifPrivate() const
{
    Q_D(const QIfAbstractFeature);
    return d;
}

QIfAbstractFeatureListModelPrivate::QIfAbstractFeatureListModelPrivate(const QString &interfaceName, QIfAbstractFeatureListModel *model)
    : QAbstractItemModelPrivate()
    , m_feature(new QIfHelperFeature(interfaceName, model))
    , m_qmlCreation(false)
{

}

QIfAbstractFeatureListModelPrivate::~QIfAbstractFeatureListModelPrivate()
{
    delete m_feature;
}

void QIfAbstractFeatureListModelPrivate::initialize()
{
}

QIfFeatureInterface *QIfAbstractFeatureListModelPrivate::backend() const
{
    return m_feature->ifPrivate()->backend();
}

/*!
    \class QIfAbstractFeatureListModel
    \inmodule QtInterfaceFramework
    \brief The QIfAbstractFeatureListModel is the base class for QtInterfaceFramework Features which should act as a model.

    This base class is necessary to avoid virtual inheritance from QAbstractListModel and
    QIfAbstractFeature.

    For more details on how a Feature works, see QIfAbstractFeature.
    For more information about models in QtInterfaceFramework, see \l{Models}.

    \section1 Subclassing

    Your QIfAbstractFeatureListModel subclass must provide implementations for all virtual
    functions from QIfAbstractFeature as well as the virtual functions from QAbstractListModel.

    \list
        \li For more details on how to integrate with the \l{Dynamic Backend System}, see
            \l{Write a Subclass}{QIfAbstractFeature}.
        \li For more details on what you need to do to provide the model's required
            functionality, see \l{QAbstractListModel - Subclassing}.
        \li For a class that implements all the necessary QIfAbstractFeatureListModel functions
            to provide pagination functionality, see QIfPagingModel.
    \endlist

*/

/*!
    \qmltype AbstractFeatureListModel
    \instantiates QIfAbstractFeatureListModel
    \inqmlmodule QtInterfaceFramework

    \brief The QIfAbstractFeatureListModel is the base class for QtInterfaceFramework Features which should act as a model.

    This base class is necessary to avoid virtual inheritance from QAbstractListModel and
    QIfAbstractFeature.

    This element is not directly accessible from QML. It provides the base QML properties for the
    feature, like autoDiscovery and isValid.

    For more details on how a Feature works, see QIfAbstractFeature.
    For more information about models in QtInterfaceFramework, see \l{Models}.

    \sa AbstractFeature
*/

/*!
    \fn void QIfAbstractFeatureListModel::clearServiceObject()

    This method is expected to be implemented by any class subclassing QIfAbstractFeatureListModel.

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
    Constructs a QIfAbstractFeatureListModel.

    The \a parent argument is passed on to the \l QAbstractListModel base class.

    The \a interfaceName argument is used to locate suitable service objects.
*/
QIfAbstractFeatureListModel::QIfAbstractFeatureListModel(const QString &interfaceName, QObject *parent)
    : QAbstractListModel(*new QIfAbstractFeatureListModelPrivate(interfaceName, this), parent)
{
    Q_D(QIfAbstractFeatureListModel);
    d->initialize();
    connect(d->m_feature, &QIfAbstractFeature::serviceObjectChanged, this, &QIfAbstractFeatureListModel::serviceObjectChanged);
    connect(d->m_feature, &QIfAbstractFeature::discoveryModeChanged, this, &QIfAbstractFeatureListModel::discoveryModeChanged);
    connect(d->m_feature, &QIfAbstractFeature::discoveryResultChanged, this, &QIfAbstractFeatureListModel::discoveryResultChanged);
    connect(d->m_feature, &QIfAbstractFeature::isValidChanged, this, &QIfAbstractFeatureListModel::isValidChanged);
    connect(d->m_feature, &QIfAbstractFeature::isInitializedChanged, this, &QIfAbstractFeatureListModel::isInitializedChanged);
    connect(d->m_feature, &QIfAbstractFeature::errorChanged, this, &QIfAbstractFeatureListModel::errorChanged);
}

/*!
    \qmlproperty ServiceObject AbstractFeatureListModel::serviceObject
    \brief Sets the service object for the feature.

    As features only expose the front API facing the developer, a service object implementing the
    actual function is required. This is usually retrieved through the auto discovery mechanism.

    The setter for this property returns false if the \e {Service Object} is already set to exactly this instance
    or the \e {Service Object} doesn't get accepted by the feature.

    \sa discoveryMode
*/

/*!
    \property QIfAbstractFeatureListModel::serviceObject
    \brief Sets the service object for the feature.

    As features only expose the front API facing the developer, a service object implementing the
    actual function is required. This is usually retrieved through the auto discovery mechanism.

    The setter for this property returns false if the \e {Service Object} is already set to exactly this instance
    or the \e {Service Object} doesn't get accepted by the feature.

    \sa discoveryMode
*/
QIfServiceObject *QIfAbstractFeatureListModel::serviceObject() const
{
    Q_D(const QIfAbstractFeatureListModel);
    return d->m_feature->serviceObject();
}

/*!
    \qmlproperty enumeration AbstractFeatureListModel::discoveryMode
    \brief Holds the mode that is used for the autoDiscovery

    Available values are:
    \value NoAutoDiscovery
           No auto discovery is done and the ServiceObject needs to be set manually.
    \value AutoDiscovery
           Tries to find a production backend with a matching interface and falls back to a simulation backend if not found.
    \value LoadOnlyProductionBackends
           Only tries to load a production backend with a matching interface.
    \value LoadOnlySimulationBackends
           Only tries to load a simulation backend with a matching interface.

    If needed the auto discovery will be started once the Feature creation is completed.

    \note If you change this property after the Feature is instantiated you need to call startAutoDiscovery() to search for
    a new Service Object
*/

/*!
    \property QIfAbstractFeatureListModel::discoveryMode
    \brief Holds the mode that is used for the autoDiscovery

    \note If you change this property after the Feature is instantiated you need to call startAutoDiscovery() to search for
    a new Service Object
*/
QIfAbstractFeature::DiscoveryMode QIfAbstractFeatureListModel::discoveryMode() const
{
    Q_D(const QIfAbstractFeatureListModel);
    return d->m_feature->discoveryMode();
}

/*!
    \qmlproperty enumeration AbstractFeatureListModel::discoveryResult
    \brief The result of the last autoDiscovery attempt

    Available values are:
    \value NoResult
           Indicates that no auto discovery was started because the feature has already assigned a valid ServiceObject.
    \value ErrorWhileLoading
           An error has happened while searching for a backend with a matching interface.
    \value ProductionBackendLoaded
           As a result of the auto discovery a production backend was loaded.
    \value SimulationBackendLoaded
           As a result of the auto discovery a simulation backend was loaded.
*/

/*!
    \property QIfAbstractFeatureListModel::discoveryResult
    \brief The result of the last autoDiscovery attempt

    \sa startAutoDiscovery()
*/
QIfAbstractFeature::DiscoveryResult QIfAbstractFeatureListModel::discoveryResult() const
{
    Q_D(const QIfAbstractFeatureListModel);
    return d->m_feature->discoveryResult();
}

/*!
    \qmlproperty bool AbstractFeatureListModel::isValid
    \brief Indicates whether the feature is ready for use.

    The property is \c true if the feature is ready to be used, otherwise \c false. Not being
    ready usually indicates that no suitable service object could be found, or that automatic
    discovery has not been triggered.

    The backend still might not have sent all properties yet and is not fully initialized.
    Use isInitialized instead to know when the feature holds all correct values.

    \sa QIfServiceObject, discoveryMode, isInitialized
*/
/*!
    \property QIfAbstractFeatureListModel::isValid
    \brief Indicates whether the feature is ready to use.

    The property is \c true if the feature is ready to be used, otherwise \c false. Not being
    ready usually indicates that no suitable service object could be found, or that automatic
    discovery has not been triggered.

    The backend still might not have sent all properties yet and is not fully initialized.
    Use isInitialized instead to know when the feature holds all correct values.

    \sa QIfServiceObject, discoveryMode, isInitialized
*/
bool QIfAbstractFeatureListModel::isValid() const
{
    Q_D(const QIfAbstractFeatureListModel);
    return d->m_feature->isValid();
}

/*!
    \qmlproperty bool AbstractFeatureListModel::isInitialized
    \brief Indicates whether the feature has been initialized with all the values from the backend.

    The property is \c true once the backend sends the QIfFeatureInterface::initializationDone signal
    to indicate that all values have now been initialized with values from the backend.

    \sa isValid, QIfFeatureInterface::initializationDone
*/
/*!
    \property QIfAbstractFeatureListModel::isInitialized
    \brief Indicates whether the feature has been initialized with all the values from the backend.

    The property is \c true once the backend sends the QIfFeatureInterface::initializationDone signal
    to indicate that all values have now been initialized with values from the backend.

    \sa isValid, QIfFeatureInterface::initializationDone
*/
bool QIfAbstractFeatureListModel::isInitialized() const
{
    Q_D(const QIfAbstractFeatureListModel);
    return d->m_feature->isInitialized();
}

/*!
    Returns the last error code.

    \sa QIfAbstractFeature::Error
*/
QIfAbstractFeature::Error QIfAbstractFeatureListModel::error() const
{
    Q_D(const QIfAbstractFeatureListModel);
    return d->m_feature->error();
}

/*!
    \qmlproperty string AbstractFeatureListModel::error

    Last error message of the feature. Empty if no error.
*/
/*!
    \property QIfAbstractFeatureListModel::error

    Last error message of the feature. Empty if no error.
*/
QString QIfAbstractFeatureListModel::errorMessage() const
{
    Q_D(const QIfAbstractFeatureListModel);
    return d->m_feature->errorMessage();
}

bool QIfAbstractFeatureListModel::setServiceObject(QIfServiceObject *so)
{
    Q_D(QIfAbstractFeatureListModel);
    return d->m_feature->setServiceObject(so);
}

void QIfAbstractFeatureListModel::setDiscoveryMode(QIfAbstractFeature::DiscoveryMode discoveryMode)
{
    Q_D(QIfAbstractFeatureListModel);
    d->m_feature->setDiscoveryMode(discoveryMode);
}

/*!
    \qmlmethod enumeration AbstractFeatureListModel::startAutoDiscovery()

    Performs an automatic discovery attempt.

    See AbstractFeature::startAutoDiscovery() for more information
*/

/*!
    \brief Performs an automatic discovery attempt.
*/
QIfAbstractFeature::DiscoveryResult QIfAbstractFeatureListModel::startAutoDiscovery()
{
    Q_D(QIfAbstractFeatureListModel);
    return d->m_feature->startAutoDiscovery();
}

/*!
    \internal
*/
QIfAbstractFeatureListModel::QIfAbstractFeatureListModel(QIfAbstractFeatureListModelPrivate &dd, QObject *parent)
    : QAbstractListModel(dd, parent)
{
    Q_D(QIfAbstractFeatureListModel);
    d->initialize();
    connect(d->m_feature, &QIfAbstractFeature::serviceObjectChanged, this, &QIfAbstractFeatureListModel::serviceObjectChanged);
    connect(d->m_feature, &QIfAbstractFeature::discoveryModeChanged, this, &QIfAbstractFeatureListModel::discoveryModeChanged);
    connect(d->m_feature, &QIfAbstractFeature::discoveryResultChanged, this, &QIfAbstractFeatureListModel::discoveryResultChanged);
    connect(d->m_feature, &QIfAbstractFeature::isValidChanged, this, &QIfAbstractFeatureListModel::isValidChanged);
    connect(d->m_feature, &QIfAbstractFeature::isInitializedChanged, this, &QIfAbstractFeatureListModel::isInitializedChanged);
    connect(d->m_feature, &QIfAbstractFeature::errorChanged, this, &QIfAbstractFeatureListModel::errorChanged);
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
bool QIfAbstractFeatureListModel::acceptServiceObject(QIfServiceObject *serviceObject)
{
    Q_D(QIfAbstractFeatureListModel);
    return d->m_feature->acceptServiceObjectDefaultImpl(serviceObject);
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

    /code
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
    /endcode

    \sa acceptServiceObject(), disconnectFromServiceObject(), clearServiceObject()
*/
void QIfAbstractFeatureListModel::connectToServiceObject(QIfServiceObject *serviceObject)
{
    Q_D(QIfAbstractFeatureListModel);
    d->m_feature->connectToServiceObjectDefaultImpl(serviceObject);
}

/*!
    This method is expected to be implemented by any class subclassing QIfAbstractFeature.

    The implementation should disconnect all connections to the \a serviceObject.

    There is no need to reset internal variables to safe defaults. A call to this function is
    always followed by a call to \l connectToServiceObject or \l clearServiceObject.

    The default implementation disconnects all signals from the serviceObject to this instance.

    \sa acceptServiceObject(), connectToServiceObject(), clearServiceObject()
*/
void QIfAbstractFeatureListModel::disconnectFromServiceObject(QIfServiceObject *serviceObject)
{
    Q_D(QIfAbstractFeatureListModel);
    d->m_feature->disconnectFromServiceObjectDefaultImpl(serviceObject);
}

/*!
    \internal
*/
void QIfAbstractFeatureListModel::classBegin()
{
    Q_D(QIfAbstractFeatureListModel);
    d->m_qmlCreation = true;
}

/*!
    \internal
*/
void QIfAbstractFeatureListModel::componentComplete()
{
    Q_D(QIfAbstractFeatureListModel);
    d->m_qmlCreation = false;
    startAutoDiscovery();
}

/*!
    Returns the interface name this Feature expect to be available from the Service Object and this Feature is implementing.

    See \l {Extending Qt Interface Framework} for more information.
*/
QString QIfAbstractFeatureListModel::interfaceName() const
{
    Q_D(const QIfAbstractFeatureListModel);
    return d->m_feature->interfaceName();
}

/*!
    Returns the current error code converted from QIfAbstractFeature::Error to QString

    \sa error
*/
QString QIfAbstractFeatureListModel::errorText() const
{
    Q_D(const QIfAbstractFeatureListModel);
    return d->m_feature->errorText();
}

/*!
    Sets \a error with the \a message.

    Emits errorChanged() signal.

    \sa QIfAbstractFeature::Error
*/
void QIfAbstractFeatureListModel::setError(QIfAbstractFeature::Error error, const QString &message)
{
    Q_D(QIfAbstractFeatureListModel);
    d->m_feature->setError(error, message);
}

/*!
    Updates \a error and \a message from the backend.

    This slot can be used when implementing a new Feature to report generic errors.
*/
void QIfAbstractFeatureListModel::onErrorChanged(QIfAbstractFeature::Error error, const QString &message)
{
    setError(error, message);
}

QT_END_NAMESPACE
