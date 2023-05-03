// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qiffilterandbrowsemodel.h"
#include "qiffilterandbrowsemodel_p.h"

#include "qifqmlconversion_helper.h"
#include "qiffilterandbrowsemodelinterface.h"
#include "queryparser/qifqueryparser_p.h"

#include <QDebug>
#include <QMetaObject>

using namespace Qt::StringLiterals;

QT_BEGIN_NAMESPACE

QIfFilterAndBrowseModelPrivate::QIfFilterAndBrowseModelPrivate(const QString &interface, QIfFilterAndBrowseModel *model)
    : QIfPagingModelPrivate(interface, model)
    , q_ptr(model)
    , m_queryTerm(nullptr)
    , m_canGoBack(false)
{
}

QIfFilterAndBrowseModelPrivate::~QIfFilterAndBrowseModelPrivate()
{
    delete m_queryTerm;
}

void QIfFilterAndBrowseModelPrivate::resetModel()
{
    QIfFilterAndBrowseModelInterface* backend = searchBackend();

    if (backend)
        backend->setContentType(m_identifier, m_contentTypeRequested);
}

void QIfFilterAndBrowseModelPrivate::parseQuery()
{
    if (!searchBackend())
        return;

    if (m_query.isEmpty()) {
        //The new query is empty, tell it to the backend and delete the old term
        setupFilter(nullptr, {});
        return;
    }

    if (!m_capabilities.testFlag(QtInterfaceFrameworkModule::SupportsFiltering) && !m_capabilities.testFlag(QtInterfaceFrameworkModule::SupportsSorting)) {
        qtif_qmlOrCppWarning(q_ptr, u"The backend doesn't support filtering or sorting. Changing the query will have no effect"_s);
        return;
    }

    QIfQueryParser parser;
    parser.setQuery(m_query);
    parser.setAllowedIdentifiers(m_queryIdentifiers);

    QIfAbstractQueryTerm* queryTerm = parser.parse();

    if (!queryTerm) {
        qtif_qmlOrCppWarning(q_ptr, parser.lastError());
        return;
    }
    QList<QIfOrderTerm> orderTerms = parser.orderTerms();

    setupFilter(queryTerm, orderTerms);
}

void QIfFilterAndBrowseModelPrivate::setupFilter(QIfAbstractQueryTerm* queryTerm, const QList<QIfOrderTerm> &orderTerms)
{
    //1. Tell the backend about the new filter (or none)
    QIfFilterAndBrowseModelInterface* backend = searchBackend();
    if (backend)
        backend->setupFilter(m_identifier, queryTerm, orderTerms);

    //2. Now it's safe to delete the old filter
    delete m_queryTerm;
    m_queryTerm = nullptr;

    //3. Save the new filter
    m_queryTerm = queryTerm;
    m_orderTerms = orderTerms;
}

void QIfFilterAndBrowseModelPrivate::clearToDefaults()
{
    QIfPagingModelPrivate::clearToDefaults();

    Q_Q(QIfFilterAndBrowseModel);
    delete m_queryTerm;
    m_queryTerm = nullptr;
    m_query.clear();
    emit q->queryChanged(m_query);
    m_contentType = QString();
    emit q->contentTypeChanged(m_contentType);
    m_contentTypeRequested = QString();
    m_canGoBack = false;
    emit q->canGoBackChanged(m_canGoBack);
    m_availableContentTypes.clear();
    emit q->availableContentTypesChanged(m_availableContentTypes);
    m_canGoForward.clear();

    //Explicitly call the PagingModel resetModel to also reset the fetched data
    QIfPagingModelPrivate::resetModel();
}

void QIfFilterAndBrowseModelPrivate::onCanGoForwardChanged(const QUuid &identifier, const QVector<bool> &indexes, int start)
{
    if (m_identifier != identifier)
        return;

    //Always keep the list size in sync;
    m_canGoForward.resize(qMax(m_itemList.count(), indexes.count()));

    //Update the list
    for (int i = 0; i < indexes.count(); i++)
        m_canGoForward[start + i] = indexes.at(i);
}

