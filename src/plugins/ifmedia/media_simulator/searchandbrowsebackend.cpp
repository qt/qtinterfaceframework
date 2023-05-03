// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "searchandbrowsebackend.h"
#include "logging.h"

#include <QtConcurrent/QtConcurrent>

#include <QFuture>
#include <QSqlError>
#include <QSqlQuery>
#include <QtDebug>

using namespace Qt::StringLiterals;

static const QString artistLiteral = u"artist"_s;
static const QString albumLiteral = u"album"_s;
static const QString trackLiteral = u"track"_s;

QDataStream &operator<<(QDataStream &stream, const SearchAndBrowseItem &obj)
{
    stream << obj.name();
    stream << obj.type();
    stream << obj.url();
    stream << QVariant(obj.data());
    return stream;
}

QDataStream &operator>>(QDataStream &stream, SearchAndBrowseItem &obj)
{
    QString name;
    QString type;
    QUrl url;
    QVariant data;
    stream >> name;
    stream >> type;
    stream >> url;
    stream >> data;
    obj.setName(name);
    obj.setType(type);
    obj.setUrl(url);
    obj.setData(data.toMap());
    return stream;
}

SearchAndBrowseBackend::SearchAndBrowseBackend(const QVariantMap &serviceSettings, const QSqlDatabase &database, QObject *parent)
    : QIfFilterAndBrowseModelInterface(parent)
    , m_threadPool(new QThreadPool(this))
{
    Q_UNUSED(serviceSettings)

    m_threadPool->setMaxThreadCount(1);

    qRegisterMetaType<SearchAndBrowseItem>();
    qRegisterMetaType<QIfAudioTrackItem>();

    m_db = database;
    m_db.open();

    m_contentTypes << artistLiteral;
    m_contentTypes << albumLiteral;
    m_contentTypes << trackLiteral;
}

QStringList SearchAndBrowseBackend::availableContentTypes() const
{
    return m_contentTypes;
}

void SearchAndBrowseBackend::initialize()
{
    emit availableContentTypesChanged(m_contentTypes);
    emit initializationDone();
}

void SearchAndBrowseBackend::registerInstance(const QUuid &identifier)
{
    m_state.insert(identifier, {});
}

void SearchAndBrowseBackend::unregisterInstance(const QUuid &identifier)
{
    m_state.remove(identifier);
}

void SearchAndBrowseBackend::setContentType(const QUuid &identifier, const QString &contentType)
{
    auto &state = m_state[identifier];
    state.contentType = contentType;

    QStringList types = state.contentType.split('/');
    QString current_type = types.last();
    bool canGoBack = types.count() >= 2;

    if (!m_contentTypes.contains(current_type)) {
        emit errorChanged(QIfAbstractFeature::InvalidOperation, u"The provided content type is not supported"_s);
        return;
    }

    QSet<QString> identifiers;
    if (current_type == artistLiteral || current_type == albumLiteral)
        identifiers = identifiersFromItem<SearchAndBrowseItem>();
    else
        identifiers = identifiersFromItem<QIfAudioTrackItem>();
    emit queryIdentifiersChanged(identifier, identifiers);
    emit canGoBackChanged(identifier, canGoBack);
    emit contentTypeChanged(identifier, contentType);
}

void SearchAndBrowseBackend::setupFilter(const QUuid &identifier, QIfAbstractQueryTerm *term, const QList<QIfOrderTerm> &orderTerms)
{
    auto &state = m_state[identifier];
    state.queryTerm = term;
    state.orderTerms = orderTerms;
}

