// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qifamfmtuner.h"
#include "qifamfmtuner_p.h"
#include <QtInterfaceFramework/QIfServiceObject>
#include <QtDebug>

using namespace Qt::StringLiterals;

QT_BEGIN_NAMESPACE

QIfAmFmTunerPrivate::QIfAmFmTunerPrivate(const QString &interface, QIfAmFmTuner *parent)
    : QIfAbstractFeaturePrivate(interface, parent)
    , q_ptr(parent)
    , m_frequency(-1)
    , m_minimumFrequency(-1)
    , m_maximumFrequency(-1)
    , m_stepSize(-1)
    , m_band(QIfAmFmTuner::FMBand)
    , m_scanRunning(false)
{
    qRegisterMetaType<QIfTunerStation>();
    qRegisterMetaType<QIfAmFmTunerStation>();
}

void QIfAmFmTunerPrivate::clearToDefaults()
{
    onFrequencyChanged(-1);
    onMinimumFrequencyChanged(-1);
    onMaximumFrequencyChanged(-1);
    onStepSizeChanged(-1);
    onBandChanged(QIfAmFmTuner::FMBand);
    onStationChanged(QIfAmFmTunerStation());
    onScanStatusChanged(false);
}

void QIfAmFmTunerPrivate::onFrequencyChanged(int frequency)
{
    if (m_frequency == frequency)
        return;

    Q_Q(QIfAmFmTuner);
    m_frequency = frequency;
    emit q->frequencyChanged(frequency);
}

void QIfAmFmTunerPrivate::onMinimumFrequencyChanged(int frequency)
{
    if (m_minimumFrequency == frequency)
        return;

    Q_Q(QIfAmFmTuner);
    m_minimumFrequency = frequency;
    emit q->minimumFrequencyChanged(frequency);
}

void QIfAmFmTunerPrivate::onMaximumFrequencyChanged(int frequency)
{
    if (m_maximumFrequency == frequency)
        return;

    Q_Q(QIfAmFmTuner);
    m_maximumFrequency = frequency;
    emit q->maximumFrequencyChanged(frequency);
}

void QIfAmFmTunerPrivate::onStepSizeChanged(int stepSize)
{
    if (m_stepSize == stepSize)
        return;

    Q_Q(QIfAmFmTuner);
    m_stepSize = stepSize;
    emit q->stepSizeChanged(stepSize);
}

void QIfAmFmTunerPrivate::onBandChanged(QIfAmFmTuner::Band band)
{
    if (m_band == band)
        return;

    Q_Q(QIfAmFmTuner);
    m_band = band;
    emit q->bandChanged(band);
}

void QIfAmFmTunerPrivate::onStationChanged(const QIfAmFmTunerStation &station)
{
    if (m_station == station)
        return;

    Q_Q(QIfAmFmTuner);
    m_station = station;
    emit q->stationChanged(station);
}

void QIfAmFmTunerPrivate::onScanStatusChanged(bool scanRunning)
{
    if (m_scanRunning == scanRunning)
        return;

    Q_Q(QIfAmFmTuner);
    m_scanRunning = scanRunning;
    emit q->scanRunningChanged(scanRunning);
    if (scanRunning)
        emit q->scanStarted();
    else
        emit q->scanStopped();
}

QIfAmFmTunerBackendInterface *QIfAmFmTunerPrivate::tunerBackend() const
{
    return backend<QIfAmFmTunerBackendInterface*>();
}

/*!
    \class QIfAmFmTuner
    \inmodule QtIfMedia
    \brief Provides a interface to control the AM/FM tuner.

    The QIfAmFmTuner provides the methods to control a AM/FM tuner.

    Uses \c qtifmedia as configurationId for \l QIfConfiguration based settings.
*/

/*!
    \qmltype AmFmTuner
    \instantiates QIfAmFmTuner
    \inqmlmodule QtInterfaceFramework.Media
    \inherits AbstractFeature
    \brief Provides a interface to control the AM/FM tuner.

    The AmFmTuner provides the methods to control a AM/FM tuner.

    Uses \c qtifmedia as configurationId for \l InterfaceFrameworkConfiguration based settings.
*/

