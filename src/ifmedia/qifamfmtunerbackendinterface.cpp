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

#include "qifamfmtunerbackendinterface.h"

QT_BEGIN_NAMESPACE

/*!
    \class QIfAmFmTunerBackendInterface
    \inmodule QtIfMedia
    \ingroup backends
    \inherits QObject
    \keyword org.qt-project.interfaceframework.AmFmTuner/1.0
    \brief The QIfAmFmTunerBackendInterface defines the interface for backends to the
    QIfAmFmTuner feature class.

    The QIfAmFmTunerBackendInterface is the interface used by \l QIfAmFmTuner

    The interface is discovered by a \l QIfAmFmTuner object, which connects to it and sets it up.

    <example of a fully featured backend>
*/

/*!
    Constructs a backend interface.

    The \a parent is sent to the QObject constructor.
*/
QIfAmFmTunerBackendInterface::QIfAmFmTunerBackendInterface(QObject *parent)
    : QIfFeatureInterface(parent)
{
}

/*!
    \fn QIfAmFmTunerBackendInterface::setFrequency(int frequency)

    Changes the frequency to the new value passed as \a frequency.

    This method is expected to emit a \l frequencyChanged() signal when the internal state changes due to this function call
*/

/*!
    \fn QIfAmFmTunerBackendInterface::setBand(QIfAmFmTuner::Band band)

    Changes the band to the new value passed as \a band.

    This method is expected to emit a \l bandChanged() signal when the internal state changes due to this function call
*/

/*!
    \fn QIfAmFmTunerBackendInterface::stepUp()

    Increases the frequency by the current bands step size.

    \sa frequencyChanged stationChanged
*/

/*!
    \fn QIfAmFmTunerBackendInterface::stepDown()

    Decreases the frequency by the current bands step size.

    \sa frequencyChanged stationChanged
*/

/*!
    \fn QIfAmFmTunerBackendInterface::seekUp()

    Seeks the next available Station in upwards direction and tunes it.

    If the end of the band where hit without finding a station, the search will be continued from the beginning of the band.

    \sa frequencyChanged stationChanged
*/

/*!
    \fn QIfAmFmTunerBackendInterface::seekDown()

    Seeks the next available Station in downwards direction and tunes it.

    If the beginning of the band where hit without finding a station, the search will be continued from the end of the band.

    \sa frequencyChanged stationChanged
*/

/*!
    \fn QIfAmFmTunerBackendInterface::startScan()

    Starts a scan through all available stations.
    The scan will seek to the next available station and will stay there for some seconds until it seeks to the next station.
    It is expected that this function will inform about the current state of the backend by emitting the scanStatusChanged() signal.

    \sa stopScan() scanStatusChanged()
*/

/*!
    \fn QIfAmFmTunerBackendInterface::stopScan()

    Stops the currently active scan. If no scan is active, this method does nothing.
    It is expected that this function will inform about the current state of the backend by emitting the scanStatusChanged() signal.

    \sa startScan() scanStatusChanged()
*/

/*!
    \fn QIfAmFmTunerBackendInterface::frequencyChanged(int frequency)

    Emitted when the current frequency changed. The new value is passed as \a frequency.
*/

/*!
    \fn QIfAmFmTunerBackendInterface::minimumFrequencyChanged(int minimumFrequency)

    Emitted when the minimum frequency of the current band changed. The new value is passed as \a minimumFrequency.
    This is usually caused by a switching the band.

    \sa bandChanged() maximumFrequencyChanged() stepSizeChanged()
*/

/*!
    \fn QIfAmFmTunerBackendInterface::maximumFrequencyChanged(int maximumFrequency)

    Emitted when the maximum frequency of the current band changed. The new value is passed as \a maximumFrequency.
    This is usually caused by a switching the band.

    \sa bandChanged() minimumFrequencyChanged() stepSizeChanged()
*/

/*!
    \fn QIfAmFmTunerBackendInterface::stepSizeChanged(int stepSize)

    Emitted when the frequency step size of the current band changed. The new value is passed as \a stepSize.
    This is usually caused by a switching the band.

    \sa bandChanged() minimumFrequencyChanged() maximumFrequencyChanged()
*/

/*!
    \fn QIfAmFmTunerBackendInterface::bandChanged(QIfAmFmTuner::Band band)

    Emitted when the current band changed. The new value is passed as \a band.

    \sa minimumFrequencyChanged() maximumFrequencyChanged() stepSizeChanged()
*/

/*!
    \fn QIfAmFmTunerBackendInterface::stationChanged(const QIfAmFmTunerStation &station)

    Emitted when the current station changed. The new station is passed as \a station.
*/

/*!
    \fn QIfAmFmTunerBackendInterface::scanStatusChanged(bool scanRunning)

    Emitted when the status of the station scan has changed. This signal should be emitted
    as a result of a call to startScan() or stopScan() and informs about the current status using the \a scanRunning argument.

    \sa startScan() stopScan()
*/

QT_END_NAMESPACE
