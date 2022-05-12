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
