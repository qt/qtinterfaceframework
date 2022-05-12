/****************************************************************************
**
** Copyright (C) 2022 The Qt Company Ltd.
** Copyright (C) 2019 Luxoft Sweden AB
** Copyright (C) 2018 Pelagicore AG
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtInterfaceFramework module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:COMM$
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** $QT_END_LICENSE$
**
**
**
**
**
**
**
**
**
**
**
**
**
**
**
**
**
**
**
******************************************************************************/

#include "qiffilterandbrowsemodelinterface.h"

QT_BEGIN_NAMESPACE

/*!
    \class QIfFilterAndBrowseModelInterface
    \inmodule QtInterfaceFramework
    \ingroup backends
    \inherits QObject
    \keyword org.qt-project.interfaceframework.FilterAndBrowseModel/1.0
    \brief The QIfFilterAndBrowseModelInterface defines the interface for backends to the
    QIfFilterAndBrowseModel feature class.

    The QIfFilterAndBrowseModelInterface is the interface used by \l QIfFilterAndBrowseModel

    The interface is discovered by a \l QIfFilterAndBrowseModel object, which connects to it and sets it up.

    Every QIfFilterAndBrowseModel generates its own QUuid which is passed to the backend interface and can
    be used to identify a model instance.

    \sa QIfFilterAndBrowseModel

    //TODO explain how the interface works on a example
    <example of a fully featured backend>
*/

QIfFilterAndBrowseModelInterface::QIfFilterAndBrowseModelInterface(QObject *parent)
    : QIfPagingModelInterface(parent)
{}

/*!
    \fn template <class T> QIfFilterAndBrowseModelInterface::identifiersFromItem()

    Returns all properties of type T.

    These can registered as identifiers for the \l {Qt Interface Framework Query Language} using the
    queryIdentifiersChanged() signal.
*/
/*!
    \internal
*/
QSet<QString> QIfFilterAndBrowseModelInterface::identifiersFromItem(const QMetaObject &object)
{
    QSet<QString> identifiers;
    for (int i=0; i < object.propertyCount(); i++) {
        QLatin1String propName(object.property(i).name());
        if (propName != QLatin1String("objectName"))
            identifiers.insert(propName);
    }

    return identifiers;
}

/*!
    \fn void QIfFilterAndBrowseModelInterface::setContentType(const QUuid &identifier, const QString &contentType)

    Sets the \a contentType of the QIfFilterAndBrowseModel instance identified by \a identifier.
    The given contenType can contain additional path information. The encoding is defined by the
    goForward() method. In case the \a contentType is not valid the error() signal should be used.

    \note The QIfFilterAndBrowseModel doesn't check the validity of the contentType, this is the backend's
    responsibility.

    If the QIfFilterAndBrowseModel supports filtering (see QIfPagingModel::capabilitiesChanged),
    the backend needs to emit the queryIdentifiersChanged signal once the contentType is set.

    Finally, the contentTypeChanged signal needs to be emitted, when the backend has set the contentType
    and it's ready for use.

    Calls to this function are followed by calls to setupFilter() and fetchData().

    \sa identifiersFromItem queryIdentifiersChanged contentTypeChanged
*/

/*!
    \fn void QIfFilterAndBrowseModelInterface::setupFilter(const QUuid &identifier, QIfAbstractQueryTerm *term, const QList<QIfOrderTerm> &orderTerms)

    Setup the filter for the QIfFilterAndBrowseModel instance identified by \a identifier.

    The \a term and \a orderTerms arguments are representations of the query which is used for
    filtering and sorting. The \a term argument is a null-pointer when the backend doesn't support
    filtering and sorting or when no query was defined in the QIfFilterAndBrowseModel instance.
*/

/*!
    \fn QIfPendingReply<QString> QIfFilterAndBrowseModelInterface::goBack(const QUuid &identifier)

    Requests to go back to the previous displayed data set of the QIfFilterAndBrowseModel instance identified by \a identifier.

    The new content type is returned in the form of a QIfPendingReply. Once ready the new content type
    must be set using setSuccess(), or using setFailed() if there's an error.

    See \l Browsing for more information on how this is used.
    \sa canGoBackChanged()
*/

