/****************************************************************************
**
** Copyright (C) 2022 The Qt Company Ltd.
** Copyright (C) 2019 Luxoft Sweden AB
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

class MediaPlayerBackend;
class MediaIndexerBackend;
class FilterAndBrowseModel;
class MediaDiscoveryBackend;

class MediaQtROPlugin : public QObject, QIfServiceInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID QIfServiceInterface_iid FILE "media_qtro.json")
    Q_INTERFACES(QIfServiceInterface)

public:
    explicit MediaQtROPlugin(QObject *parent = nullptr);

    QStringList interfaces() const override;
    QIfFeatureInterface *interfaceInstance(const QString &interface) const override;

private:

    MediaPlayerBackend *m_player;
    MediaIndexerBackend *m_indexer;
    FilterAndBrowseModel *m_searchModel;
    MediaDiscoveryBackend *m_discovery;
};

#endif // MEDIAPLUGIN_H