/*!
    \enum QIfAmFmTuner::Band
    \value AMBand
          The AM Band is based on the Amplitude Modulation technique and can range from 520 to 1610 kHz (1710 kHz).
          The step size is usually between 9 or 10 kHz.
    \value FMBand
          The FM Band is based on the Frequency Modulation technique and can range from 87.5 to 108.0 MHz.
          The step size is usually 100 kHz.
*/

/*!
    Constructs a QIfAmFmTuner.

    The \a parent argument is passed on to the \l QIfAbstractFeature base class.
*/
QIfAmFmTuner::QIfAmFmTuner(QObject *parent)
    : QIfAbstractFeature(*new QIfAmFmTunerPrivate(QLatin1String(QIfAmFmTuner_iid), this), parent)
{
    setConfigurationId(u"qtifmedia"_s);
}

/*!
    \qmlproperty int AmFmTuner::frequency
    \brief The current frequency of the tuner.
*/
/*!
    \property QIfAmFmTuner::frequency
    \brief The current frequency of the tuner.
*/
int QIfAmFmTuner::frequency() const
{
    Q_D(const QIfAmFmTuner);
    return d->m_frequency;
}

/*!
    \qmlproperty int AmFmTuner::minimumFrequency
    \brief The minimum frequency of the current band.
*/
/*!
    \property QIfAmFmTuner::minimumFrequency
    \brief The minimum frequency of the current band.
*/
int QIfAmFmTuner::minimumFrequency() const
{
    Q_D(const QIfAmFmTuner);
    return d->m_minimumFrequency;
}

/*!
    \qmlproperty int AmFmTuner::maximumFrequency
    \brief The maximum frequency of the current band.
*/
/*!
    \property QIfAmFmTuner::maximumFrequency
    \brief The maximum frequency of the current band.
*/
int QIfAmFmTuner::maximumFrequency() const
{
    Q_D(const QIfAmFmTuner);
    return d->m_maximumFrequency;
}

/*!
    \qmlproperty int AmFmTuner::stepSize
    \brief The frequency step size of the current band.

    \sa stepUp() stepDown()
*/
/*!
    \property QIfAmFmTuner::stepSize
    \brief The frequency step size of the current band.

    \sa stepUp() stepDown()
*/
int QIfAmFmTuner::stepSize() const
{
    Q_D(const QIfAmFmTuner);
    return d->m_stepSize;
}

/*!
    \qmlproperty enumeration AmFmTuner::band
    \brief The current band of the tuner.

    Available values are:
    \value AMBand
           The AM Band is based on the Amplitude Modulation technique and can range from 520 to 1610 kHz (1710 kHz).
           The step size is usually between 9 or 10 kHz.
    \value FMBand
           The FM Band is based on the Frequency Modulation technique and can range from 87.5 to 108.0 MHz.
           The step size is usually 100 kHz.
*/
/*!
    \property QIfAmFmTuner::band
    \brief The current band of the tuner.
*/
QIfAmFmTuner::Band QIfAmFmTuner::band() const
{
    Q_D(const QIfAmFmTuner);
    return d->m_band;
}

/*!
    \qmlproperty AmFmTunerStation AmFmTuner::station
    \brief The currently tuned station.
*/
/*!
    \property QIfAmFmTuner::station
    \brief The currently tuned station.
*/
QIfAmFmTunerStation QIfAmFmTuner::station() const
{
    Q_D(const QIfAmFmTuner);
    return d->m_station;
}

/*!
    \qmlproperty bool AmFmTuner::scanRunning
    \c true while a scan is in progress, \c false otherwise.

    \sa startScan() stopScan() scanStarted() scanStopped()
*/
/*!
    \property QIfAmFmTuner::scanRunning
    \c true while a scan is in progress, \c false otherwise.

    \sa startScan() stopScan() scanStarted() scanStopped()
*/
bool QIfAmFmTuner::isScanRunning() const
{
    Q_D(const QIfAmFmTuner);
    return d->m_scanRunning;
}

