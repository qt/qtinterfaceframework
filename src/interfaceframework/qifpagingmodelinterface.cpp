// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qifpagingmodelinterface.h"

QT_BEGIN_NAMESPACE

/*!
    \class QIfPagingModelInterface
    \inmodule QtInterfaceFramework
    \ingroup backends
    \inherits QObject
    \keyword org.qt-project.interfaceframework.PagingModel/1.0
    \brief The QIfPagingModelInterface defines the interface for backends to the
    QIfPagingModel feature class.

    The QIfPagingModelInterface is the interface used by \l QIfPagingModel

    The interface is discovered by a \l QIfPagingModel object, which connects to it and sets it up.

    \section1 QIfPagingModel registration

    Every QIfPagingModel generates its own QUuid which is passed to the backend interface and can
    be used to identify a model instance. By using the QUuid of a specific instance in a signal,
    only this specific instance gets notified about the change. This can be useful for stateful
    backends, such as an addressbook with a filter function. Because of the filtering, the number
    of items inside the model will change. The notification and the filtering itself is only done
    for the specific instance which requested it. All other instances could still display the full
    set of model data. The QIfFilterAndBrowseModel is using this concept for filtering. To inform
    all feature instances of a change, a default constructed QUuid can be used instead.

    The \l registerInstance and \l unregisterInstance functions are called respectively when a new
    QIfPagingModel instance connects to this backend or disconnects from it.

    \section1 Capabilities

    Depending on the data of the model and the underlying store the data is coming from, different
    features might be supported. The \l supportedCapabilitiesChanged signal informs the
    QIfPagingModel instance about the features supported by this backend. The \l
    {ModelCapabilities} type used in this signal hosts different features supported in the
    QIfPagingModel and the QIfFilterAndBrowseModel.

    For a QIfPagingModel, the \l {ModelCapabilities}{SupportsGetSize}
    capability can be used to indicate that this backend knows the total size of the model and that
    it will inform about it using the \l countChanged() signal. In case the capabilities are not
    reported, or the SupportsGetSize capability is not set, the QIfPagingModel needs to use the
    \l{QIfPagingModel::}{FetchMore} loading type.

    \section1 Returning model data

    Other than QIfFeatureInterface based backends, the \l initialize() function does not emit the
    complete state. Instead the actual model data is returned on request. This is done to support
    big models as well and save bandwidth when the data needs to be transferred over an IPC. Once a
    QIfPagingModel is used inside a View class, the model data is requested from the backend by
    calling \l fetchData(). The amount of data requested in one call can be controlled by the
    QIfPagingModel instance. Once the requested data is ready, the backend needs to inform about it
    using the
    \l dataFetched() signal.
    To inform the QIfPagingModel about an updated row, the dataChanged() signal can be used.

    \section1 Example call order

    \list 1
        \li A QIfPagingModel instance is created and connects to this backend
        \li \l initialize() is called
        \li The backend initializes it's internal state and informs about all extra properties it
            has by emitting the respective change signals.
        \li The backend emits \l initializationDone()
        \li The QIfPagingModel instance calls \l registerInstance() with its own QUuid
        \li The backend informs about its capabilities and its size by emitting
            \l supportedCapabilitiesChanged() and \l countChanged().
        \li The QIfPagingModel fetches the first rows by calling \l fetchData()
        \li The backend emits dataFetched() once it is ready.
        \li The last two steps might repeat depending on the current state of the view
    \endlist

    \sa QIfPagingModel
*/

/*!
    \fn QIfPagingModelInterface::QIfPagingModelInterface(QObject *parent = nullptr)

    Constructs a backend interface.

    The \a parent is sent to the QObject constructor.
*/
QIfPagingModelInterface::QIfPagingModelInterface(QObject *parent)
    : QIfFeatureInterface(parent)
{}

/*!
    \fn void QIfPagingModelInterface::registerInstance(const QUuid &identifier)

    Registers the instance of QIfPagingModel identified by \a identifier with this backend. This
    function will be called by QIfPagingModel and all its derived classes after the initialize()
    function, but before any data will be requested e.g. via fetchData().

    \sa unregisterInstance
*/

/*!
    \fn void QIfPagingModelInterface::unregisterInstance(const QUuid &identifier)

    Unregisters the instance of QIfPagingModel identified by \a identifier with this backend. This
    function will be called by QIfPagingModel and all its derived classes before the
    ServiceObject gets disconnected and can be used to cleanup all cached data for this \a
    identifier.

    \sa registerInstance
*/

/*!
    \fn void QIfPagingModelInterface::fetchData(const QUuid &identifier, int start, int count)

    This function is called whenever new data needs to be retrieved by a QIfPagingModel identified by \a identifier.

    The parameters \a start and \a count define the range of data which should be fetched. This method is expected to emit the dataFetched() signal once
    the new data is ready.

    \sa dataFetched()
*/

/*!
    \fn void QIfPagingModelInterface::supportedCapabilitiesChanged(const QUuid &identifier, QtInterfaceFrameworkModule::ModelCapabilities capabilities)

    Emitted when the \a capabilities of the model instance identified by \a identifier changed.

    \note If a null QQuuid is used as a identifier, all model instances will be informed.
*/

/*!
    \fn void QIfPagingModelInterface::countChanged(const QUuid &identifier, int count)

    This signal is emitted when the current number of items in the QIfPagingModel instance identified by \a identifier changed.
    The new number of items is returned as \a count.

    This signal is expected to be emitted after the model instance has requested new data for the first time by calling fetchData() and
    should be emitted before the data is returned by emitting the dataFetched() signal.

    \note If a null QQuuid is used as a identifier, all model instances will be informed.

    \sa fetchData() dataFetched()
*/

/*!
    \fn void QIfPagingModelInterface::dataFetched(const QUuid &identifier, const QList<QVariant> &data, int start, bool moreAvailable)

    This signal is emitted as a result of a call to fetchData() and returns the requested data in the argument \a data to the QIfPagingModel instance identified by \a identifier.
    The arguments \a start holds the index where the data starts and \a moreAvailable holds whether there is more data available and a new fetchData() call can be used to retrieve this data.

    \note If a null QQuuid is used as a identifier, all model instances will be informed.

    \sa fetchData() dataFetched()
*/

/*!
    \fn void QIfPagingModelInterface::dataChanged(const QUuid &identifier, const QList<QVariant> &data, int start, int count)

    This signal is emitted whenever the data in the QIfPagingModel instance identified by \a identifier changed and the model needs to be updated.
    The new data is passed as \a data. The arguments \a start and \a count can be used to define the set of items which should be replaced with the new data.

    For inserting a new item, the item is passed in \a data and \a start is used for where the item should be inserted, the \a count argument needs to be 0 as we don't want to replace existing data:

    \code
    QList<ExampleItem> list;
    ExampleItem item = ExampleItem();
    list.insert(index, item);
    QVariantList items = { QVariant::fromValue(item) };
    emit dataChanged(items, index, 0);
    \endcode

    Removing an item is very similar, \a start is used to indicate which item and \a count to indicate how much:

    \code
    list.removeAt(index);
    emit dataChanged(identifier, QVariantList(), index, 1);
    \endcode

    \note If a null QQuuid is used as a identifier, all model instances will be informed.
*/

QT_END_NAMESPACE

#include "moc_qifpagingmodelinterface.cpp"
