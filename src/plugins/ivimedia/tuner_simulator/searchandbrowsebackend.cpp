/****************************************************************************
**
** Copyright (C) 2019 Luxoft Sweden AB
** Copyright (C) 2018 Pelagicore AG
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtIvi module of the Qt Toolkit.
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

#include "searchandbrowsebackend.h"
#include "amfmtunerbackend.h"

#include <QtDebug>

SearchAndBrowseBackend::SearchAndBrowseBackend(AmFmTunerBackend *tunerBackend, QObject *parent)
    : QIviSearchAndBrowseModelInterface(parent)
    , m_tunerBackend(tunerBackend)
{
    qRegisterMetaType<QIviAmFmTunerStation>();
}

void SearchAndBrowseBackend::initialize()
{
    QStringList contentTypes;
    contentTypes << QStringLiteral("stations");
    contentTypes << QStringLiteral("presets");
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
    emit queryIdentifiersChanged(identifier, identifiersFromItem<QIviAmFmTunerStation>());
    emit contentTypeChanged(identifier, contentType);
}

void SearchAndBrowseBackend::setupFilter(const QUuid &identifier, QIviAbstractQueryTerm *term, const QList<QIviOrderTerm> &orderTerms)
{
    Q_UNUSED(identifier)
    Q_UNUSED(term)
    Q_UNUSED(orderTerms)
}

void SearchAndBrowseBackend::fetchData(const QUuid &identifier, int start, int count)
{
    emit supportedCapabilitiesChanged(identifier, QtIviCoreModule::ModelCapabilities(
                                          QtIviCoreModule::SupportsStatelessNavigation |
                                          QtIviCoreModule::SupportsGetSize |
                                          QtIviCoreModule::SupportsInsert |
                                          QtIviCoreModule::SupportsMove |
                                          QtIviCoreModule::SupportsRemove
                                          ));

    QVector<QIviAmFmTunerStation> stations;

    if (m_contentType[identifier] == QLatin1String("station"))
        stations = m_tunerBackend->m_bandHash[QIviAmFmTuner::AMBand].m_stations + m_tunerBackend->m_bandHash[QIviAmFmTuner::FMBand].m_stations;
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

QIviPendingReply<QString> SearchAndBrowseBackend::goBack(const QUuid &identifier)
{
    Q_UNUSED(identifier)
    return QIviPendingReply<QString>::createFailedReply();
}

QIviPendingReply<QString> SearchAndBrowseBackend::goForward(const QUuid &identifier, int index)
{
    Q_UNUSED(identifier)
    Q_UNUSED(index)
    return QIviPendingReply<QString>::createFailedReply();
}

QIviPendingReply<void> SearchAndBrowseBackend::insert(const QUuid &identifier, int index, const QVariant &item)
{
    const QIviAmFmTunerStation *station = qtivi_gadgetFromVariant<QIviAmFmTunerStation>(this, item);
    if (!station)
        return QIviPendingReply<void>::createFailedReply();

    const QString type = m_contentType.value(identifier);
    if (type != QLatin1String("presets"))
        return QIviPendingReply<void>::createFailedReply();

    m_presets.insert(index, *station);
    QVariantList stations = { QVariant::fromValue(*station) };
    emit dataChanged(identifier, stations, index, 0);

    QIviPendingReply<void> reply;
    reply.setSuccess();
    return reply;
}

QIviPendingReply<void> SearchAndBrowseBackend::remove(const QUuid &identifier, int index)
{
    const QString type = m_contentType.value(identifier);
    if (type != QLatin1String("presets"))
        return QIviPendingReply<void>::createFailedReply();

    m_presets.removeAt(index);
    emit dataChanged(identifier, QVariantList(), index, 1);

    QIviPendingReply<void> reply;
    reply.setSuccess();
    return reply;
}

QIviPendingReply<void> SearchAndBrowseBackend::move(const QUuid &identifier, int currentIndex, int newIndex)
{
    const QString type = m_contentType.value(identifier);
    if (type != QLatin1String("presets"))
        return QIviPendingReply<void>::createFailedReply();

    int min = qMin(currentIndex, newIndex);
    int max = qMax(currentIndex, newIndex);

    m_presets.move(currentIndex, newIndex);
    QVariantList stations;
    for (int i = min; i <= max; i++)
        stations.append(QVariant::fromValue(m_presets.at(i)));

    emit dataChanged(identifier, stations, min, max - min + 1);

    QIviPendingReply<void> reply;
    reply.setSuccess();
    return reply;
}

QIviPendingReply<int> SearchAndBrowseBackend::indexOf(const QUuid &identifier, const QVariant &item)
{
    const QIviAmFmTunerStation *station = qtivi_gadgetFromVariant<QIviAmFmTunerStation>(this, item);
    if (!station)
        return QIviPendingReply<int>::createFailedReply();

    QVector<QIviAmFmTunerStation> stations;
    const QString type = m_contentType.value(identifier);

    if (type == QLatin1String("station"))
        stations = m_tunerBackend->m_bandHash[QIviAmFmTuner::AMBand].m_stations + m_tunerBackend->m_bandHash[QIviAmFmTuner::FMBand].m_stations;
    else if (type == QLatin1String("presets"))
        stations = m_presets;
    else
        return QIviPendingReply<int>::createFailedReply();

    QIviPendingReply<int> reply;
    reply.setSuccess(stations.indexOf(*station));
    return reply;
}
