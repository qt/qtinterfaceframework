// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QIFPLAYABLEITEM_H
#define QIFPLAYABLEITEM_H

#include <QtIfMedia/qtifmediaglobal.h>
#include <QtInterfaceFramework/QIfFilterAndBrowseModel>
#include <QtInterfaceFramework/QIfStandardItem>
#include <QtCore/QUrl>

QT_BEGIN_NAMESPACE

class QIfPlayableItemPrivate;
class QIfAudioTrackItemPrivate;

class Q_QTIFMEDIA_EXPORT QIfPlayableItem : public QIfStandardItem
{
    Q_GADGET

    Q_PROPERTY(QUrl url READ url WRITE setUrl FINAL)

public:
    explicit QIfPlayableItem();
    explicit QIfPlayableItem(const QIfPlayableItem &);
    QIfPlayableItem &operator=(const QIfPlayableItem &);
    ~QIfPlayableItem() override;

    virtual QUrl url() const;
    virtual void setUrl(const QUrl &url);
    QString type() const override;

    bool operator==(const QIfPlayableItem &other) const;
    inline bool operator!=(const QIfPlayableItem &other) const { return !(*this == other); }

private:
    QSharedDataPointer<QIfPlayableItemPrivate> d;
};
Q_DECLARE_TYPEINFO(QIfPlayableItem, Q_MOVABLE_TYPE);

class Q_QTIFMEDIA_EXPORT QIfAudioTrackItem : public QIfPlayableItem
{
    Q_GADGET

    Q_PROPERTY(QString title READ title WRITE setTitle FINAL)
    Q_PROPERTY(QString artist READ artist WRITE setArtist FINAL)
    Q_PROPERTY(QString album READ album WRITE setAlbum FINAL)
    Q_PROPERTY(QString genre READ genre WRITE setGenre FINAL)
    Q_PROPERTY(int year READ year WRITE setYear FINAL)
    Q_PROPERTY(int trackNumber READ trackNumber WRITE setTrackNumber FINAL)
    Q_PROPERTY(qint64 duration READ duration WRITE setDuration FINAL)
    Q_PROPERTY(QUrl coverArtUrl READ coverArtUrl WRITE setCoverArtUrl FINAL)
    Q_PROPERTY(int rating READ rating WRITE setRating FINAL)

public:
    QIfAudioTrackItem();
    QIfAudioTrackItem(const QIfAudioTrackItem &);
    QIfAudioTrackItem &operator=(const QIfAudioTrackItem &);
    ~QIfAudioTrackItem() override;

    virtual QString title() const;
    virtual void setTitle(const QString &title);
    virtual QString artist() const;
    virtual void setArtist(const QString &artist);
    virtual QString album() const;
    virtual void setAlbum(const QString &album);
    virtual QString genre() const;
    virtual void setGenre(const QString &genre);
    virtual int year() const;
    virtual void setYear(int year);
    virtual int trackNumber() const;
    virtual void setTrackNumber(int trackNumber);
    virtual qint64 duration() const;
    virtual void setDuration(qint64 duration);
    virtual QUrl coverArtUrl() const;
    virtual void setCoverArtUrl(const QUrl &url);
    virtual int rating() const;
    virtual void setRating(int rating);
    QString name() const override;
    QString type() const override;

    bool operator==(const QIfAudioTrackItem &other) const;
    inline bool operator!=(const QIfAudioTrackItem &other) const { return !(*this == other); }

private:
    QSharedDataPointer<QIfAudioTrackItemPrivate> d;
};
Q_DECLARE_TYPEINFO(QIfAudioTrackItem, Q_MOVABLE_TYPE);

Q_QTIFMEDIA_EXPORT QDataStream &operator<<(QDataStream &stream, const QIfAudioTrackItem &obj);
Q_QTIFMEDIA_EXPORT QDataStream &operator>>(QDataStream &stream, QIfAudioTrackItem &obj);

QT_END_NAMESPACE

Q_DECLARE_METATYPE(QIfPlayableItem)
Q_DECLARE_METATYPE(QIfAudioTrackItem)

#endif // QIFPLAYABLEITEM_H
