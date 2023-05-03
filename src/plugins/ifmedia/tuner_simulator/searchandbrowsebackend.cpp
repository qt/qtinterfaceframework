// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "searchandbrowsebackend.h"
#include "amfmtunerbackend.h"

#include <QtDebug>

using namespace Qt::StringLiterals;

SearchAndBrowseBackend::SearchAndBrowseBackend(AmFmTunerBackend *tunerBackend, QObject *parent)
    : QIfFilterAndBrowseModelInterface(parent)
    , m_tunerBackend(tunerBackend)
{
    qRegisterMetaType<QIfAmFmTunerStation>();
}

void SearchAndBrowseBackend::initialize()
{
    QStringList contentTypes;
    contentTypes << u"stations"_s;
    contentTypes << u"presets"_s;
    emit availableContentTypesChanged(contentTypes);
    emit initializationDone();
}

void SearchAndBrowseBackend::registerInstance(const QUuid &identifier)
{
    m_contentType.insert(identifier, QString());
}

void SearchAndBrowseBackend::unregisterInstance(const QUuid &identifier)
{
    m_contentType.remove(identifier);
}

void SearchAndBrowseBackend::setContentType(const QUuid &identifier, const QString &contentType)
{
    m_contentType[identifier] = contentType;
    emit queryIdentifiersChanged(identifier, identifiersFromItem<QIfAmFmTunerStation>());
    emit contentTypeChanged(identifier, contentType);
}

void SearchAndBrowseBackend::setupFilter(const QUuid &identifier, QIfAbstractQueryTerm *term, const QList<QIfOrderTerm> &orderTerms)
{
    Q_UNUSED(identifier)
    Q_UNUSED(term)
    Q_UNUSED(orderTerms)
}

void SearchAndBrowseBackend::fetchData(const QUuid &identifier, int start, int count)
{
    emit supportedCapabilitiesChanged(identifier, QtInterfaceFrameworkModule::ModelCapabilities(
                                          QtInterfaceFrameworkModule::SupportsStatelessNavigation |
                                          QtInterfaceFrameworkModule::SupportsGetSize |
                                          QtInterfaceFrameworkModule::SupportsInsert |
                                          QtInterfaceFrameworkModule::SupportsMove |
                                          QtInterfaceFrameworkModule::SupportsRemove
                                          ));

    QVector<QIfAmFmTunerStation> stations;

    if (m_contentType[identifier] == QLatin1String("station"))
        stations = m_tunerBackend->m_bandHash[QIfAmFmTuner::AMBand].m_stations + m_tunerBackend->m_bandHash[QIfAmFmTuner::FMBand].m_stations;
    else if (m_contentType[identifier] == QLatin1String("presets"))
        stations = m_presets;
    else
        return;

    emit countChanged(identifier, stations.length());
    QVariantList requestedStations;

    int size = qMin(count, stations.length());
    for (int i = start; i < size; i++)
        requestedStations.append(QVariant::fromValue(stations.at(i)));

    emit dataFetched(identifier, requestedStations, start, start + count < stations.length());
}

QIfPendingReply<QString> SearchAndBrowseBackend::goBack(const QUuid &identifier)
{
    Q_UNUSED(identifier)
    return QIfPendingReply<QString>::createFailedReply();
}

QIfPendingReply<QString> SearchAndBrowseBackend::goForward(const QUuid &identifier, int index)
{
    Q_UNUSED(identifier)
    Q_UNUSED(index)
    return QIfPendingReply<QString>::createFailedReply();
}

QIfPendingReply<void> SearchAndBrowseBackend::insert(const QUuid &identifier, int index, const QVariant &item)
{
    const QIfAmFmTunerStation *station = qtif_gadgetFromVariant<QIfAmFmTunerStation>(this, item);
    if (!station)
        return QIfPendingReply<void>::createFailedReply();

    const QString type = m_contentType.value(identifier);
    if (type != QLatin1String("presets"))
        return QIfPendingReply<void>::createFailedReply();

    m_presets.insert(index, *station);
    QVariantList stations = { QVariant::fromValue(*station) };
    emit dataChanged(identifier, stations, index, 0);

    QIfPendingReply<void> reply;
    reply.setSuccess();
    return reply;
}

QIfPendingReply<void> SearchAndBrowseBackend::remove(const QUuid &identifier, int index)
{
    const QString type = m_contentType.value(identifier);
    if (type != QLatin1String("presets"))
        return QIfPendingReply<void>::createFailedReply();

    m_presets.removeAt(index);
    emit dataChanged(identifier, QVariantList(), index, 1);

    QIfPendingReply<void> reply;
    reply.setSuccess();
    return reply;
}

QIfPendingReply<void> SearchAndBrowseBackend::move(const QUuid &identifier, int currentIndex, int newIndex)
{
    const QString type = m_contentType.value(identifier);
    if (type != QLatin1String("presets"))
        return QIfPendingReply<void>::createFailedReply();

    int min = qMin(currentIndex, newIndex);
    int max = qMax(currentIndex, newIndex);

    m_presets.move(currentIndex, newIndex);
    QVariantList stations;
    for (int i = min; i <= max; i++)
        stations.append(QVariant::fromValue(m_presets.at(i)));

    emit dataChanged(identifier, stations, min, max - min + 1);

    QIfPendingReply<void> reply;
    reply.setSuccess();
    return reply;
}

QIfPendingReply<int> SearchAndBrowseBackend::indexOf(const QUuid &identifier, const QVariant &item)
{
    const QIfAmFmTunerStation *station = qtif_gadgetFromVariant<QIfAmFmTunerStation>(this, item);
    if (!station)
        return QIfPendingReply<int>::createFailedReply();

    QVector<QIfAmFmTunerStation> stations;
    const QString type = m_contentType.value(identifier);

    if (type == QLatin1String("station"))
        stations = m_tunerBackend->m_bandHash[QIfAmFmTuner::AMBand].m_stations + m_tunerBackend->m_bandHash[QIfAmFmTuner::FMBand].m_stations;
    else if (type == QLatin1String("presets"))
        stations = m_presets;
    else
        return QIfPendingReply<int>::createFailedReply();

    QIfPendingReply<int> reply;
    reply.setSuccess(stations.indexOf(*station));
    return reply;
}