void QIfFilterAndBrowseModelPrivate::onCanGoBackChanged(const QUuid &identifier, bool canGoBack)
{
    if (m_identifier != identifier)
        return;

    Q_Q(QIfFilterAndBrowseModel);
    if (m_canGoBack == canGoBack)
        return;

    m_canGoBack = canGoBack;
    emit q->canGoBackChanged(m_canGoBack);
}

void QIfFilterAndBrowseModelPrivate::onContentTypeChanged(const QUuid &identifier, const QString &contentType)
{
    if (m_identifier != identifier)
        return;

    Q_Q(QIfFilterAndBrowseModel);
    // Don't return if the content type is already correct. We still need to continue to update the
    // query and start fetching again
    if (m_contentType != contentType) {
        m_contentType = contentType;
        emit q->contentTypeChanged(m_contentType);
    }
    parseQuery();

    QIfPagingModelPrivate::resetModel();
}

void QIfFilterAndBrowseModelPrivate::onAvailableContentTypesChanged(const QStringList &contentTypes)
{
    Q_Q(QIfFilterAndBrowseModel);
    if (m_availableContentTypes == contentTypes)
        return;

    m_availableContentTypes = contentTypes;
    emit q->availableContentTypesChanged(contentTypes);
}

void QIfFilterAndBrowseModelPrivate::onQueryIdentifiersChanged(const QUuid &identifier, const QSet<QString> &queryIdentifiers)
{
    if (m_identifier != identifier)
        return;

    m_queryIdentifiers = queryIdentifiers;
}

QIfFilterAndBrowseModelInterface *QIfFilterAndBrowseModelPrivate::searchBackend() const
{
    return QIfAbstractFeatureListModelPrivate::backend<QIfFilterAndBrowseModelInterface*>();
}

void QIfFilterAndBrowseModelPrivate::updateContentType(const QString &contentType)
{
    Q_Q(QIfFilterAndBrowseModel);
    m_query = QString();
    m_queryIdentifiers.clear();
    emit q->queryChanged(m_query);
    m_contentTypeRequested = contentType;
    m_canGoForward.clear();
    m_canGoBack = false;

    resetModel();
}

