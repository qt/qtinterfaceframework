// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

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

    Q_PROPERTY(quint64 id READ id WRITE setId FINAL)
    Q_PROPERTY(bool failed READ failed WRITE setFailed FINAL)
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