/*!
    \fn QIfPendingReply<QString> QIfFilterAndBrowseModelInterface::goForward(const QUuid &identifier, int index)

    Requests to go to the next data set of the QIfFilterAndBrowseModel instance identified by \a identifier at \a index.

    The new content type is returned in the form of a QIfPendingReply. Once ready the new content type
    must be set using setSuccess(), or using setFailed() if there's an error.

    See \l Browsing for more information on how this is used.
    \sa canGoForwardChanged()
*/

/*!
    \fn QIfFilterAndBrowseModelInterface::insert(const QUuid &identifier, int index, const QVariant &item)

    Adds the browsable \a item to the current dataset of the QIfFilterAndBrowseModel instance identified by \a identifier at \a index.

    The provided item could be owned by another model or QML, because of that it's expected that the backend stores its internal representation.

    \sa dataChanged()
*/

/*!
    \fn QIfFilterAndBrowseModelInterface::remove(const QUuid &identifier, int index)

    Removes the browsable item at position \a index from the current dataset of the QIfFilterAndBrowseModel instance identified by \a identifier.

    \sa dataChanged()
*/

/*!
    \fn QIfFilterAndBrowseModelInterface::move(const QUuid &identifier, int currentIndex, int newIndex)

    Moves the browsable item at position \a currentIndex of the current dataset of the QIfFilterAndBrowseModel instance identified by \a identifier to the new position \a newIndex.

    \sa dataChanged()
*/

/*!
    \fn QIfFilterAndBrowseModelInterface::indexOf(const QUuid &identifier, const QVariant &item)

    Determines the index of \a item in the model identified by \a identifier.
*/

/*!
    \fn QIfFilterAndBrowseModelInterface::canGoBackChanged(const QUuid &identifier, bool canGoBack)

    Emitted to inform the QIfFilterAndBrowseModel instance, identified by \a identifier, whether it \a canGoBack to the data set previously
    shown. If the instance can display the previous data set, \a canGoBack is set to \c true.

    See \l Browsing for more information on how this is used.
    \sa goBack
*/


/*!
    \fn QIfFilterAndBrowseModelInterface::canGoForwardChanged(const QUuid &identifier, const QVector<bool> &indexes, int start)

    Emitted to inform the QIfFilterAndBrowseModel instance identified by \a identifier that the following
    \a indexes can be used to show a new set of data.

    The \a start parameter can be used to inform only about a limited set of indexes. This signal
    can be emitted during a QIfPagingModelInterface::fetchData() call to inform about the state
    of the just fetched data.

    See \l Browsing for more information on how this is used.
    \sa goForward
*/

/*!
    \fn QIfFilterAndBrowseModelInterface::contentTypeChanged(const QUuid &identifier, const QString &contentType)

    Emitted as a result of a call to setContentType, to inform the QIfFilterAndBrowseModel instance identified by \a identifier
    about it's new \a contentType.

    \sa setContentType
*/

/*!
    \fn QIfFilterAndBrowseModelInterface:availableContentTypesChanged(const QStringList &availableContentTypes)

    Emitted during the initialization phase, to inform about all available content types(\a availableContentTypes).

    The value is provided to the user as indication of which content types can be used.

    \note The QIfFilterAndBrowseModel doesn't check the validity of the contentType, this is the responsibility
    of the backend.

    \sa setContentType
*/

/*!
    \fn QIfFilterAndBrowseModelInterface:queryIdentifiersChanged(const QUuid &identifier, const QSet<QString> &queryIdentifiers)

    Emitted as a result of a call to setContentType, to inform the QIfFilterAndBrowseModel instance identified by \a identifier
    about the currently supported \a queryIdentifiers.

    The \a queryIdentifiers are used to setup the \l {Qt Interface Framework Query Language} to be able to show
    meaningful errors for invalid queries. Not emitting this signal, will cause the \l {Qt Interface Framework Query Language} to not limit the
    possible identifiers.
*/

QT_END_NAMESPACE
