/****************************************************************************
**
** Copyright (C) 2016 Pelagicore AG
** Contact: http://www.qt.io/ or http://www.pelagicore.com/
**
** This file is part of the QtIVI module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL3-PELAGICORE$
** Commercial License Usage
** Licensees holding valid commercial Qt IVI licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Pelagicore. For licensing terms
** and conditions, contact us at http://www.pelagicore.com.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPLv3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** $QT_END_LICENSE$
**
** SPDX-License-Identifier: LGPL-3.0
**
****************************************************************************/

#ifndef SIMPLEPLUGIN_H
#define SIMPLEPLUGIN_H

#include <QtCore/QObject>
#include <QtCore/QStringList>
#include <QtIVICore/QtIVIServiceInterface>

class SimplePlugin : public QObject, public QtIVIServiceInterface
{
    Q_OBJECT
    Q_INTERFACES(QtIVIServiceInterface)
    Q_PLUGIN_METADATA(IID "com.pelagicore.QtIVIServiceInterface" FILE "simple_plugin.json")
public:
    SimplePlugin();
    virtual ~SimplePlugin() {}

    QStringList interfaces() const {
        return QStringList() << "simple_plugin";
    }

    QObject *interfaceInstance(const QString &interface) const {
        Q_UNUSED(interface)
        return 0;
    }

};

#endif // SIMPLEPLUGIN_H