/*!
    \class QIfFilterAndBrowseModel
    \inmodule QtInterfaceFramework
    \brief The QIfFilterAndBrowseModel is a generic model which can be used to search, browse, filter and sort data.

    The QIfFilterAndBrowseModel should be used directly or as a base class whenever a lot of data needs to be
    presented in a ListView.

    The model is built upon the basic principle of filtering and sorting the data already where
    they are created instead of retrieving everything and sort or filter it locally. In addition the QIfFilterAndBrowseModel
    only fetches the data it really needs and can it can be configured how this can be done.

    The backend filling the model with data needs to implement the QIfFilterAndBrowseModelInterface class.

    \section1 Setting it up
    The QIfFilterAndBrowseModel is using QtInterfaceFramework's \l {Dynamic Backend System} and is derived from QIfAbstractFeatureListModel.
    Other than most "QtInterfaceFramework Feature classes", the QIfFilterAndBrowseModel doesn't automatically connect to available backends.

    The easiest approach to set it up, is to connect to the same backend used by another feature. E.g. for connecting to the
    media backend, use the instance from the mediaplayer feature:
    \code
        QIfMediaPlayer *player = new QIfMediaPlayer();
        player->startAutoDiscovery();
        QIfFilterAndBrowseModel *model = new QIfFilterAndBrowseModel();
        model->setServiceObject(player->serviceObject());
    \endcode

    \section2 Content Types

    Once the model is connected to a backend, the contentType needs to be selected. All possible content types can be queried
    from the availableContentTypes property. As the name already suggests, this property selects what type of content
    should be shown in the model. For the mediaplayer example, the available content types could be "track", "album" and "artist".

    \section1 Filtering and Sorting
    \target FilteringAndSorting

    One of the main use case of the QIfFilterAndBrowseModel is to provide a powerful way of filtering and sorting the content
    of the underlying data model. As explained above, the filtering and sorting is supposed to happen where the data is produced.
    To make this work across multiple backends the \l {Qt Interface Framework Query Language} was invented.

    The \l {QIfFilterAndBrowseModel::}{query} property is used to sort the content of the model: e.g. by setting the string "[/name]", the content
    will be sorted by name in ascending order.

    For filtering, the same property is used but without the brackets e.g. "name='Example Item'" for only showing items which
    have the 'name' property set to 'Example Item'.

    Filtering and sorting can also be combined in one string and the filter part can also be more complex. More on that
    can be found in the detailed \l {Qt Interface Framework Query Language} Documentation.

    \section1 Browsing
    \target Browsing

    In addition to filtering and sorting, the QIfFilterAndBrowseModel also supports browsing through a hierarchy of different
    content types. The easiest way to explain this is to look at the existing media example.

    When implementing a library view of all available media files, you might want to provide a way for the user to browse
    through the media database and select a song. You might also want to provide several staring points and from there
    limit the results. E.g.

    \list
        \li Artist -> Album -> Track
        \li Album -> Track
        \li Track
    \endlist

    This can be achieved by defining a complex filter query which takes the previously selected item into account.
    That is the most powerful way of doing it, as the developer/designer can define the browsing order and it can easily
    be changed. The downside of this is that the backend needs to support this way of filtering and sorting as well, which
    is not always be the case. A good example here is a DLNA backend, where the server already defines a fixed  browsing order.

    The QIfFilterAndBrowseModel provides the following methods for browsing:
    \list
        \li canGoForward()
        \li goForward()
        \li canGoBack()
        \li goBack()
    \endlist

    \section2 Navigation Types

    The QIfFilterAndBrowseModel supports two navigation types when browsing through the available data: for most use cases
    the simple InModelNavigation type is sufficient. By using this, the content type of the current model instance changes
    when navigating and the model is reset to show the new data.
    The other navigation type is OutOfModelNavigation and leaves the current model instance as it is. Instead the goForward()
    method returns a new model instance which contains the new data. This is especially useful when several views need to
    be open at the same time. E.g. when used inside a QML StackView.

    \code
        QIfFilterAndBrowseModel *artistModel = new QIfFilterAndBrowseModel();
        model->setContentType("artist");
        //Returns a new instance of QIfFilterAndBrowseModel which contains all albums from the artist at index '0'
        QIfFilterAndBrowseModel *albumModel = artistModel->goForward(0, QIfFilterAndBrowseModel::OutOfModelNavigation);
    \endcode

    \note Please also see the \l{QIfPagingModel}{QIfPagingModel documentation} for how the data loading works and
          the \l{Models} section for more information about all models in QtInterfaceFramework.
*/

/*!
    \enum QIfFilterAndBrowseModel::NavigationType
    \value InModelNavigation
           The new content will be loaded into this model and the existing model data will be reset
    \value OutOfModelNavigation
           A new model will be returned which loads the new content. The model data of this model will
           not be changed and can still be used.
*/

/*!
    \enum QIfFilterAndBrowseModel::Roles
    \value CanGoForwardRole
          True if this item can be used to go one level forward and display the next set of items. See also goForward()
    \omitvalue LastRole

    \sa QIfPagingModel::Roles
*/

