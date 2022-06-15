// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QIFMEDIAINDEXERCONTROLBACKENDINTERFACE_H
#define QIFMEDIAINDEXERCONTROLBACKENDINTERFACE_H

#include <QtInterfaceFramework/QIfFeatureInterface>
#include <QtIfMedia/qtifmediaglobal.h>
#include <QtIfMedia/QIfMediaIndexerControl>

QT_BEGIN_NAMESPACE

class Q_QTIFMEDIA_EXPORT QIfMediaIndexerControlBackendInterface : public QIfFeatureInterface
{
    Q_OBJECT
public:
    explicit QIfMediaIndexerControlBackendInterface(QObject *parent = nullptr);

    virtual void pause() = 0;
    virtual void resume() = 0;

Q_SIGNALS:
    void progressChanged(qreal progress);
    void stateChanged(QIfMediaIndexerControl::State state);
};

#define QIfMediaIndexer_iid "org.qt-project.interfaceframework.MediaIndexer/1.0"

QT_END_NAMESPACE

#endif // QIFMEDIAINDEXERCONTROLBACKENDINTERFACE_H
