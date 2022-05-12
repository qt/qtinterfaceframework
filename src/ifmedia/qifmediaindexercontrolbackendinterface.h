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