/*!
    \qmltype FilterAndBrowseModel
    \instantiates QIfFilterAndBrowseModel
    \inqmlmodule QtInterfaceFramework
    \inherits PagingModel
    \brief The FilterAndBrowseModel is a generic model which can be used to search, browse, filter and sort data.

    The FilterAndBrowseModel should be used directly or as a base class whenever a lot of data needs to be
    presented in a ListView.

    The model is built upon the basic principle of filtering and sorting the data already where
    they are created instead of retrieving everything and sort or filter it locally. In addition the FilterAndBrowseModel
    only fetches the data it really needs and can it can be configured how this can be done.

    All rows in the model need to be subclassed from StandardItem.

    The following roles are available in this model:

    \table
    \header
        \li Role name
        \li Type
        \li Description
    \row
        \li \c name
        \li string
        \li The name of the item. E.g. The name of a contact in a addressbook, or the artist-name in a list of artists.
    \row
        \li \c type
        \li string
        \li The type of the item. E.g. "artist", "track", "contact".
    \row
        \li \c item
        \li object
        \li The item itself. This provides access to the properties which are type specific. E.g. the address of a contact.
    \row
        \li \c canGoForward
        \li bool
        \li True if this item can be used to go one level forward and display the next set of items. \sa goForward()
    \endtable

    \section1 Setting it up
    The FilterAndBrowseModel is using QtInterfaceFramework's \l {Dynamic Backend System} and is derived from QIfAbstractFeatureListModel.
    Other than most "QtInterfaceFramework Feature classes", the FilterAndBrowseModel doesn't automatically connect to available backends.

    The easiest approach to set it up, is to connect to the same backend used by another feature. E.g. for connecting to the
    media backend, use the instance from the mediaplayer feature:
    \qml
        Item {
            MediaPlayer {
                id: player
            }

            FilterAndBrowseModel {
                serviceObject: player.serviceObject
            }
        }
    \endqml

    \section2 Content Types

    Once the model is connected to a backend, the contentType needs to be selected. All possible content types can be queried
    from the availableContentTypes property. As the name already suggests, this property selects what type of content
    should be shown in the model. For the mediaplayer example, the available content types could be "track", "album" and "artist".

    \section1 Filtering and Sorting
    \target FilteringAndSorting

    One of the main use case of the FilterAndBrowseModel is to provide a powerful way of filtering and sorting the content
    of the underlying data model. As explained above, the filtering and sorting is supposed to happen where the data is produced.
    To make this work across multiple backends the \l {Qt Interface Framework Query Language} was invented.

    The \l {FilterAndBrowseModel::}{query} property is used to sort the content of the model: e.g. by setting the string "[/name]", the content
    will be sorted by name in ascending order.

    For filtering, the same property is used but without the brackets e.g. "name='Example Item'" for only showing items which
    have the 'name' property set to 'Example Item'.

    Filtering and sorting can also be combined in one string and the filter part can also be more complex. More on that
    can be found in the detailed \l {Qt Interface Framework Query Language} Documentation.

    \section1 Browsing
    \target Browsing

    In addition to filtering and sorting, the FilterAndBrowseModel also supports browsing through a hierarchy of different
    content types. The easiest way to explain this is to look at the existing media example.

    When implementing a library view of all available media files, you might want to provide a way for the user to browse
    through the media database and select a song. You might also want to provide several staring points and from there
    limit the results. E.g.

    \list
        \li Artist -> Album -> Track
        \li Album -> Track
        \li Track
    \endlist

    This can be achieved by defining a complex filter query which takes the previously selected item into account.
    That is the most powerful way of doing it, as the developer/designer can define the browsing order and it can easily
    be changed. The downside of this is that the backend needs to support this way of filtering and sorting as well, which
    is not always be the case. A good example here is a DLNA backend, where the server already defines a fixed  browsing order.

    The FilterAndBrowseModel provides the following methods/properties for browsing:
    \list
        \li canGoForward()
        \li goForward()
        \li canGoBack
        \li goBack()
    \endlist

    \section2 Navigation Types

    The FilterAndBrowseModel supports two navigation types when browsing through the available data: for most use cases
    the simple InModelNavigation type is sufficient. By using this, the content type of the current model instance changes
    when navigating and the model is reset to show the new data.
    The other navigation type is OutOfModelNavigation and leaves the current model instance as it is. Instead the goForward()
    method returns a new model instance which contains the new data. This is especially useful when several views need to
    be open at the same time. E.g. when used inside a QML StackView.

    \qml
        StackView {
            id: stack
            initialItem: view

            Component {
                id: view
                ListView {
                    model: FilterAndBrowseModel {
                        contentType: "artist"
                    }
                    delegate: MouseArea {
                        onClicked: {
                            stack.push({ "item" : view,
                                        "properties:" {
                                            "model" : model->goForward(index, FilterAndBrowseModel.OutOfModelNavigation)
                                        }});
                        }
                    }
                }
            }
        }
    \endqml

    \note Please also see the \l{PagingModel}{PagingModel documentation} for how the data loading works and
          the \l{Models} section for more information about all models in QtInterfaceFramework.
*/

