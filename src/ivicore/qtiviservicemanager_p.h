/****************************************************************************
**
** Copyright (C) 2015 Pelagicore AG
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

#ifndef QTIVISERVICEMANAGER_P_H
#define QTIVISERVICEMANAGER_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail. This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <QtCore/QAbstractListModel>
#include <QtCore/QVariantMap>
#include <QtCore/QStringList>
#include <QtCore/QMap>
#include <QtCore/QSet>

#include <QtIVICore/qtiviglobal.h>

QT_BEGIN_NAMESPACE

class QPluginLoader;
class QtIVIServiceInterface;
class QtIVIServiceObject;
class QtIVIServiceManager;
class QtIVIProxyServiceObject;

struct Backend{
    QVariantMap metaData;
    QtIVIServiceInterface *interface;
    QObject *interfaceObject;
    QtIVIProxyServiceObject *proxyServiceObject;
    QPluginLoader *loader;
};

class Q_QTIVICORE_EXPORT QtIVIServiceManagerPrivate : public QObject
{
    Q_OBJECT

public:
    explicit QtIVIServiceManagerPrivate(QtIVIServiceManager *parent);

    static QtIVIServiceManagerPrivate* get(QtIVIServiceManager *serviceManager);

    QList<QtIVIServiceObject*> findServiceByInterface(const QString &interface);

    void searchPlugins();
    void registerBackend(const QString fileName, const QJsonObject metaData);
    bool registerBackend(QObject *serviceBackendInterface, const QStringList &interfaces);
    void addBackend(struct Backend *backend);

    void unloadAllBackends();

    QtIVIServiceInterface *loadServiceBackendInterface(struct Backend *backend);

    QList<Backend*> m_backends;
    QSet<QString> m_interfaceNames;

    QtIVIServiceManager * const q_ptr;
    Q_DECLARE_PUBLIC(QtIVIServiceManager)

Q_SIGNALS:
    void beginInsertRows(const QModelIndex &index, int start, int end);
    void endInsertRows();

};

QT_END_NAMESPACE

#endif // QTIVISERVICEMANAGER_P_H

