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


#ifndef MEDIAPLUGIN_H
#define MEDIAPLUGIN_H

#include <QtInterfaceFramework/QIfServiceInterface>

#include <QSqlDatabase>

class MediaPlayerBackend;
class SearchAndBrowseBackend;
class MediaDiscoveryBackend;
class MediaIndexerBackend;
class AmFmTunerBackend;

class MediaSimulatorPlugin : public QObject, QIfServiceInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID QIfServiceInterface_iid FILE "media_simulator.json")
    Q_INTERFACES(QIfServiceInterface)

public:
    explicit MediaSimulatorPlugin(QObject *parent = nullptr);

    QStringList interfaces() const override;
    QIfFeatureInterface *interfaceInstance(const QString &interface) const override;

private:

    MediaPlayerBackend *m_player;
    SearchAndBrowseBackend *m_browse;
    MediaDiscoveryBackend *m_discovery;
    MediaIndexerBackend *m_indexer;
};

#endif // MEDIAPLUGIN_H
