/****************************************************************************
**
** Copyright (C) 2015 Pelagicore AG
** Contact: http://www.pelagicore.com/
**
** This file is part of Qt IVI.
**
** $QT_BEGIN_LICENSE:LGPL3$
** Commercial License Usage
** Licensees holding valid commercial Qt IVI licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Pelagicore. For licensing terms
** and conditions see http://www.pelagicore.com.
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
****************************************************************************/

#ifndef QTIVISERVICEMANAGER_P_H
#define QTIVISERVICEMANAGER_P_H

#include <QAbstractListModel>
#include <QVariantMap>
#include <QStringList>
#include <QMap>
#include <QSet>

class QPluginLoader;
class QtIVIServiceInterface;
class QtIVIServiceObject;
class QtIVIServiceManager;

struct Backend{
    QVariantMap metaData;
    QtIVIServiceInterface *interface;
    QObject *interfaceObject;
    QPluginLoader *loader;
};

class QtIVIServiceManagerPrivate : public QObject
{
    Q_OBJECT

public:
    explicit QtIVIServiceManagerPrivate(QtIVIServiceManager *parent);

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

signals:
    void beginInsertRows(const QModelIndex &index, int start, int end);
    void endInsertRows();

};

#endif // QTIVISERVICEMANAGER_P_H