void SearchAndBrowseBackend::fetchData(const QUuid &identifier, int start, int count)
{
    emit supportedCapabilitiesChanged(identifier, QtInterfaceFrameworkModule::ModelCapabilities(
                                          QtInterfaceFrameworkModule::SupportsFiltering |
                                          QtInterfaceFrameworkModule::SupportsSorting |
                                          QtInterfaceFrameworkModule::SupportsAndConjunction |
                                          QtInterfaceFrameworkModule::SupportsOrConjunction |
                                          QtInterfaceFrameworkModule::SupportsStatelessNavigation |
                                          QtInterfaceFrameworkModule::SupportsGetSize
                                          ));

    if (!m_state.contains(identifier)) {
        qCCritical(media) << "INTERNAL ERROR: No state available for this uuid";
        return;
    }
    auto state = m_state[identifier];

    qCDebug(media) << "FETCH" << identifier << state.contentType << start << count;

    //Determine the current type and which items got selected previously to define the base filter.
    QStringList where_clauses;
    QStringList types = state.contentType.split('/');
    for (const QString &filter_type : types) {
        QStringList parts = filter_type.split('?');
        if (parts.count() != 2)
            continue;

        QString filter = QString::fromUtf8(QByteArray::fromBase64(parts.at(1).toUtf8(), QByteArray::Base64UrlEncoding));
        where_clauses.append(u"%1 = \"%2\""_s.arg(mapIdentifiers(parts.at(0), u"name"_s), filter));
    }
    QString current_type = types.last();

    QString order;
    if (!state.orderTerms.isEmpty())
        order = u"ORDER BY %1"_s.arg(createSortOrder(current_type, state.orderTerms));

    QString columns;
    QString groupBy;
    if (current_type == artistLiteral) {
        columns = u"artistName, coverArtUrl"_s;
        groupBy = u"artistName"_s;
    } else if (current_type == albumLiteral) {
        columns = u"artistName, albumName, coverArtUrl"_s;
        groupBy = u"artistName, albumName"_s;
    } else {
        columns = u"artistName, albumName, trackName, genre, number, file, id, coverArtUrl"_s;
    }

    QString filterClause = createWhereClause(current_type, state.queryTerm);
    if (!filterClause.isEmpty())
        where_clauses.append(filterClause);

    QString whereClause = where_clauses.join(u" AND "_s);

    QString countQuery = u"SELECT count() FROM (SELECT %1 FROM track %2 %3)"_s
            .arg(columns,
                 whereClause.isEmpty() ? QString() : u"WHERE "_s + whereClause,
                 groupBy.isEmpty() ? QString() : u"GROUP BY "_s + groupBy);

    // QtConcurrent::run doesn't allow ignoring the return value
    auto future = QtConcurrent::run(m_threadPool, [this, countQuery, identifier]() {
        QSqlQuery query(m_db);
        if (query.exec(countQuery)) {
            while (query.next()) {
                emit countChanged(identifier, query.value(0).toInt());
            }
        } else {
            sqlError(this, query.lastQuery(), query.lastError().text());
        }
    });

    QString queryString = u"SELECT %1 FROM track %2 %3 %4 LIMIT %5, %6"_s
            .arg(columns,
            whereClause.isEmpty() ? QString() : u"WHERE "_s + whereClause,
            order,
            groupBy.isEmpty() ? QString() : u"GROUP BY "_s + groupBy,
            QString::number(start),
            QString::number(count));

    // QtConcurrent::run doesn't allow ignoring the return value
    future = QtConcurrent::run(m_threadPool,
                      &SearchAndBrowseBackend::search,
                      this,
                      identifier,
                      queryString,
                      current_type,
                      start,
                      count);
}