/*!
    Constructs a QIfFilterAndBrowseModel.

    The \a parent argument is passed on to the \l QIfAbstractFeatureListModel base class.
*/
QIfFilterAndBrowseModel::QIfFilterAndBrowseModel(QObject *parent)
    : QIfPagingModel(*new QIfFilterAndBrowseModelPrivate(QStringLiteral(QIfFilterAndBrowseModel_iid), this), parent)
{
}

/*!
    \qmlproperty string FilterAndBrowseModel::query
    \brief Holds the current query used for filtering and sorting the current content of the model.

    \note When changing this property the content will be reset.

    See \l {Qt Interface Framework Query Language} for more information.
    \sa FilteringAndSorting
*/

/*!
    \property QIfFilterAndBrowseModel::query
    \brief Holds the current query used for filtering and sorting the current content of the model.

    \note When changing this property the content will be reset.

    See \l {Qt Interface Framework Query Language} for more information.
    \sa FilteringAndSorting
*/
QString QIfFilterAndBrowseModel::query() const
{
    Q_D(const QIfFilterAndBrowseModel);
    return d->m_query;
}

void QIfFilterAndBrowseModel::setQuery(const QString &query)
{
    Q_D(QIfFilterAndBrowseModel);
    if (d->m_query == query)
        return;

    //TODO If we use the stateless navigation this needs to be prevented on the second+ model

    d->m_query = query;
    emit queryChanged(d->m_query);

    //The query is checked in resetModel
    d->resetModel();
}

/*!
    \qmlproperty string FilterAndBrowseModel::contentType
    \brief Holds the current type of content displayed in this model.

    \note When changing this property the content will be reset.

    \sa FilterAndBrowseModel::availableContentTypes
*/

/*!
    \property QIfFilterAndBrowseModel::contentType
    \brief Holds the current type of content displayed in this model.

    \note When changing this property the content will be reset.

    \sa availableContentTypes
*/
QString QIfFilterAndBrowseModel::contentType() const
{
    Q_D(const QIfFilterAndBrowseModel);
    return d->m_contentType;
}

void QIfFilterAndBrowseModel::setContentType(const QString &contentType)
{
    Q_D(QIfFilterAndBrowseModel);
    if (d->m_contentTypeRequested == contentType)
        return;

    d->updateContentType(contentType);
}

/*!
    \qmlproperty list<string> FilterAndBrowseModel::availableContentTypes
    \brief Holds all the available content types

    \sa contentType
*/

/*!
    \property QIfFilterAndBrowseModel::availableContentTypes
    \brief Holds all the available content types

    \sa contentType
*/
QStringList QIfFilterAndBrowseModel::availableContentTypes() const
{
    Q_D(const QIfFilterAndBrowseModel);
    return d->m_availableContentTypes;
}

/*!
    \qmlproperty bool FilterAndBrowseModel::canGoBack
    \brief Holds whether the goBack() function can be used to return to the previous content.

    See \l Browsing for more information.
*/

/*!
    \property QIfFilterAndBrowseModel::canGoBack
    \brief Holds whether the goBack() function can be used to return to the previous content.

    See \l Browsing for more information.
*/
bool QIfFilterAndBrowseModel::canGoBack() const
{
    Q_D(const QIfFilterAndBrowseModel);
    return d->m_canGoBack;
}

/*!
    \reimp
*/
QVariant QIfFilterAndBrowseModel::data(const QModelIndex &index, int role) const
{
    Q_D(const QIfFilterAndBrowseModel);
    Q_UNUSED(role)
    if (!index.isValid())
        return QVariant();

    int row = index.row();

    if (row >= d->m_itemList.count() || row < 0)
        return QVariant();

    if (role == CanGoForwardRole)
        return canGoForward(row);
    else
        return QIfPagingModel::data(index, role);
}

