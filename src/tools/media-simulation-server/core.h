// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only


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