/*!
    \qmlmethod AmFmTuner::tune(AmFmTunerStation station)

    Tunes to the provided \a station.
*/

/*!
    \fn void QIfAmFmTuner::tune(const QIfAmFmTunerStation &station)

    Tunes to the provided \a station.
*/
void QIfAmFmTuner::tune(const QIfAmFmTunerStation &station)
{
    Q_D(QIfAmFmTuner);

    if (station.band() != d->m_band)
        setBand(station.band());
    if (station.frequency() != d->m_frequency)
        setFrequency(station.frequency());
}

void QIfAmFmTuner::setFrequency(int frequency)
{
    Q_IF_BACKEND(QIfAmFmTuner, d->tunerBackend(), "Can't set the frequency without a connected backend");

    //TODO Check the minimum/maximum Frequency here ? Add a convention how things like this are done this is also used in the vehicle functions module.

    backend->setFrequency(frequency);
}

void QIfAmFmTuner::setBand(QIfAmFmTuner::Band band)
{
    Q_IF_BACKEND(QIfAmFmTuner, d->tunerBackend(), "Can't set the band without a connected backend");

    //TODO Check the minimum/maximum Frequency here ? Add a convention how things like this are done this is also used in the vehicle functions module.

    backend->setBand(band);
}

/*!
    \qmlmethod AmFmTuner::stepUp()

    Increases the frequency by the current bands step size.
*/

/*!
    \fn void QIfAmFmTuner::stepUp()

    Increases the frequency by the current bands step size.
*/
void QIfAmFmTuner::stepUp()
{
    Q_IF_BACKEND(QIfAmFmTuner, d->tunerBackend(), "Can't step up without a connected backend");

    //TODO Should we pass this down or use setFrequency instead ?
    backend->stepUp();
}

/*!
    \qmlmethod AmFmTuner::stepDown()

    Decreases the frequency by the current bands step size.
*/

/*!
    \fn void QIfAmFmTuner::stepDown()

    Decreases the frequency by the current bands step size.
*/
void QIfAmFmTuner::stepDown()
{
    Q_IF_BACKEND(QIfAmFmTuner, d->tunerBackend(), "Can't step down without a connected backend");

    //TODO Should we pass this down or use setFrequency instead ?
    backend->stepDown();
}

/*!
    \qmlmethod AmFmTuner::seekUp()

    Seeks the next available Station in upwards direction and tunes it.

    If the end of the band where hit without finding a station, the search will be continued from the beginning of the band.
*/

/*!
    \fn void QIfAmFmTuner::seekUp()

    Seeks the next available Station in upwards direction and tunes it.

    If the end of the band where hit without finding a station, the search will be continued from the beginning of the band.
*/
void QIfAmFmTuner::seekUp()
{
    Q_IF_BACKEND(QIfAmFmTuner, d->tunerBackend(), "Can't seek up without a connected backend");

    backend->seekUp();
}

/*!
    \qmlmethod AmFmTuner::seekDown()

    Seeks the next available Station in downwards direction and tunes it.

    If the beginning of the band where hit without finding a station, the search will be continued from the end of the band.
*/

/*!
    \fn void QIfAmFmTuner::seekDown()

    Seeks the next available Station in downwards direction and tunes it.

    If the beginning of the band where hit without finding a station, the search will be continued from the end of the band.
*/
void QIfAmFmTuner::seekDown()
{
    Q_IF_BACKEND(QIfAmFmTuner, d->tunerBackend(), "Can't seek down without a connected backend");

    backend->seekDown();
}

/*!
    \qmlmethod AmFmTuner::startScan()

    Starts a scan through all available stations.

    The scan will seek to the next available station and will stay there for some seconds until it seeks to the next station.

    \sa stopScan scanRunning scanStarted scanStopped
*/

