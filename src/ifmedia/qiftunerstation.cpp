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

#include "qiftunerstation.h"

QT_BEGIN_NAMESPACE

class QIfTunerStationPrivate : public QSharedData
{
public:
    QIfTunerStationPrivate()
        : m_frequency(-1)
        , m_receptionQuality(-1)
    {}

    QIfTunerStationPrivate(const QIfTunerStationPrivate &other)
        : QSharedData(other)
        , m_stationName(other.m_stationName)
        , m_frequency(other.m_frequency)
        , m_stationLogoUrl(other.m_stationLogoUrl)
        , m_category(other.m_category)
        , m_radioText(other.m_radioText)
        , m_receptionQuality(other.m_receptionQuality)
    {}

    QString m_stationName;
    int m_frequency;
    QString m_stationLogoUrl;
    QString m_category;
    QString m_radioText;
    int m_receptionQuality;
};

class QIfAmFmTunerStationPrivate : public QSharedData
{
public:
    QIfAmFmTunerStationPrivate()
        : m_band(QIfAmFmTuner::FMBand)
    {}

    QIfAmFmTunerStationPrivate(const QIfAmFmTunerStationPrivate &other)
        : QSharedData(other)
        , m_band(other.m_band)
    {}

    QIfAmFmTuner::Band m_band;
};

/*!
    \class QIfTunerStation
    \inmodule QtIfMedia
    \brief The QIfTunerStation represents a tuner station.
*/

/*!
    \qmltype TunerStation
    \instantiates QIfTunerStation
    \inqmlmodule QtInterfaceFramework.Media
    \inherits StandardItem
    \brief The TunerStation represents a tuner station.

    \note This item is not creatable from QML.
*/

/*!
    \qmlproperty string TunerStation::stationName

    Holds the name of the tuner station.
*/

/*!
    \property QIfTunerStation::stationName

    Holds the name of the tuner station.
*/

/*!
    \qmlproperty int TunerStation::frequency

    Holds the frequency of the tuner station.
*/

/*!
    \property QIfTunerStation::frequency

    Holds the frequency of the tuner station.
*/

/*!
    \qmlproperty string TunerStation::stationLogoUrl

    Holds the logo of this station.

    This can be a url to a local image file or a link to a qml image provider.
*/

/*!
    \property QIfTunerStation::stationLogoUrl

    Holds the logo of this station.

    This can be a url to a local image file or a link to a qml image provider.
*/

/*!
    \qmlproperty string TunerStation::category

    Holds the category of the tuner station.

    For a AM/FM station this is usually string representation of pty (programme type) inside the RDS (Radio Data Segment).
*/

/*!
    \property QIfTunerStation::category

    Holds the category of the tuner station.

    For a AM/FM station this is usually string representation of pty (programme type) inside the RDS (Radio Data Segment).
*/

/*!
    \qmlproperty string TunerStation::radioText

    Holds the radio text transmitted by the tuner station.

    The radio text is used to provide additional information about the current station program or for commericials.
    It is expected that the backend extracts the useful information from the radioText and provide it in more suitable properties
    like the the currentArtist or similar.
*/

/*!
    \property QIfTunerStation::radioText

    Holds the radio text transmitted by the tuner station.

    The radio text is used to provide additional information about the current station program or for commericials.
    It is expected that the backend extracts the useful information from the radioText and provide it in more suitable properties
    like the the currentArtist or similar.
*/

/*!
    \qmlproperty int TunerStation::receptionQuality

    Holds the reception quality of the tuner station.

    //TODO What's the usual unit and range of this ?
*/

/*!
    \property QIfTunerStation::receptionQuality

    Holds the reception quality of the tuner station.

    //TODO What's the usual unit and range of this ?
*/

QIfTunerStation::QIfTunerStation()
    : QIfStandardItem()
    , d(new QIfTunerStationPrivate)
{
}

QIfTunerStation::QIfTunerStation(const QIfTunerStation &rhs)
    : QIfStandardItem(rhs)
    , d(rhs.d)
{
}

QIfTunerStation &QIfTunerStation::operator=(const QIfTunerStation &rhs)
{
    QIfStandardItem::operator=(rhs);
    if (this != &rhs)
        d.operator=(rhs.d);
    return *this;
}

QIfTunerStation::~QIfTunerStation()
{
}

QString QIfTunerStation::stationName() const
{
    return d->m_stationName;
}

void QIfTunerStation::setStationName(const QString &stationName)
{
    d->m_stationName = stationName;
}

int QIfTunerStation::frequency() const
{
    return d->m_frequency;
}

