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

#include "qifmediaindexercontrolbackendinterface.h"

QT_BEGIN_NAMESPACE

/*!
    \class QIfMediaIndexerControlBackendInterface
    \inmodule QtIfMedia
    \ingroup backends
    \inherits QObject
    \keyword org.qt-project.interfaceframework.MediaIndexer/1.0
    \brief The QIfMediaIndexerControlBackendInterface defines the interface for backends to the
    QIfMediaIndexerControl feature class.

    The QIfMediaIndexerControlBackendInterface is the interface used by \l QIfMediaIndexerControl

    The interface is discovered by a \l QIfMediaIndexerControl object, which connects to it and sets it up.

    <example of a fully featured backend>
*/

/*!
    Constructs a backend interface.

    The \a parent is sent to the QObject constructor.
*/
QIfMediaIndexerControlBackendInterface::QIfMediaIndexerControlBackendInterface(QObject *parent)
    : QIfFeatureInterface(parent)
{

}

/*!
    \fn QIfMediaIndexerControlBackendInterface::pause()

    Pauses the currently ongoing indexing. If currently no indexing is ongoing this call should not perform any actions.

    \sa resume()
*/

/*!
    \fn QIfMediaIndexerControlBackendInterface::resume()

    Resumes the previously paused indexing. If there is nothing to resume this call should not perform any actions.

    \sa pause()
*/

/*!
    \fn void QIfMediaIndexerControlBackendInterface::progressChanged(qreal progress)

    This signal is emitted when indexing is currently ongoing and the progress of the operation changed.
    The \a progress argument passes the current progress between \e 0 and \e 1.
*/

/*!
    \fn void QIfMediaIndexerControlBackendInterface::stateChanged(QIfMediaIndexerControl::State state)

    This signal is emitted whenever the indexing state changed. The \a state argument holds the new state
    of the indexer.
*/

QT_END_NAMESPACE