/*!
    \fn void QIfAmFmTuner::startScan()

    Starts a scan through all available stations.

    The scan will seek to the next available station and will stay there for some seconds until it seeks to the next station.

    \sa stopScan scanRunning scanStarted scanStopped
*/
void QIfAmFmTuner::startScan()
{
    Q_IF_BACKEND(QIfAmFmTuner, d->tunerBackend(), "Can't start scanning without a connected backend");

    if (d->m_scanRunning) {
        qWarning("A scan is already in progress");
        return;
    }

    backend->startScan();
}

/*!
    \qmlmethod AmFmTuner::stopScan()

    Stops the currently active scan. If no scan is active, this method does nothing.

    \sa startScan scanRunning scanStarted scanStopped
*/

/*!
    \fn void QIfAmFmTuner::stopScan()

    Stops the currently active scan. If no scan is active, this method does nothing.

    \sa startScan scanRunning scanStarted scanStopped
*/
void QIfAmFmTuner::stopScan()
{
    Q_IF_BACKEND(QIfAmFmTuner, d->tunerBackend(), "Can't stop scanning without a connected backend");

    if (!d->m_scanRunning) {
        qWarning("Currently no scan is active which can be stopped");
        return;
    }

    backend->stopScan();
}

/*!
    \internal
*/
QIfAmFmTuner::QIfAmFmTuner(QIfAmFmTunerPrivate &dd, QObject *parent)
    : QIfAbstractFeature(dd, parent)
{
}

/*!
    \reimp
*/
void QIfAmFmTuner::connectToServiceObject(QIfServiceObject *serviceObject)
{
    Q_UNUSED(serviceObject);

    Q_D(QIfAmFmTuner);

    QIfAmFmTunerBackendInterface *backend = d->tunerBackend();
    if (!backend)
        return;

    QObjectPrivate::connect(backend, &QIfAmFmTunerBackendInterface::frequencyChanged,
                            d, &QIfAmFmTunerPrivate::onFrequencyChanged);
    QObjectPrivate::connect(backend, &QIfAmFmTunerBackendInterface::minimumFrequencyChanged,
                            d, &QIfAmFmTunerPrivate::onMinimumFrequencyChanged);
    QObjectPrivate::connect(backend, &QIfAmFmTunerBackendInterface::maximumFrequencyChanged,
                            d, &QIfAmFmTunerPrivate::onMaximumFrequencyChanged);
    QObjectPrivate::connect(backend, &QIfAmFmTunerBackendInterface::stepSizeChanged,
                            d, &QIfAmFmTunerPrivate::onStepSizeChanged);
    QObjectPrivate::connect(backend, &QIfAmFmTunerBackendInterface::bandChanged,
                            d, &QIfAmFmTunerPrivate::onBandChanged);
    QObjectPrivate::connect(backend, &QIfAmFmTunerBackendInterface::stationChanged,
                            d, &QIfAmFmTunerPrivate::onStationChanged);
    QObjectPrivate::connect(backend, &QIfAmFmTunerBackendInterface::scanStatusChanged,
                            d, &QIfAmFmTunerPrivate::onScanStatusChanged);

    QIfAbstractFeature::connectToServiceObject(serviceObject);
}

/*!
    \reimp
*/
void QIfAmFmTuner::clearServiceObject()
{
    Q_D(QIfAmFmTuner);
    d->clearToDefaults();
}

/*!
    \qmlsignal AmFmTuner::scanStarted()

    A new scan has started and is now running.

    \sa startScan stopScan scanRunning scanStopped
*/

/*!
    \fn void QIfAmFmTuner::scanStarted()

    A new scan has started and is now running.

    \sa startScan stopScan scanRunning scanStopped
*/

/*!
    \qmlsignal AmFmTuner::scanStopped()

    The currently active scan has stopped.

    \sa startScan stopScan scanRunning scanStarted
*/

/*!
    \fn void QIfAmFmTuner::scanStopped()

    The currently active scan has stopped.

    \sa startScan stopScan scanRunning scanStarted
*/

QT_END_NAMESPACE

#include "moc_qifamfmtuner.cpp"
