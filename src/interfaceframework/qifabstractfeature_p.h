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

QT_BEGIN_NAMESPACE

class Q_QTINTERFACEFRAMEWORK_EXPORT QIfPropertyOverrider {
public:
    QIfPropertyOverrider() = default;
    virtual ~QIfPropertyOverrider() = default;

    virtual QVariant property(int propertyIndex) const = 0;
    virtual void setProperty(int propertyIndex, const QVariant &value) = 0;
    virtual bool isOverridden(int propertyIndex) const = 0;
private:
    Q_DISABLE_COPY(QIfPropertyOverrider)
};

class Q_QTINTERFACEFRAMEWORK_EXPORT QIfAbstractFeaturePrivate : public QObjectPrivate
{
public:
    QIfAbstractFeaturePrivate(const QString &interface, QIfAbstractFeature *parent);

    static QIfAbstractFeaturePrivate *get(QIfAbstractFeature *q);

    virtual void initialize();
    virtual bool notify(const QByteArray &propertyName, const QVariant &value);

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

    QIfAbstractFeature * const q_ptr;
    Q_DECLARE_PUBLIC(QIfAbstractFeature)
    Q_DISABLE_COPY(QIfAbstractFeaturePrivate)

    QString m_interface;
    QIfServiceObject *m_serviceObject;
    QIfAbstractFeature::DiscoveryMode m_discoveryMode;
    QIfAbstractFeature::DiscoveryResult m_discoveryResult;
    QString m_errorMessage;
    QIfAbstractFeature::Error m_error;
    bool m_qmlCreation;
    bool m_isInitialized;
    bool m_isConnected;

    bool m_supportsPropertyOverriding;
    QIfPropertyOverrider *m_propertyOverride;
};

QT_END_NAMESPACE

#endif // QIFABSTRACTFEATURE_P_H
