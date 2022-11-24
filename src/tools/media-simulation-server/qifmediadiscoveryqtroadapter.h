/****************************************************************************
**
** Copyright (C) 2021 The Qt Company Ltd.
** Copyright (C) 2019 Luxoft Sweden AB
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtInterfaceFramework module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or (at your option) the GNU General
** Public license version 3 or any later version approved by the KDE Free
** Qt Foundation. The licenses are as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-2.0.html and
** https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QIFMEDIADISCOVERYMODELQTROADAPTER_H
#define QIFMEDIADISCOVERYMODELQTROADAPTER_H

#include "mediadiscoverybackend.h"
#include "rep_qifmediadiscoverymodel_source.h"

#include <QRemoteObjectRegistryHost>

template <class ObjectType>
struct QIfMediaDiscoveryModelAddressWrapper: public QIfMediaDiscoveryModelSourceAPI<ObjectType> {
    QIfMediaDiscoveryModelAddressWrapper(ObjectType *object)
        : QIfMediaDiscoveryModelSourceAPI<ObjectType>(object, object->remoteObjectsLookupName())
    {}
};

class QIfMediaDiscoveryModelQtRoAdapter : public QIfMediaDiscoveryModelSource
{
public:
    QIfMediaDiscoveryModelQtRoAdapter(MediaDiscoveryBackend *parent);
    QIfMediaDiscoveryModelQtRoAdapter(const QString& remoteObjectsLookupName, MediaDiscoveryBackend *parent);

    QString remoteObjectsLookupName() const;

public Q_SLOTS:
    QStringList devices() const override;
    void onDeviceAdded(QIfServiceObject *device);
    void onDeviceRemoved(QIfServiceObject *device);

private:
    void createDeviceAdapter(QIfMediaDevice *device);

    QString m_remoteObjectsLookupName;
    MediaDiscoveryBackend *m_backend;
    QHash<QString, QObject*> m_hostMap;
};

#endif // QIFMEDIAINDEXERQTROADAPTER_H
