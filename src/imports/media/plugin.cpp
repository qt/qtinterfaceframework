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

#include <QtQml/qqmlextensionplugin.h>
#include <qqml.h>

#include <QtIfMedia/QIfAmFmTuner>
#include <QtIfMedia/QIfMediaDevice>
#include <QtIfMedia/QIfMediaDeviceDiscoveryModel>
#include <QtIfMedia/QIfMediaIndexerControl>
#include <QtIfMedia/QIfMediaPlayer>
#include <QtIfMedia/QIfPlayQueue>
#include <QtIfMedia/QIfTunerStation>

QT_BEGIN_NAMESPACE

class QIfMediaPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID QQmlExtensionInterface_iid)
public:
    void registerTypes(const char *uri) override
    {
        Q_ASSERT(QLatin1String(uri) == QLatin1String("QtInterfaceFramework.Media"));
        Q_UNUSED(uri);

        qRegisterMetaType<QIfTunerStation>();
        qRegisterMetaType<QIfAmFmTunerStation>();

        qmlRegisterType<QIfMediaPlayer>(uri, 1, 0, "MediaPlayer");
        //This should be an singleton, otherwise we might delete a pointer twice ?
        qmlRegisterType<QIfMediaDeviceDiscoveryModel>(uri, 1, 0, "MediaDeviceDiscoveryModel");
        qmlRegisterType<QIfMediaIndexerControl>(uri, 1, 0, "MediaIndexerControl");
        qmlRegisterType<QIfAmFmTuner>(uri, 1, 0, "AmFmTuner");

        qmlRegisterUncreatableType<QIfPlayQueue>(uri, 1, 0, "PlayQueue", QStringLiteral("PlayQueue needs to be retrieved from the MediaPlayer"));

        qmlRegisterUncreatableType<QIfMediaDevice>(uri, 1, 0, "MediaDevice", QStringLiteral("MediaDevice can't be instantiated from QML"));
        qmlRegisterUncreatableType<QIfMediaUsbDevice>(uri, 1, 0, "MediaUsbDevice", QStringLiteral("MediaUsbDevice can't be instantiated from QML"));
    }
};

QT_END_NAMESPACE

#include "plugin.moc"