void QIfTunerStation::setFrequency(int frequency)
{
    d->m_frequency = frequency;
}

QString QIfTunerStation::stationLogoUrl() const
{
    return d->m_stationLogoUrl;
}

void QIfTunerStation::setStationLogoUrl(const QString &stationLogoUrl)
{
    d->m_stationLogoUrl = stationLogoUrl;
}

QString QIfTunerStation::category() const
{
    return d->m_category;
}

void QIfTunerStation::setCategory(const QString &category)
{
    d->m_category = category;
}

QString QIfTunerStation::radioText() const
{
    return d->m_radioText;
}

void QIfTunerStation::setRadioText(const QString &radioText)
{
    d->m_radioText = radioText;
}

int QIfTunerStation::receptionQuality() const
{
    return d->m_receptionQuality;
}

void QIfTunerStation::setReceptionQuality(int receptionQuality)
{
    d->m_receptionQuality = receptionQuality;
}

/*!
    \reimp
*/
QString QIfTunerStation::name() const
{
    return d->m_stationName;
}

/*!
    \reimp
*/
QString QIfTunerStation::type() const
{
    return QLatin1String("tunerstation");
}

/*!
    Returns \e true if this item is equal to \a other; otherwise returns false.

    \sa operator!=()
*/
bool QIfTunerStation::operator==(const QIfTunerStation &other) const
{
    return (QIfStandardItem::operator==(other) &&
            d->m_stationName == other.d->m_stationName &&
            d->m_frequency == other.d->m_frequency &&
            d->m_stationLogoUrl == other.d->m_stationLogoUrl &&
            d->m_category == other.d->m_category &&
            d->m_radioText == other.d->m_radioText &&
            d->m_receptionQuality == other.d->m_receptionQuality);
}

/*!
    \fn bool QIfTunerStation::operator!=(const QIfTunerStation &other) const

    Returns \e true if this item is not equal to \a other; otherwise returns false.

    \sa operator==()
*/

/*!
    \class QIfAmFmTunerStation
    \inmodule QtIfMedia
    \brief The QIfTunerStation represents a tuner station.
*/

/*!
    \qmltype AmFmTunerStation
    \instantiates QIfAmFmTunerStation
    \inqmlmodule QtInterfaceFramework.Media
    \inherits TunerStation
    \brief The TunerStation represents a tuner station.

    \note This item is not creatable from QML.
*/

/*!
    \qmlproperty enumeration AmFmTunerStation::band

    Holds the band of the tuner station.

    Available values are:
    \value AMBand
           The AM Band is based on the Amplitude Modulation technique and can range from 520 to 1610 kHz (1710 kHz).
           The step size is usually between 9 or 10 kHz.
    \value FMBand
           The FM Band is based on the Frequency Modulation technique and can range from 87.5 to 108.0 MHz.
           The step size is usually 100 kHz.
*/

/*!
    \property QIfAmFmTunerStation::band

    Holds the band of the tuner station.
*/

QIfAmFmTunerStation::QIfAmFmTunerStation()
    : QIfTunerStation()
    , d(new QIfAmFmTunerStationPrivate)
{
}

QIfAmFmTunerStation::QIfAmFmTunerStation(const QIfAmFmTunerStation &rhs)
    : QIfTunerStation(rhs)
    , d(rhs.d)
{
}

QIfAmFmTunerStation &QIfAmFmTunerStation::operator=(const QIfAmFmTunerStation &rhs)
{
    QIfTunerStation::operator=(rhs);
    if (this != &rhs)
        d.operator=(rhs.d);
    return *this;
}

QIfAmFmTunerStation::~QIfAmFmTunerStation()
{
}

QIfAmFmTuner::Band QIfAmFmTunerStation::band() const
{
    return d->m_band;
}

void QIfAmFmTunerStation::setBand(QIfAmFmTuner::Band band)
{
    d->m_band = band;
}

/*!
    \reimp
*/
QString QIfAmFmTunerStation::type() const
{
    return QLatin1String("amfmtunerstation");
}

/*!
    Returns \e true if this item is equal to \a other; otherwise returns false.

    \sa operator!=()
*/
bool QIfAmFmTunerStation::operator==(const QIfAmFmTunerStation &other) const
{
    return (QIfTunerStation::operator==(other) &&
            d->m_band == other.d->m_band);
}

/*!
    \fn bool QIfAmFmTunerStation::operator!=(const QIfAmFmTunerStation &other) const

    Returns \e true if this item is not equal to \a other; otherwise returns false.

    \sa operator==()
*/

QT_END_NAMESPACE