void SearchAndBrowseBackend::search(const QUuid &identifier, const QString &queryString, const QString &type, int start, int count)
{
    QVariantList list;
    QSqlQuery query(m_db);

    if (query.exec(queryString)) {
        while (query.next()) {
            QString artist = query.value(0).toString();
            QString album = query.value(1).toString();

            if (type == trackLiteral) {
                QIfAudioTrackItem item;
                item.setId(query.value(6).toString());
                item.setTitle(query.value(2).toString());
                item.setArtist(artist);
                item.setAlbum(album);
                item.setUrl(QUrl::fromLocalFile(query.value(5).toString()));
                item.setCoverArtUrl(QUrl::fromLocalFile(query.value(7).toString()));
                list.append(QVariant::fromValue(item));
            } else {
                SearchAndBrowseItem item;
                item.setType(type);
                if (type == artistLiteral) {
                    item.setName(artist);
                    item.setData(QVariantMap{{"coverArtUrl", QUrl::fromLocalFile(query.value(1).toString())}});
                } else if (type == albumLiteral) {
                    item.setName(album);
                    item.setData(QVariantMap{{"artist", artist},
                                             {"coverArtUrl", QUrl::fromLocalFile(query.value(2).toString())}
                                             });
                }
                list.append(QVariant::fromValue(item));
            }

//            if (type == "artist") {
//                DiskArtistItem* artistItem = new DiskArtistItem();
//                artistItem->m_artist = artist;
//                list.append(artistItem);
//            } else if (type == "album") {
//                DiskAlbumItem* albumItem = new DiskAlbumItem();
//                albumItem->m_album = album;
//                albumItem->m_artist = artist;
//                list.append(albumItem);
//            } else if (type == "track") {
//                DiskTrackItem* trackItem = new DiskTrackItem();
//                trackItem->m_artist = artist;
//                trackItem->m_album = album;
//                trackItem->m_track = query.value(2).toString();
//                trackItem->m_genres.append(query.value(3).toString());
//                trackItem->m_number = query.value(4).toUInt();
//                trackItem->m_url = QUrl::fromLocalFile(query.value(5).toString()).toString();
//                list.append(trackItem);
//            }
        }
    } else {
        qCWarning(media) << query.lastError().text();
    }

    emit dataFetched(identifier, list, start, list.count() >= count);

    auto &state = m_state[identifier];
    for (int i=0; i < list.count(); i++) {
        if (start + i >= state.items.count())
            state.items.append(list.at(i));
        else
            state.items.replace(start + i, list.at(i));
    }

    if (type == artistLiteral || type == albumLiteral)
        emit canGoForwardChanged(identifier, QVector<bool>(list.count(), true), start);
}

QString SearchAndBrowseBackend::createSortOrder(const QString &type, const QList<QIfOrderTerm> &orderTerms)
{
    QStringList order;
    int i = 0;
    for (const QIfOrderTerm & term : orderTerms) {
        if (i)
            order.append(u","_s);

        order.append(mapIdentifiers(type, term.propertyName()));
        if (term.isAscending())
            order.append(u"ASC"_s);
        else
            order.append(u"DESC"_s);

        i++;
    }

    return order.join(' ');
}

QString SearchAndBrowseBackend::mapIdentifiers(const QString &type, const QString &identifer)
{
    if (identifer == u"name"_s) {
        if (type == artistLiteral)
            return u"artistName"_s;
        else if (type == albumLiteral)
            return u"albumName"_s;
        else if (type == trackLiteral)
            return u"trackName"_s;
    }

    return identifer;
}

