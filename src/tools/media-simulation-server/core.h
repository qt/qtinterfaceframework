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


#ifndef CORE_H
#define CORE_H

#include <QRemoteObjectNode>

QT_BEGIN_NAMESPACE

class Core : public QObject
{
    Q_OBJECT
private:
    Core(QObject *parent=nullptr);

public:
    virtual ~Core();
    void init();
    static Core* instance();
    QRemoteObjectRegistryHost* host() const;

private:
    void reportError(QRemoteObjectNode::ErrorCode code);

    static Core* s_instance;
    QRemoteObjectRegistryHost* m_host;
};

QT_END_NAMESPACE

#endif
