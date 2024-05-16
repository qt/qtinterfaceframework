// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QIFABSTRACTFEATURE_P_H
#define QIFABSTRACTFEATURE_P_H

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

#include <private/qobject_p.h>
#include <private/qtifglobal_p.h>

#include "qifabstractfeature.h"
#include "qiffeatureinterface.h"
#include "qifserviceobject.h"
#include "qifservicemanager.h"

QT_BEGIN_NAMESPACE

class Q_QTINTERFACEFRAMEWORK_EXPORT QIfAbstractFeaturePrivate : public QObjectPrivate
{
public:
    QIfAbstractFeaturePrivate(const QString &interface, QIfAbstractFeature *parent);

    static QIfAbstractFeaturePrivate *get(QIfAbstractFeature *q);

    virtual void initialize();

    QIfFeatureInterface *backend() const;
    template <class T> T backend() const
    {
        Q_Q(const QIfAbstractFeature);
        if (m_serviceObject)
            return m_serviceObject->interfaceInstance<T>(q->interfaceName());
        return nullptr;
    }

    void setDiscoveryResult(QIfAbstractFeature::DiscoveryResult discoveryResult);
    void onInitializationDone();
    void serviceObjectDestroyed();
    void loadServiceObject(QIfServiceManager::SearchFlag searchFlag);
    void onServiceObjectLoaded(QIfServiceObjectHandle handle);
    void onServiceObjectFailure();

    QIfAbstractFeature * const q_ptr;
    Q_DECLARE_PUBLIC(QIfAbstractFeature)
    Q_DISABLE_COPY(QIfAbstractFeaturePrivate)

    QString m_interface;
    QIfServiceObject *m_serviceObject;
    QIfAbstractFeature::DiscoveryMode m_discoveryMode;
    QIfAbstractFeature::DiscoveryResult m_discoveryResult;
    QString m_configurationId;
    QStringList m_preferredBackends;
    bool m_backendUpdatesEnabled;
    bool m_asynchronousBackendLoading;
    QList<QIfServiceObjectHandle> m_serviceHandles;
    int m_currentServiceHandleIndex;
    QIfServiceManager::SearchFlag m_currentSearch;
    QString m_errorMessage;
    QIfAbstractFeature::Error m_error;
    bool m_qmlCreation;
    bool m_isInitialized;
    bool m_isConnected;
};

QT_END_NAMESPACE

#endif // QIFABSTRACTFEATURE_P_H