/*!
    \qmlmethod void FilterAndBrowseModel::goBack()
    Goes one level back in the navigation history.

    See also \l Browsing for more information.
*/
/*!
    Goes one level back in the navigation history.

    See also \l Browsing for more information.
*/
void QIfFilterAndBrowseModel::goBack()
{
    Q_D(QIfFilterAndBrowseModel);
    QIfFilterAndBrowseModelInterface *backend = d->searchBackend();

    if (!backend) {
        qtif_qmlOrCppWarning(this, "No backend connected");
        return;
    }

    if (!d->m_canGoBack) {
        qtif_qmlOrCppWarning(this, "Can't go backward anymore");
        return;
    }

    QIfPendingReply<QString> reply = backend->goBack(d->m_identifier);
    reply.then([this, reply](const QString &value) {
        Q_D(QIfFilterAndBrowseModel);
        d->updateContentType(value);
    },
    [this]() {
        qtif_qmlOrCppWarning(this, "Going backward failed");
    });
}

/*!
    \qmlmethod bool FilterAndBrowseModel::canGoForward(i)
    Returns true when the item at index \a i can be used to show the next set of elements.

    See also \l Browsing for more information.
*/
/*!
    Returns true when the item at index \a i can be used to show the next set of elements.

    See also \l Browsing for more information.
*/
bool QIfFilterAndBrowseModel::canGoForward(int i) const
{
    Q_D(const QIfFilterAndBrowseModel);
    QIfFilterAndBrowseModelInterface *backend = d->searchBackend();

    if (i >= d->m_canGoForward.count() || i < 0)
        return false;

    if (!backend) {
        qtif_qmlOrCppWarning(this, "No backend connected");
        return false;
    }

    return d->m_canGoForward[i];
}

/*!
    \qmlmethod FilterAndBrowseModel FilterAndBrowseModel::goForward(i, navigationType)
    Uses the item at index \a i and shows the next set of items.

    \a navigationType can be one of the following values:
    \target InModelNavigation
    \value InModelNavigation
           The new content will be loaded into this model and the existing model data will be reset
    \target OutOfModelNavigation
    \value OutOfModelNavigation
           A new model will be returned which loads the new content. The model data of this model will
           not be changed and can still be used.

    \note Whether the OutOfModelNavigation navigation type is supported is decided by the backend.

    See also \l Browsing for more information.
*/
/*!
    Returns true when the item at index \a i can be used to show the next set of elements.

    Uses the item at index \a i and shows the next set of items. The \a navigationType can be used
    to control whether the new data should be shown in this model instance or whether a new instance
    should be created and returned. If a instance is returned, this instance is owned
    by the caller.

    \note Whether the OutOfModelNavigation navigation type is supported is decided by the backend.

    See also \l Browsing for more information.
*/
QIfFilterAndBrowseModel *QIfFilterAndBrowseModel::goForward(int i, NavigationType navigationType)
{
    Q_D(QIfFilterAndBrowseModel);
    QIfFilterAndBrowseModelInterface *backend = d->searchBackend();

    if (i >= d->m_itemList.count() || i < 0)
        return nullptr;

    if (!backend) {
        qtif_qmlOrCppWarning(this, "No backend connected");
        return nullptr;
    }

    if (!d->m_canGoForward.value(i, false)) {
        qtif_qmlOrCppWarning(this, "Can't go forward anymore");
        return nullptr;
    }

    if (navigationType == OutOfModelNavigation) {
        if (d->m_capabilities.testFlag(QtInterfaceFrameworkModule::SupportsStatelessNavigation)) {
            QIfPendingReply<QString> reply = backend->goForward(d->m_identifier, i);
            auto newModel = new QIfFilterAndBrowseModel(serviceObject());
            reply.then([reply, newModel](const QString &value) {
                newModel->setContentType(value);
            },
            [this]() {
                qtif_qmlOrCppWarning(this, "Going forward failed");
            });
            return newModel;

        } else {
            qtif_qmlOrCppWarning(this, "The backend doesn't support the OutOfModelNavigation");
            return nullptr;
        }
    } else {
        QIfPendingReply<QString> reply = backend->goForward(d->m_identifier, i);
        reply.then([this, reply](const QString &value) {
            Q_D(QIfFilterAndBrowseModel);
            d->updateContentType(value);
        },
        [this]() {
            qtif_qmlOrCppWarning(this, "Going forward failed");
        });
    }

    return nullptr;
}

