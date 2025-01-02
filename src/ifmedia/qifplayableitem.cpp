// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qifplayableitem.h"
#include <QDataStream>
#include <QDebug>

QT_BEGIN_NAMESPACE

class QIfPlayableItemPrivate : public QSharedData
{
public:
    QIfPlayableItemPrivate()
    {}

    QIfPlayableItemPrivate(const QIfPlayableItemPrivate &other)
        : QSharedData(other)
        , m_url(other.m_url)
    {}

    QUrl m_url;
};

class QIfAudioTrackItemPrivate : public QSharedData
{
public:
    QIfAudioTrackItemPrivate()
        : m_year(-1)
        , m_trackNumber(-1)
        , m_duration(-1)
        , m_rating(-1)
    {}

    QIfAudioTrackItemPrivate(const QIfAudioTrackItemPrivate &other)
        : QSharedData(other)
        , m_title(other.m_title)
        , m_artist(other.m_artist)
        , m_album(other.m_album)
        , m_genre(other.m_genre)
        , m_year(other.m_year)
        , m_trackNumber(other.m_trackNumber)
        , m_duration(other.m_duration)
        , m_coverArtUrl(other.m_coverArtUrl)
        , m_rating(other.m_rating)
    {}

    QString m_title;
    QString m_artist;
    QString m_album;
    QString m_genre;
    int m_year;
    int m_trackNumber;
    qint64 m_duration;
    QUrl m_coverArtUrl;
    int m_rating;
};

/*!
    \class QIfPlayableItem
    \inmodule QtIfMedia
    \brief The QIfPlayableItem is the base class of a row in the QIfPlayQueue model.
*/

/*!
    \qmltype PlayableItem
    \instantiates QIfPlayableItem
    \inqmlmodule QtInterfaceFramework.Media
    \inherits StandardItem
    \brief The PlayableItem is the base class of a row in the PlayQueue model.

    \note This item is not creatable from QML.
*/

/*!
    \qmlproperty QUrl PlayableItem::url

    The url to the resource which should be played.

    E.g. a local file url to a audio track or a url to a web stream.
*/

/*!
    \property QIfPlayableItem::url

    The url to the resource which should be played.

    E.g. a local file url to a audio track or a url to a web stream.
*/

QIfPlayableItem::QIfPlayableItem()
    : QIfStandardItem()
    , d(new QIfPlayableItemPrivate)
{
}

QIfPlayableItem::QIfPlayableItem(const QIfPlayableItem &rhs)
    : QIfStandardItem(rhs)
    , d(rhs.d)
{
}

QIfPlayableItem &QIfPlayableItem::operator=(const QIfPlayableItem &rhs)
{
    QIfStandardItem::operator=(rhs);
    if (this != &rhs)
        d.operator=(rhs.d);
    return *this;
}

QIfPlayableItem::~QIfPlayableItem()
{
}

QUrl QIfPlayableItem::url() const
{
    return d->m_url;
}

void QIfPlayableItem::setUrl(const QUrl &url)
{
    d->m_url = url;
}

/*!
    \reimp
*/
QString QIfPlayableItem::type() const
{
    return QLatin1String("playableitem");
}

/*!
    Returns \e true if this item is equal to \a other; otherwise returns false.

    \sa operator!=()
*/
bool QIfPlayableItem::operator==(const QIfPlayableItem &other) const
{
    return (QIfStandardItem::operator==(other) &&
            d->m_url == other.d->m_url);
}

/*!
    \fn bool QIfPlayableItem::operator!=(const QIfPlayableItem &other) const

    Returns \e true if this item is not equal to \a other; otherwise returns false.

    \sa operator==()
*/

/*!
    \class QIfAudioTrackItem
    \inmodule QtIfMedia
    \brief The QIfAudioTrackItem represents a playable audio track.
*/

/*!
    \qmltype AudioTrackItem
    \instantiates QIfAudioTrackItem
    \inqmlmodule QtInterfaceFramework.Media
    \inherits PlayableItem
    \brief The AudioTrackItem represents a playable audio track.

    \note This item is not creatable from QML.
*/

/*!
    \qmlproperty string AudioTrackItem::title

    Holds the title of the audio track.
*/

/*!
    \property QIfAudioTrackItem::title

    Holds the title of the audio track.
*/

/*!
    \qmlproperty string AudioTrackItem::album

    Holds the album name of the audio track.
*/

/*!
    \property QIfAudioTrackItem::album

    Holds the album name of the audio track.
*/

/*!
    \qmlproperty string AudioTrackItem::artist

    Holds the artist of the audio track.
*/

/*!
    \property QIfAudioTrackItem::artist

    Holds the artist of the audio track.
*/

/*!
    \qmlproperty string AudioTrackItem::genre

    Holds the genre of the audio track.
*/

/*!
    \property QIfAudioTrackItem::genre

    Holds the genre of the audio track.
*/

/*!
    \qmlproperty int AudioTrackItem::year

    Holds the release year of the audio track.
*/

/*!
    \property QIfAudioTrackItem::year

    Holds the release year of the audio track.
*/

/*!
    \qmlproperty int AudioTrackItem::trackNumber

    Holds the number this track has in the album.
*/

/*!
    \property QIfAudioTrackItem::trackNumber

    Holds the number this track has in the album.
*/

/*!
    \qmlproperty int AudioTrackItem::duration

    Holds the duration of the audio track in ms.
*/

/*!
    \property QIfAudioTrackItem::duration

    Holds the duration of the audio track.
*/

/*!
    \qmlproperty string AudioTrackItem::coverArt

    Holds the cover art of the audio track.

    This can be a url to a local image file or a link to a qml image provider
*/