QString SearchAndBrowseBackend::createWhereClause(const QString &type, QIfAbstractQueryTerm *term)
{
    if (!term)
        return QString();

    switch (term->type()) {
    case QIfAbstractQueryTerm::ScopeTerm: {
        auto *scope = static_cast<QIfScopeTerm*>(term);
        return u"%1 (%2)"_s.arg(scope->isNegated() ? u"NOT"_s : QString(), createWhereClause(type, scope->term()));
    }
    case QIfAbstractQueryTerm::ConjunctionTerm: {
        auto *conjunctionTerm = static_cast<QIfConjunctionTerm*>(term);
        QLatin1String conjunction = QLatin1String("AND");
        if (conjunctionTerm->conjunction() == QIfConjunctionTerm::Or)
            conjunction = QLatin1String("OR");

        QString string;
        const auto terms = conjunctionTerm->terms();
        for (QIfAbstractQueryTerm *term : terms) {
            string += createWhereClause(type, term) + QLatin1Char(' ') + conjunction + QLatin1Char(' ');
        }
        if (!string.isEmpty())
            string.chop(2 + conjunction.size()); // chop off trailing " AND " or " OR "
        return string;
    }
    case QIfAbstractQueryTerm::FilterTerm: {
        auto *filter = static_cast<QIfFilterTerm*>(term);
        QString operatorString;
        bool negated = filter->isNegated();
        QString value;
        if (filter->value().typeId() == QMetaType::QString)
            value = u"'%1'"_s.arg(filter->value().toString().replace('*', '%'));
        else
            value = filter->value().toString();

        switch (filter->operatorType()){
            case QIfFilterTerm::Equals: operatorString = u"="_s; break;
            case QIfFilterTerm::EqualsCaseInsensitive: operatorString = u"LIKE"_s; break;
            case QIfFilterTerm::Unequals: operatorString = u"="_s; negated = !negated; break;
            case QIfFilterTerm::GreaterThan: operatorString = u">"_s; break;
            case QIfFilterTerm::GreaterEquals: operatorString = u">="_s; break;
            case QIfFilterTerm::LowerThan: operatorString = u"<"_s; break;
            case QIfFilterTerm::LowerEquals: operatorString = u"<="_s; break;
        }

        QStringList clause;
        if (negated)
            clause.append(u"NOT"_s);
        clause.append(mapIdentifiers(type, filter->propertyName()));
        clause.append(operatorString);
        clause.append(value);

        return clause.join(u" "_s);
    }
    }

    return QString();
}

QIfPendingReply<QString> SearchAndBrowseBackend::goBack(const QUuid &identifier)
{
    auto &state = m_state[identifier];
    QStringList types = state.contentType.split('/');

    if (types.count() < 2)
        return QIfPendingReply<QString>::createFailedReply();

    types.removeLast();
    types.replace(types.count() - 1, types.at(types.count() - 1).split('?').at(0));

    return QIfPendingReply<QString>(types.join('/'));
}

QIfPendingReply<QString> SearchAndBrowseBackend::goForward(const QUuid &identifier, int index)
{
    auto &state = m_state[identifier];

    const QIfStandardItem *i = qtif_gadgetFromVariant<QIfStandardItem>(this, state.items.value(index, QVariant()));
    if (!i)
        return QIfPendingReply<QString>::createFailedReply();

    QString itemId = i->id();
    QStringList types = state.contentType.split('/');

    QString current_type = types.last();
    QString new_type = state.contentType + u"?%1"_s.arg(QLatin1String(itemId.toUtf8().toBase64(QByteArray::Base64UrlEncoding)));

    if (current_type == artistLiteral)
        new_type += QLatin1String("/album");
    else if (current_type == albumLiteral)
        new_type += QLatin1String("/track");
    else
        return QIfPendingReply<QString>::createFailedReply();

    return QIfPendingReply<QString>(new_type);
}

QIfPendingReply<void> SearchAndBrowseBackend::insert(const QUuid &identifier, int index, const QVariant &item)
{
    Q_UNUSED(identifier)
    Q_UNUSED(index)
    Q_UNUSED(item)

    return QIfPendingReply<void>::createFailedReply();
}

QIfPendingReply<void> SearchAndBrowseBackend::remove(const QUuid &identifier, int index)
{
    Q_UNUSED(identifier)
    Q_UNUSED(index)

    return QIfPendingReply<void>::createFailedReply();
}

QIfPendingReply<void> SearchAndBrowseBackend::move(const QUuid &identifier, int currentIndex, int newIndex)
{
    Q_UNUSED(identifier)
    Q_UNUSED(currentIndex)
    Q_UNUSED(newIndex)

    return QIfPendingReply<void>::createFailedReply();
}

QIfPendingReply<int> SearchAndBrowseBackend::indexOf(const QUuid &identifier, const QVariant &item)
{
    Q_UNUSED(identifier)
    Q_UNUSED(item)

    return QIfPendingReply<int>::createFailedReply();
}