/*!
    \qmlmethod FilterAndBrowseModel::insert(int index, StandardItem item)

    Insert the \a item at the position \a index.

    If the backend doesn't accept the provided item, this operation will end in a no op.

    The returned PendingReply notifies about when the action has been done or whether it failed.
*/

/*!
    \fn void QIfFilterAndBrowseModel::insert(int index, const QVariant &variant)

    Insert the \a variant at the position \a index.

    If the backend doesn't accept the provided item, this operation will end in a no op.

    The returned QIfPendingReply notifies about when the action has been done or whether it failed.
*/
QIfPendingReply<void> QIfFilterAndBrowseModel::insert(int index, const QVariant &variant)
{
    Q_D(QIfFilterAndBrowseModel);
    const auto item = qtif_gadgetFromVariant<QIfStandardItem>(this, variant);
    if (!item)
        return QIfPendingReply<void>::createFailedReply();

    QIfFilterAndBrowseModelInterface *backend = d->searchBackend();
    if (!backend) {
        qtif_qmlOrCppWarning(this, "Can't insert items without a connected backend");
        return QIfPendingReply<void>::createFailedReply();
    }

    if (!d->m_capabilities.testFlag(QtInterfaceFrameworkModule::SupportsInsert)) {
        qtif_qmlOrCppWarning(this, "The backend doesn't support inserting items");
        return QIfPendingReply<void>::createFailedReply();
    }

    return backend->insert(d->m_identifier, index, variant);
}

/*!
    \qmlmethod FilterAndBrowseModel::remove(int index)

    Removes the item at position \a index.

    The returned PendingReply notifies about when the action has been done or whether it failed.
*/

/*!
    \fn void QIfFilterAndBrowseModel::remove(int index)

    Removes the item at position \a index.

    The returned QIfPendingReply notifies about when the action has been done or whether it failed.
*/
QIfPendingReply<void> QIfFilterAndBrowseModel::remove(int index)
{
    Q_D(QIfFilterAndBrowseModel);
    QIfFilterAndBrowseModelInterface *backend = d->searchBackend();
    if (!backend) {
        qtif_qmlOrCppWarning(this, "Can't remove items without a connected backend");
        return QIfPendingReply<void>::createFailedReply();
    }

    if (!d->m_capabilities.testFlag(QtInterfaceFrameworkModule::SupportsRemove)) {
        qtif_qmlOrCppWarning(this, "The backend doesn't support removing of items");
        return QIfPendingReply<void>::createFailedReply();
    }

    return backend->remove(d->m_identifier, index);
}

/*!
    \qmlmethod FilterAndBrowseModel::move(int cur_index, int new_index)

    Moves the item at position \a cur_index to the new position \a new_index.

    The returned PendingReply notifies about when the action has been done or whether it failed.
*/

/*!
    \fn void QIfFilterAndBrowseModel::move(int cur_index, int new_index)

    Moves the item at position \a cur_index to the new position \a new_index.

    The returned QIfPendingReply notifies about when the action has been done or whether it failed.
*/
QIfPendingReply<void> QIfFilterAndBrowseModel::move(int cur_index, int new_index)
{
    Q_D(QIfFilterAndBrowseModel);
    QIfFilterAndBrowseModelInterface *backend = d->searchBackend();
    if (!backend) {
        qtif_qmlOrCppWarning(this, "Can't move items without a connected backend");
        return QIfPendingReply<void>::createFailedReply();
    }

    if (!d->m_capabilities.testFlag(QtInterfaceFrameworkModule::SupportsMove)) {
        qtif_qmlOrCppWarning(this, "The backend doesn't support moving of items");
        return QIfPendingReply<void>::createFailedReply();
    }

    return backend->move(d->m_identifier, cur_index, new_index);
}