/*!
    \property QIfAudioTrackItem::coverArtUrl

    Holds the cover art of the audio track.

    This can be a url to a local image file or a link to a qml image provider
*/

/*!
    \qmlproperty int AudioTrackItem::rating

    Holds the rating of the audio track.

    This is usually a value between \e 0 and \e 5.
*/

/*!
    \property QIfAudioTrackItem::rating

    Holds the rating of the audio track.

    This is usually a value between \e 0 and \e 5.
*/
QIfAudioTrackItem::QIfAudioTrackItem()
    : QIfPlayableItem()
    , d(new QIfAudioTrackItemPrivate)
{
}

QIfAudioTrackItem::QIfAudioTrackItem(const QIfAudioTrackItem &rhs)
    : QIfPlayableItem(rhs)
    , d(rhs.d)
{
}

QIfAudioTrackItem &QIfAudioTrackItem::operator=(const QIfAudioTrackItem &rhs)
{
    QIfPlayableItem::operator=(rhs);
    if (this != &rhs)
        d.operator=(rhs.d);
    return *this;
}

QIfAudioTrackItem::~QIfAudioTrackItem()
{
}

QString QIfAudioTrackItem::title() const
{
    return d->m_title;
}

void QIfAudioTrackItem::setTitle(const QString &title)
{
    d->m_title = title;
}

QString QIfAudioTrackItem::artist() const
{
    return d->m_artist;
}

void QIfAudioTrackItem::setArtist(const QString &artist)
{
    d->m_artist = artist;
}

QString QIfAudioTrackItem::album() const
{
    return d->m_album;
}

void QIfAudioTrackItem::setAlbum(const QString &album)
{
    d->m_album = album;
}

QString QIfAudioTrackItem::genre() const
{
    return d->m_genre;
}

void QIfAudioTrackItem::setGenre(const QString &genre)
{
    d->m_genre = genre;
}

int QIfAudioTrackItem::year() const
{
    return d->m_year;
}

void QIfAudioTrackItem::setYear(int year)
{
    d->m_year = year;
}

int QIfAudioTrackItem::trackNumber() const
{
    return d->m_trackNumber;
}

void QIfAudioTrackItem::setTrackNumber(int trackNumber)
{
    d->m_trackNumber = trackNumber;
}

qint64 QIfAudioTrackItem::duration() const
{
    return d->m_duration;
}

void QIfAudioTrackItem::setDuration(qint64 duration)
{
    d->m_duration = duration;
}

QUrl QIfAudioTrackItem::coverArtUrl() const
{
    return d->m_coverArtUrl;
}

void QIfAudioTrackItem::setCoverArtUrl(const QUrl &url)
{
    d->m_coverArtUrl = url;
}

int QIfAudioTrackItem::rating() const
{
    return d->m_rating;
}


void QIfAudioTrackItem::setRating(int rating)
{
    d->m_rating = rating;
}

/*!
    \reimp
*/
QString QIfAudioTrackItem::name() const
{
    return d->m_title;
}

/*!
    \reimp
*/
QString QIfAudioTrackItem::type() const
{
    return QLatin1String("audiotrack");
}

/*!
    Returns \e true if this item is equal to \a other; otherwise returns false.

    \sa operator!=()
*/
bool QIfAudioTrackItem::operator==(const QIfAudioTrackItem &other) const
{
    return (QIfPlayableItem::operator==(other) &&
            d->m_title == other.d->m_title &&
            d->m_artist == other.d->m_artist &&
            d->m_album == other.d->m_album &&
            d->m_genre == other.d->m_genre &&
            d->m_year == other.d->m_year &&
            d->m_trackNumber == other.d->m_trackNumber &&
            d->m_duration == other.d->m_duration &&
            d->m_coverArtUrl == other.d->m_coverArtUrl &&
            d->m_rating == other.d->m_rating);
}

QDataStream &operator<<(QDataStream &stream, const QIfAudioTrackItem &obj)
{
    stream << obj.id();
    stream << obj.title();
    stream << obj.artist();
    stream << obj.album();
    stream << obj.genre();
    stream << obj.year();
    stream << obj.trackNumber();
    stream << obj.duration();
    stream << obj.coverArtUrl();
    stream << obj.rating();
    stream << obj.url();
    stream << QVariant(obj.data());
    return stream;
}

QDataStream &operator>>(QDataStream &stream, QIfAudioTrackItem &obj)
{
    QString id;
    QString title;
    QString artist;
    QString album;
    QString genre;
    int year;
    int trackNumber;
    qint64 duration;
    QUrl coverArtUrl;
    int rating;
    QUrl url;
    QVariant data;
    stream >> id;
    stream >> title;
    stream >> artist;
    stream >> album;
    stream >> genre;
    stream >> year;
    stream >> trackNumber;
    stream >> duration;
    stream >> coverArtUrl;
    stream >> rating;
    stream >> url;
    stream >> data;
    obj.setId(id);
    obj.setTitle(title);
    obj.setArtist(artist);
    obj.setAlbum(album);
    obj.setGenre(genre);
    obj.setYear(year);
    obj.setTrackNumber(trackNumber);
    obj.setDuration(duration);
    obj.setCoverArtUrl(coverArtUrl);
    obj.setRating(rating);
    obj.setUrl(url);
    obj.setData(data.toMap());
    return stream;
}

/*!
    \fn bool QIfAudioTrackItem::operator!=(const QIfAudioTrackItem &other) const

    Returns \e true if this item is not equal to \a other; otherwise returns false.

    \sa operator==()
*/

QT_END_NAMESPACE
