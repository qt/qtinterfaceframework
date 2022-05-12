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

#ifndef QIFSERVICEINTERFACE_H
#define QIFSERVICEINTERFACE_H

#include <QtInterfaceFramework/qiffeatureinterface.h>
#include <QtInterfaceFramework/qtifglobal.h>

#include <QtCore/QtPlugin>

QT_BEGIN_NAMESPACE

template <class T> T qif_interface_cast(QObject *backend)
{
    T inst = qobject_cast<T>(backend);
    static bool showOnce = true;
    if (!inst && showOnce) {
        typedef typename std::remove_cv<typename std::remove_pointer<T>::type>::type ObjType;
        qCritical("Casting the backend to the interface %s failed.\n"
                  "Either the backend pointer is not of the correct type or casting failed because "
                  "debug and release libraries were mixed.", ObjType::staticMetaObject.className());
        showOnce = false;
    }
    return inst;
}

class Q_QTINTERFACEFRAMEWORK_EXPORT QIfServiceInterface
{
public:
    virtual ~QIfServiceInterface();

    virtual QStringList interfaces() const = 0;
    virtual QIfFeatureInterface *interfaceInstance(const QString &interfaceName) const = 0;

    template <class T> T interfaceInstance(const QString &interfaceName) const {
        T inst = qif_interface_cast<T>(interfaceInstance(interfaceName));
        return inst;
    }
};

#define QIfServiceInterface_iid "org.qt-project.interfaceframework.QIfServiceInterface/1.0"
Q_DECLARE_INTERFACE(QIfServiceInterface, QIfServiceInterface_iid)

QT_END_NAMESPACE

Q_DECLARE_METATYPE(QIfServiceInterface*)

#endif // QIFSERVICEINTERFACE_H

