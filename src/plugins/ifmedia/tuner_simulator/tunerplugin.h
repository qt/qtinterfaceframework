// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef TUNERPLUGIN_H
#define TUNERPLUGIN_H

#include <QtInterfaceFramework/QIfServiceInterface>

class AmFmTunerBackend;
class SearchAndBrowseBackend;

class TunerSimulatorPlugin : public QObject, QIfServiceInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID QIfServiceInterface_iid FILE "tuner_simulator.json")
    Q_INTERFACES(QIfServiceInterface)

public:
    explicit TunerSimulatorPlugin(QObject *parent = nullptr);

    QStringList interfaces() const override;
    QIfFeatureInterface *interfaceInstance(const QString &interface) const override;

private:
    AmFmTunerBackend *m_amfmtuner;
    SearchAndBrowseBackend *m_searchbackend;
};

#endif // TUNERPLUGIN_H
