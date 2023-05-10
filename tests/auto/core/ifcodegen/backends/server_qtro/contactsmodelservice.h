// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0

#ifndef CONTACTSMODELSERVICE_H
#define CONTACTSMODELSERVICE_H

#include "contact.h"
#include <QtIfRemoteObjectsHelper/rep_qifpagingmodel_source.h>

class ContactsModelService : public QIfPagingModelSimpleSource
{
public:
    ContactsModelService(QObject* parent = nullptr);

    void registerInstance(const QUuid &identifier) override;
    void unregisterInstance(const QUuid &identifier) override;

    void fetchData(const QUuid &identifier, int start, int count) override;

public Q_SLOTS:
    void insert(int index, const Contact &item);
    void remove(int index);
    void move(int currentIndex, int newIndex);
    void reset();
    void update(int index, const Contact &item);
    const Contact &at(int index) const;

private:
    QList<Contact> m_list;
};

#endif // CONTACTSMODELSERVICE_H
