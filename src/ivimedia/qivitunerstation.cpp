/****************************************************************************
**
** Copyright (C) 2017 Pelagicore AG
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtIVI module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL-QTAS$
** Commercial License Usage
** Licensees holding valid commercial Qt Automotive Suite licenses may use
** this file in accordance with the commercial license agreement provided
** with the Software or, alternatively, in accordance with the terms
** contained in a written agreement between you and The Qt Company.  For
** licensing terms and conditions see https://www.qt.io/terms-conditions.
** For further information use the contact form at https://www.qt.io/contact-us.
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
** SPDX-License-Identifier: LGPL-3.0
**
****************************************************************************/

#include "qivitunerstation.h"

QT_BEGIN_NAMESPACE

class QIviTunerStationPrivate : public QSharedData
{
public:
    QIviTunerStationPrivate()
        : m_frequency(-1)
        , m_receptionQuality(-1)
    {}

    QIviTunerStationPrivate(const QIviTunerStationPrivate &other)
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

class QIviAmFmTunerStationPrivate : public QSharedData
{
public:
    QIviAmFmTunerStationPrivate()
        : m_band(QIviAmFmTuner::FMBand)
    {}

    QIviAmFmTunerStationPrivate(const QIviAmFmTunerStationPrivate &other)
        : QSharedData(other)
        , m_band(other.m_band)
    {}