/*!
    \qmlmethod FilterAndBrowseModel::indexOf(StandardItem item)

    Determines the index of \a item in this model.

    The result is returned as a PendingReply.
*/

/*!
    \fn void QIfFilterAndBrowseModel::indexOf(const QVariant &variant)

    Determines the index of \a variant in this model.

    The result is returned as a QIfPendingReply.
*/
QIfPendingReply<int> QIfFilterAndBrowseModel::indexOf(const QVariant &variant)
{
    Q_D(QIfFilterAndBrowseModel);
    const auto *item = qtif_gadgetFromVariant<QIfStandardItem>(this, variant);
    if (!item)
        return QIfPendingReply<int>::createFailedReply();

    QIfFilterAndBrowseModelInterface *backend = d->searchBackend();
    if (!backend) {
        qtif_qmlOrCppWarning(this, "Can't get the index without a connected backend");
        return QIfPendingReply<int>::createFailedReply();
    }

    return backend->indexOf(d->m_identifier, variant);
}

/*!
    \reimp
*/
QHash<int, QByteArray> QIfFilterAndBrowseModel::roleNames() const
{
    static QHash<int, QByteArray> roles;
    if (roles.isEmpty()) {
        roles =  QIfPagingModel::roleNames();
        roles[CanGoForwardRole] = "canGoForward";
    }
    return roles;
}

/*!
    \internal
*/
QIfFilterAndBrowseModel::QIfFilterAndBrowseModel(QIfServiceObject *serviceObject, QObject *parent)
    : QIfFilterAndBrowseModel(parent)
{
    setServiceObject(serviceObject);
}

/*!
    \internal
*/
QIfFilterAndBrowseModel::QIfFilterAndBrowseModel(QIfFilterAndBrowseModelPrivate &dd, QObject *parent)
    : QIfPagingModel(dd, parent)
{
}

/*!
    \reimp
*/
void QIfFilterAndBrowseModel::connectToServiceObject(QIfServiceObject *serviceObject)
{
    Q_D(QIfFilterAndBrowseModel);

    QIfFilterAndBrowseModelInterface *backend = d->searchBackend();
    if (!backend)
        return;

    QObjectPrivate::connect(backend, &QIfFilterAndBrowseModelInterface::availableContentTypesChanged,
                            d, &QIfFilterAndBrowseModelPrivate::onAvailableContentTypesChanged);
    QObjectPrivate::connect(backend, &QIfFilterAndBrowseModelInterface::contentTypeChanged,
                            d, &QIfFilterAndBrowseModelPrivate::onContentTypeChanged);
    QObjectPrivate::connect(backend, &QIfFilterAndBrowseModelInterface::queryIdentifiersChanged,
                            d, &QIfFilterAndBrowseModelPrivate::onQueryIdentifiersChanged);
    QObjectPrivate::connect(backend, &QIfFilterAndBrowseModelInterface::canGoBackChanged,
                            d, &QIfFilterAndBrowseModelPrivate::onCanGoBackChanged);
    QObjectPrivate::connect(backend, &QIfFilterAndBrowseModelInterface::canGoForwardChanged,
                            d, &QIfFilterAndBrowseModelPrivate::onCanGoForwardChanged);

    QIfPagingModel::connectToServiceObject(serviceObject);

    //once the initialization is done QIfPagingModel will reset the model
}

/*!
    \reimp
*/
void QIfFilterAndBrowseModel::clearServiceObject()
{
    Q_D(QIfFilterAndBrowseModel);
    d->clearToDefaults();
}

QT_END_NAMESPACE

#include "moc_qiffilterandbrowsemodel.cpp"
