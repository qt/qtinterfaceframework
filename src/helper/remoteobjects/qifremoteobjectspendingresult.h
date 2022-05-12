/****************************************************************************
**
** Copyright (C) 2022 The Qt Company Ltd.
** Copyright (C) 2019 Luxoft Sweden AB
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

#ifndef QIFREMOTEOBJECTSPENDINGRESULT_H
#define QIFREMOTEOBJECTSPENDINGRESULT_H

#include <QtCore/QDataStream>
#include <QtCore/QObject>
#include <QtRemoteObjects/qtremoteobjectglobal.h>

QT_BEGIN_NAMESPACE

namespace qtif_private {
    Q_DECLARE_LOGGING_CATEGORY(qLcQtIfRoHelper)
}

class QIfRemoteObjectsPendingResult
{
    Q_GADGET

    Q_PROPERTY(quint64 id READ id WRITE setId)
    Q_PROPERTY(bool failed READ failed WRITE setFailed)
public:
    QIfRemoteObjectsPendingResult();
    explicit QIfRemoteObjectsPendingResult(quint64 id, bool failed);
    quint64 id() const;
    void setId(quint64 id);
    bool failed() const;
    void setFailed(bool failed);
private:
    quint64 m_id;
    bool m_failed;
};

//POD, passing by value should be fine
inline bool operator==(QIfRemoteObjectsPendingResult left, QIfRemoteObjectsPendingResult right) Q_DECL_NOTHROW {
    return left.id() == right.id() && left.failed() == right.failed();
}
inline bool operator!=(QIfRemoteObjectsPendingResult left, QIfRemoteObjectsPendingResult right) Q_DECL_NOTHROW {
    return !(left == right);
}

inline QDataStream &operator<<(QDataStream &ds, QIfRemoteObjectsPendingResult obj) {
    QtRemoteObjects::copyStoredProperties(&obj, ds);
    return ds;
}

inline QDataStream &operator>>(QDataStream &ds, QIfRemoteObjectsPendingResult &obj) {
    QtRemoteObjects::copyStoredProperties(ds, &obj);
    return ds;
}

QT_END_NAMESPACE

#endif //QIFREMOTEOBJECTSPENDINGRESULT_H
