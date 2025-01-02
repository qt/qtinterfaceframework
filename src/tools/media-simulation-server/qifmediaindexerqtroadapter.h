// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QIFMEDIAINDEXERQTROADAPTER_H
#define QIFMEDIAINDEXERQTROADAPTER_H

#include "mediaindexerbackend.h"
#include "rep_qifmediaindexer_source.h"

template <class ObjectType>
struct QIfMediaIndexerAddressWrapper: public QIfMediaIndexerSourceAPI<ObjectType> {
    QIfMediaIndexerAddressWrapper(ObjectType *object)
        : QIfMediaIndexerSourceAPI<ObjectType>(object, object->remoteObjectsLookupName())
    {}
};

class QIfMediaIndexerQtRoAdapter : public QIfMediaIndexerSource
{
public:
    QIfMediaIndexerQtRoAdapter(MediaIndexerBackend *parent);
    QIfMediaIndexerQtRoAdapter(const QString& remoteObjectsLookupName, MediaIndexerBackend *parent);

    QString remoteObjectsLookupName() const;
    QIfMediaIndexerControl::State state() const override;
    qreal progress() const override;

public Q_SLOTS:
    QVariant pause() override;
    QVariant resume() override;

private:
    QString m_remoteObjectsLookupName;
    MediaIndexerBackend *m_backend;
};

#endif // QIFMEDIAINDEXERQTROADAPTER_H
