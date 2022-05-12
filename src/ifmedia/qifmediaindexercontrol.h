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

#ifndef QIFMEDIAINDEXERCONTROL_H
#define QIFMEDIAINDEXERCONTROL_H

#include <QtInterfaceFramework/QIfAbstractFeature>
#include <QtIfMedia/qtifmediaglobal.h>

QT_BEGIN_NAMESPACE

class QIfMediaIndexerControlPrivate;

class Q_QTIFMEDIA_EXPORT QIfMediaIndexerControl : public QIfAbstractFeature
{
    Q_OBJECT

    Q_PROPERTY(qreal progress READ progress NOTIFY progressChanged)
    Q_PROPERTY(QIfMediaIndexerControl::State state READ state NOTIFY stateChanged)
public:

    enum State {
        Idle,
        Active,
        Paused,
        Error
    };
    Q_ENUM(State)

    explicit QIfMediaIndexerControl(QObject *parent = nullptr);

    qreal progress() const;
    QIfMediaIndexerControl::State state() const;

public Q_SLOTS:
    void pause();
    void resume();

Q_SIGNALS:
    void progressChanged(qreal progress);
    void stateChanged(QIfMediaIndexerControl::State state);

protected:
    QIfMediaIndexerControl(QIfMediaIndexerControlPrivate &dd, QObject *parent = nullptr);

    void connectToServiceObject(QIfServiceObject *serviceObject) override;
    void clearServiceObject() override;

private:
    Q_DECLARE_PRIVATE(QIfMediaIndexerControl)
    Q_PRIVATE_SLOT(d_func(), void onProgressChanged(qreal progress))
    Q_PRIVATE_SLOT(d_func(), void onStateChanged(QIfMediaIndexerControl::State state))
};

Q_QTIFMEDIA_EXPORT QDataStream &operator<<(QDataStream &out, QIfMediaIndexerControl::State var);
Q_QTIFMEDIA_EXPORT QDataStream &operator>>(QDataStream &in, QIfMediaIndexerControl::State &var);

QT_END_NAMESPACE

#endif // QIFMEDIAINDEXERCONTROL_H
