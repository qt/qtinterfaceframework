// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "tunerplugin.h"
#include "amfmtunerbackend.h"
#include "searchandbrowsebackend.h"

#include <QStringList>
#include <QtDebug>
#include <QtInterfaceFramework/QIfFilterAndBrowseModel>
#include <QtIfMedia/QIfMediaPlayer>

TunerSimulatorPlugin::TunerSimulatorPlugin(QObject *parent)
    : QObject(parent)
    , m_amfmtuner(new AmFmTunerBackend(this))
    , m_searchbackend(new SearchAndBrowseBackend(m_amfmtuner, this))
{
}

QStringList TunerSimulatorPlugin::interfaces() const
{
    QStringList list;
    list << QStringLiteral(QIfFilterAndBrowseModel_iid);
    list << QStringLiteral(QIfAmFmTuner_iid);
    return list;
}

QIfFeatureInterface *TunerSimulatorPlugin::interfaceInstance(const QString &interface) const
{
    if (interface == QStringLiteral(QIfAmFmTuner_iid))
        return m_amfmtuner;
    else if (interface == QStringLiteral(QIfFilterAndBrowseModel_iid))
        return m_searchbackend;

    return nullptr;
}
