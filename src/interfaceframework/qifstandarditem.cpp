// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qifstandarditem.h"

QT_BEGIN_NAMESPACE

class QIfStandardItemPrivate : public QSharedData
{
public:
    QIfStandardItemPrivate() = default;
    QIfStandardItemPrivate(const QIfStandardItemPrivate &other) = default;

    QString m_id;
    QVariantMap m_data;
};

/*!
    \class QIfStandardItem
    \inmodule QtInterfaceFramework
    \brief The QIfStandardItem is the base class of a row in the QIfPagingModel model and derived models.
*/

/*!
    \qmltype StandardItem
    \qmlabstract
    \instantiates QIfStandardItem
    \inqmlmodule QtInterfaceFramework
    \brief The StandardItem is the base class of a row in the PagingModel and derived models.

    \note This item is not creatable from QML.
*/

/*!
    \qmlproperty string StandardItem::id
    A unique identifier, which can be used to identify this item.

    This is mainly used by the backend to implement filtering or browsing.
*/

/*!
    \property QIfStandardItem::id
    A unique identifier, which can be used to identify this item.

    This is mainly used by the backend to implement filtering or browsing.
*/

/*!
    \qmlproperty string StandardItem::name
    The name of the item. E.g. The name of a contact in a addressbook, or the artist-name in a list of artists.
*/

/*!
    \property QIfStandardItem::name
    The name of the item. E.g. The name of a contact in a addressbook, or the artist-name in a list of artists.
*/

/*!
    \qmlproperty string StandardItem::type
    The type of the item. E.g. "artist", "track", "contact".
*/

/*!
    \property QIfStandardItem::type
    The type of the item. E.g. "artist", "track", "contact".
*/

/*!
    \qmlproperty object StandardItem::data
    A generic data field which can hold any data.

    This can be moved for storing additional data which is not part of the base class.
    E.g. the "composer" of an audio track.
*/

/*!
    \property QIfStandardItem::data
    A generic data field which can hold any data.

    This can be moved for storing additional data which is not part of the base class.
    E.g. the "composer" of an audio track.
*/

QIfStandardItem::QIfStandardItem()
    : d(new QIfStandardItemPrivate)
{

}

//defined here as a inline default copy constructor leads to compilation errors
QIfStandardItem::QIfStandardItem(const QIfStandardItem &rhs) = default;

QIfStandardItem &QIfStandardItem::operator=(const QIfStandardItem &rhs)
{
    if (this != &rhs)
        d.operator=(rhs.d);
    return *this;
}

//defined here as a inline default destructor leads to compilation errors
QIfStandardItem::~QIfStandardItem() = default;

QString QIfStandardItem::id() const
{
    return d->m_id;
}

void QIfStandardItem::setId(const QString &id)
{
    d->m_id = id;
}

QString QIfStandardItem::name() const
{
    return QString();
}

QString QIfStandardItem::type() const
{
    return QString();
}

QVariantMap QIfStandardItem::data() const
{
    return d->m_data;
}

void QIfStandardItem::setData(const QVariantMap &data)
{
    d->m_data = data;
}

/*!
    Returns \e true if this item is equal to \a other; otherwise returns false.

    \sa operator!=()
*/
bool QIfStandardItem::operator==(const QIfStandardItem &other) const
{
    return (d->m_id == other.d->m_id);
}

/*!
    \fn bool QIfStandardItem::operator!=(const QIfStandardItem &other) const

    Returns \e true if this item is not equal to \a other; otherwise returns false.

    \sa operator==()
*/

QT_END_NAMESPACE
