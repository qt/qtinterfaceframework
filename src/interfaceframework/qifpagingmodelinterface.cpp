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

    Every QIfPagingModel generates its own QUuid which is passed to the backend interface and can
    be used to identify a model instance.

    \sa QIfPagingModel

    //TODO explain how the interface works on a example
    <example of a fully featured backend>
*/

/*!
    \fn QIfPagingModelInterface::QIfPagingModelInterface(QObject *parent = nullptr)

    Constructs a backend interface.

    The \a parent is sent to the QObject constructor.
*/
QIfPagingModelInterface::QIfPagingModelInterface(QObject *parent)
    : QIfFeatureInterface(parent)
{}

QIfPagingModelInterface::QIfPagingModelInterface(QObjectPrivate &dd, QObject *parent)
    : QIfFeatureInterface(dd, parent)
{
}

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
