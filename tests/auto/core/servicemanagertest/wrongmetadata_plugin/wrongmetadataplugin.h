/****************************************************************************
**
** Copyright (C) 2016 Pelagicore AG
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtIVI module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL-EXCEPT-QTAS$
** Commercial License Usage
** Licensees holding valid commercial Qt Automotive Suite licenses may use
** this file in accordance with the commercial license agreement provided
** with the Software or, alternatively, in accordance with the terms
** contained in a written agreement between you and The Qt Company.  For
** licensing terms and conditions see https://www.qt.io/terms-conditions.
** For further information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 as published by the Free Software
** Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/


#ifndef WRONGMETADATAPLUGIN_H
#define WRONGMETADATAPLUGIN_H

#include <QtCore/QObject>
#include <QtCore/QStringList>
#include <QtIVICore/QtIVIServiceInterface>

class WrongMetadataPlugin : public QObject, public QtIVIServiceInterface
{
    Q_OBJECT
    Q_INTERFACES(QtIVIServiceInterface)
public:
    WrongMetadataPlugin();
    virtual ~WrongMetadataPlugin() {}

    QStringList interfaces() const {
        return QStringList() << "wrongmetadata";
    }

    QObject *interfaceInstance(const QString &interface) const {
        Q_UNUSED(interface)
        return 0;
    }

};

#endif // WRONGMETADATAPLUGIN_H
