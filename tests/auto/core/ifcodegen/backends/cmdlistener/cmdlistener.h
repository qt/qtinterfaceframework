// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0

#ifndef CMDLISTENER_H
#define CMDLISTENER_H

#include <cmdlistener_export.h>
#include <QtCore/QObject>
#include <QtQml>

class CMDLISTENER_EXPORT CmdListener : public QObject
{
    Q_OBJECT
    QML_ELEMENT

public:
    explicit CmdListener(QObject *parent = nullptr);

Q_SIGNALS:
    void newCmd(const QString &cmd);
};

#endif // CMDLISTENER_H
