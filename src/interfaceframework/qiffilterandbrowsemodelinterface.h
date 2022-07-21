// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QIFSEARCHMODELINTERFACE_H
#define QIFSEARCHMODELINTERFACE_H

#include <QtCore/QMetaObject>
#include <QtCore/QMetaProperty>
#include <QtCore/QObject>
#include <QtCore/QSet>
#include <QtCore/QUuid>
#include <QtInterfaceFramework/QIfAbstractQueryTerm>
#include <QtInterfaceFramework/QtInterfaceFrameworkModule>
#include <QtInterfaceFramework/QIfPagingModelInterface>
#include <QtInterfaceFramework/QIfFilterAndBrowseModel>
#include <QtInterfaceFramework/QIfStandardItem>
#include <QtInterfaceFramework/qifqmlconversion_helper.h>

QT_BEGIN_NAMESPACE

class QIfFilterAndBrowseModelInterfacePrivate;

class Q_QTINTERFACEFRAMEWORK_EXPORT QIfFilterAndBrowseModelInterface : public QIfPagingModelInterface
{
    Q_OBJECT

public:
    explicit QIfFilterAndBrowseModelInterface(QObject *parent = nullptr);

    virtual void setContentType(const QUuid &identifier, const QString &contentType) = 0;
    virtual void setupFilter(const QUuid &identifier, QIfAbstractQueryTerm *term, const QList<QIfOrderTerm> &orderTerms) = 0;

    virtual QIfPendingReply<QString> goBack(const QUuid &identifier) = 0;
    virtual QIfPendingReply<QString> goForward(const QUuid &identifier, int index) = 0;

    virtual QIfPendingReply<void> insert(const QUuid &identifier, int index, const QVariant &item) = 0;
    virtual QIfPendingReply<void> remove(const QUuid &identifier, int index) = 0;
    virtual QIfPendingReply<void> move(const QUuid &identifier, int currentIndex, int newIndex) = 0;
    virtual QIfPendingReply<int> indexOf(const QUuid &identifier, const QVariant &item) = 0;

Q_SIGNALS:
    void canGoForwardChanged(const QUuid &identifier, const QVector<bool> &indexes, int start);
    void canGoBackChanged(const QUuid &identifier, bool canGoBack);
    //does this really make sense ?
    void contentTypeChanged(const QUuid &identifier, const QString &contentType);
    void availableContentTypesChanged(const QStringList &availableContentTypes);
    void queryIdentifiersChanged(const QUuid &identifier, const QSet<QString> &queryIdentifiers);

protected:
    template <typename T>
    QSet<QString> identifiersFromItem() {
        return identifiersFromItem(T::staticMetaObject);
    }
    QSet<QString> identifiersFromItem(const QMetaObject &object);

private:
    Q_DECLARE_PRIVATE(QIfFilterAndBrowseModelInterface)
};

#define QIfFilterAndBrowseModel_iid "org.qt-project.interfaceframework.FilterAndBrowseModel/1.0"

QT_END_NAMESPACE

#endif // QIFSEARCHMODELINTERFACE_H
