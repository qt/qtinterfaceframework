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

#ifndef QIFSEARCHANDBROWSEMODEL_H
#define QIFSEARCHANDBROWSEMODEL_H

#include <QtInterfaceFramework/QIfPagingModel>
#include <QtInterfaceFramework/QtInterfaceFrameworkModule>
#include <QtInterfaceFramework/QIfPendingReply>
#include <QtInterfaceFramework/QIfServiceObject>
#include <QtQml/QJSValue>

QT_BEGIN_NAMESPACE

class QIfFilterAndBrowseModelPrivate;

class Q_QTINTERFACEFRAMEWORK_EXPORT QIfFilterAndBrowseModel : public QIfPagingModel
{
    Q_OBJECT

    Q_PROPERTY(QString query READ query WRITE setQuery NOTIFY queryChanged)
    Q_PROPERTY(QString contentType READ contentType WRITE setContentType NOTIFY contentTypeChanged)
    Q_PROPERTY(QStringList availableContentTypes READ availableContentTypes NOTIFY availableContentTypesChanged)
    Q_PROPERTY(bool canGoBack READ canGoBack NOTIFY canGoBackChanged)

public:

    enum Roles {
        CanGoForwardRole = QIfPagingModel::LastRole + 1,
        LastRole = CanGoForwardRole
    };

    //TODO fix naming
    enum NavigationType {
        InModelNavigation,
        OutOfModelNavigation
    };
    Q_ENUM(NavigationType)

    explicit QIfFilterAndBrowseModel(QObject *parent = nullptr);

    QString query() const;
    void setQuery(const QString &query);

    QString contentType() const;
    void setContentType(const QString &contentType);

    QStringList availableContentTypes() const;

    bool canGoBack() const;

    QVariant data(const QModelIndex &index, int role) const override;

    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void goBack();
    Q_INVOKABLE bool canGoForward(int index) const;
    Q_INVOKABLE QIfFilterAndBrowseModel *goForward(int index, QIfFilterAndBrowseModel::NavigationType navigationType);
    Q_INVOKABLE QIfPendingReply<void> insert(int index, const QVariant &variant);
    Q_INVOKABLE QIfPendingReply<void> remove(int index);
    Q_INVOKABLE QIfPendingReply<void> move(int cur_index, int new_index);
    Q_INVOKABLE QIfPendingReply<int> indexOf(const QVariant &variant);

Q_SIGNALS:
    void queryChanged(const QString &query);
    void contentTypeChanged(const QString &contentType);
    void availableContentTypesChanged(const QStringList &availableContentTypes);
    void canGoBackChanged(bool canGoBack);

protected:
    QIfFilterAndBrowseModel(QIfServiceObject *serviceObject, QObject *parent = nullptr);
    QIfFilterAndBrowseModel(QIfFilterAndBrowseModelPrivate &dd, QObject *parent);
    void connectToServiceObject(QIfServiceObject *serviceObject) override;
    void clearServiceObject() override;

private:
    Q_DECLARE_PRIVATE(QIfFilterAndBrowseModel)
    Q_PRIVATE_SLOT(d_func(), void onCanGoForwardChanged(const QUuid &identifier, const QVector<bool> &indexes, int start))
    Q_PRIVATE_SLOT(d_func(), void onCanGoBackChanged(const QUuid &identifier, bool canGoBack))
    Q_PRIVATE_SLOT(d_func(), void onQueryIdentifiersChanged(const QUuid &identifier, const QSet<QString> &queryIdentifiers))
    Q_PRIVATE_SLOT(d_func(), void onContentTypeChanged(const QUuid &identifier, const QString &contentType))
    Q_PRIVATE_SLOT(d_func(), void onAvailableContentTypesChanged(const QStringList &contentTypes))
};

QT_END_NAMESPACE

#endif // QIFSEARCHANDBROWSEMODEL_H