    QIviAmFmTuner::Band m_band;
};

/*!
    \class QIviTunerStation
    \inmodule QtIviMedia
    \brief The QIviTunerStation represents a tuner station.
*/

/*!
    \qmltype TunerStation
    \instantiates QIviTunerStation
    \inqmlmodule QtIvi.Media
    \inherits SearchAndBrowseModelItem
    \brief The TunerStation represents a tuner station.

    \note This item is not creatable from QML.
*/

/*!
    \qmlproperty string TunerStation::stationName

    Holds the name of the tuner station.
*/

/*!
    \property QIviTunerStation::stationName

    Holds the name of the tuner station.
*/

/*!
    \qmlproperty int TunerStation::frequency

    Holds the frequency of the tuner station.
*/

/*!
    \property QIviTunerStation::frequency

    Holds the frequency of the tuner station.
*/

/*!
    \qmlproperty string TunerStation::stationLogoUrl

    Holds the logo of this station.

    This can be a url to a local image file or a link to a qml image provider.
*/

/*!
    \property QIviTunerStation::stationLogoUrl

    Holds the logo of this station.

    This can be a url to a local image file or a link to a qml image provider.
*/

/*!
    \qmlproperty string TunerStation::category

    Holds the category of the tuner station.

    For a AM/FM station this is usually string representation of pty (programme type) inside the RDS (Radio Data Segment).
*/

/*!
    \property QIviTunerStation::category

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
    \property QIviTunerStation::radioText

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
    \property QIviTunerStation::receptionQuality

    Holds the reception quality of the tuner station.

    //TODO What's the usual unit and range of this ?
*/

QIviTunerStation::QIviTunerStation()
    : QIviSearchAndBrowseModelItem()
    , d(new QIviTunerStationPrivate)
{
}

QIviTunerStation::QIviTunerStation(const QIviTunerStation &rhs)
    : QIviSearchAndBrowseModelItem(rhs)
    , d(rhs.d)
{
}

QIviTunerStation &QIviTunerStation::operator=(const QIviTunerStation &rhs)
{
    QIviSearchAndBrowseModelItem::operator=(rhs);
    if (this != &rhs)
        d.operator=(rhs.d);
    return *this;
}

QIviTunerStation::~QIviTunerStation()
{
}

QString QIviTunerStation::stationName() const
{
    return d->m_stationName;
}

void QIviTunerStation::setStationName(const QString &stationName)
{
    d->m_stationName = stationName;
}

int QIviTunerStation::frequency() const
{
    return d->m_frequency;
}

void QIviTunerStation::setFrequency(int frequency)
{
    d->m_frequency = frequency;
}

QString QIviTunerStation::stationLogoUrl() const
{
    return d->m_stationLogoUrl;
}

void QIviTunerStation::setStationLogoUrl(QString stationLogoUrl)
{
    d->m_stationLogoUrl = stationLogoUrl;
}

QString QIviTunerStation::category() const
{
    return d->m_category;
}

void QIviTunerStation::setCategory(QString category)
{
    d->m_category = category;
}

QString QIviTunerStation::radioText() const
{
    return d->m_radioText;
}

void QIviTunerStation::setRadioText(QString radioText)
{
    d->m_radioText = radioText;
}

int QIviTunerStation::receptionQuality() const
{
    return d->m_receptionQuality;
}

void QIviTunerStation::setReceptionQuality(int receptionQuality)
{
    d->m_receptionQuality = receptionQuality;
}

/*!
    \reimp
*/
QString QIviTunerStation::name() const
{
    return d->m_stationName;
}

/*!
    \reimp
*/
QString QIviTunerStation::type() const
{
    return QLatin1String("tunerstation");
}

/*!
    Returns \e true if this item is equal to \a other; otherwise returns false.

    \sa operator!=()
*/
bool QIviTunerStation::operator==(const QIviTunerStation &other)
{
    return (QIviSearchAndBrowseModelItem::operator==(other) &&
            d->m_stationName == other.d->m_stationName &&
            d->m_frequency == other.d->m_frequency &&
            d->m_stationLogoUrl == other.d->m_stationLogoUrl &&
            d->m_category == other.d->m_category &&
            d->m_radioText == other.d->m_radioText &&
            d->m_receptionQuality == other.d->m_receptionQuality);
}

/*!
    \fn bool QIviTunerStation::operator!=(const QIviTunerStation &other)

    Returns \e true if this item is not equal to \a other; otherwise returns false.

    \sa operator==()
*/

/*!
    \class QIviAmFmTunerStation
    \inmodule QtIviMedia
    \brief The QIviTunerStation represents a tuner station.
*/

/*!
    \qmltype AmFmTunerStation
    \instantiates QIviAmFmTunerStation
    \inqmlmodule QtIvi.Media
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
    \property QIviAmFmTunerStation::band

    Holds the band of the tuner station.
*/

QIviAmFmTunerStation::QIviAmFmTunerStation()
    : QIviTunerStation()
    , d(new QIviAmFmTunerStationPrivate)
{
}

QIviAmFmTunerStation::QIviAmFmTunerStation(const QIviAmFmTunerStation &rhs)
    : QIviTunerStation(rhs)
    , d(rhs.d)
{
}

QIviAmFmTunerStation &QIviAmFmTunerStation::operator=(const QIviAmFmTunerStation &rhs)
{
    QIviTunerStation::operator=(rhs);
    if (this != &rhs)
        d.operator=(rhs.d);
    return *this;
}

QIviAmFmTunerStation::~QIviAmFmTunerStation()
{
}

QIviAmFmTuner::Band QIviAmFmTunerStation::band() const
{
    return d->m_band;
}

void QIviAmFmTunerStation::setBand(QIviAmFmTuner::Band band)
{
    d->m_band = band;
}

/*!
    \reimp
*/
QString QIviAmFmTunerStation::type() const
{
    return QLatin1String("amfmtunerstation");
}

/*!
    Returns \e true if this item is equal to \a other; otherwise returns false.

    \sa operator!=()
*/
bool QIviAmFmTunerStation::operator==(const QIviAmFmTunerStation &other)
{
    return (QIviTunerStation::operator==(other) &&
            d->m_band == other.d->m_band);
}

/*!
    \fn bool QIviAmFmTunerStation::operator!=(const QIviAmFmTunerStation &other)

    Returns \e true if this item is not equal to \a other; otherwise returns false.

    \sa operator==()
*/

QT_END_NAMESPACE
