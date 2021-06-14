/****************************************************************************
**
** Copyright (C) 2021 The Qt Company Ltd.
** Copyright (C) 2019 Luxoft Sweden AB
** Copyright (C) 2018 Pelagicore AG
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtInterfaceFramework module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or (at your option) the GNU General
** Public license version 3 or any later version approved by the KDE Free
** Qt Foundation. The licenses are as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-2.0.html and
** https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qifmediaindexercontrol.h"
#include "qifmediaindexercontrol_p.h"

#include <QIfServiceObject>
#include <QtDebug>
#include <QDataStream>
#include <QMetaEnum>

QT_BEGIN_NAMESPACE

QIfMediaIndexerControlPrivate::QIfMediaIndexerControlPrivate(const QString &interface, QIfMediaIndexerControl *parent)
    : QIfAbstractFeaturePrivate(interface, parent)
    , q_ptr(parent)
    , m_progress(0)
    , m_state(QIfMediaIndexerControl::Idle)
{
}

void QIfMediaIndexerControlPrivate::clearToDefaults()
{
    onProgressChanged(0);
    onStateChanged(QIfMediaIndexerControl::Idle);
}

void QIfMediaIndexerControlPrivate::onProgressChanged(qreal progress)
{
    if (qFuzzyCompare(m_progress, progress))
        return;

    Q_Q(QIfMediaIndexerControl);
    m_progress = progress;
    emit q->progressChanged(progress);
}

void QIfMediaIndexerControlPrivate::onStateChanged(QIfMediaIndexerControl::State state)
{
    if (m_state == state)
        return;

    Q_Q(QIfMediaIndexerControl);
    m_state = state;
    emit q->stateChanged(state);
}

QIfMediaIndexerControlBackendInterface *QIfMediaIndexerControlPrivate::indexerBackend() const
{
    return backend<QIfMediaIndexerControlBackendInterface*>();
}

/*!
    \class QIfMediaIndexerControl
    \inmodule QtIfMedia
    \brief Provides an interface to control the media indexer.

    The QIfMediaIndexerControl controls the media indexer. It provides a way to temporarily pause the indexing
    and resume it, as well as to inquire about the current state and progress of the indexing operation.

    The QIfMediaIndexerControl expects a single backend to be available. It is recommended to use it
    with \l {QIfAbstractFeature::}{discoveryMode} set to \l QIfAbstractFeature::AutoDiscovery.
*/

/*!
    \qmltype MediaIndexerControl
    \instantiates QIfMediaIndexerControl
    \inqmlmodule QtInterfaceFramework.Media
    \inherits AbstractFeature
    \brief Provides an interface to control the media indexer.

    The MediaIndexerControl controls the media indexer. It provides a way to temporarily pause the indexing
    and resume it, as well as to inquire about the current state and progress of the indexing operation.

    The MediaIndexerControl expects a single backend to be available. It is recommended to use it
    with \l {AbstractFeature::}{discoveryMode} set to AbstractFeature.AutoDiscovery.
*/

/*!
    \enum QIfMediaIndexerControl::State
    \value Idle
           The indexer is currently idle and is waiting for new files to be indexed.
    \value Active
           The indexer is currently busy with indexing.
    \value Paused
           The indexer is paused, due to a call to pause().
    \value Error
           An error has occurred during the indexing operation.
*/

/*!
    Constructs a QIfMediaIndexerControl.

    The \a parent argument is passed on to the \l QIfAbstractFeature base class.
*/
QIfMediaIndexerControl::QIfMediaIndexerControl(QObject *parent)
    : QIfAbstractFeature(*new QIfMediaIndexerControlPrivate(QLatin1String(QIfMediaIndexer_iid), this), parent)
{
    qRegisterMetaType<QIfMediaIndexerControl::State>();
}

/*!
    \qmlproperty real MediaIndexerControl::progress
    \brief Holds the progress of the indexing operation.

    The value is between \e 0 and \e 1.
*/
/*!
    \property QIfMediaIndexerControl::progress
    \brief Holds the progress of the indexing operation.

    The value is between \e 0 and \e 1.
*/
qreal QIfMediaIndexerControl::progress() const
{
    Q_D(const QIfMediaIndexerControl);
    return d->m_progress;
}

/*!
    \qmlproperty enumeration MediaIndexerControl::state
    \brief Holds the current state of the indexer.

    It can be one of the following values:
    \value Idle
           The indexer is currently idle and is waiting for new files to be indexed.
    \value Active
           The indexer is currently busy with indexing.
    \value Paused
           The indexer is paused, due to a call to pause().
    \value Error
           An error has occurred during the indexing operation.
*/
/*!
    \property QIfMediaIndexerControl::state
    \brief Holds the current state of the indexer.
*/
QIfMediaIndexerControl::State QIfMediaIndexerControl::state() const
{
    Q_D(const QIfMediaIndexerControl);
    return d->m_state;
}

/*!
    \qmlmethod MediaIndexerControl::pause()

    Pauses the currently ongoing indexing operation.

    \sa resume() state
*/

/*!
    \fn void QIfMediaIndexerControl::pause()

    Pauses the currently ongoing indexing operation.

    \sa resume() state
*/
void QIfMediaIndexerControl::pause()
{
    Q_IF_BACKEND(QIfMediaIndexerControl, d->indexerBackend(), "Can't pause without a connected backend");

    backend->pause();
}

/*!
    \qmlmethod MediaIndexerControl::resume()

    Resumes from the Paused state and resumes the indexing operation.

    \sa pause() state
*/

/*!
    \fn void QIfMediaIndexerControl::resume()

    Resumes from the Paused state and resumes the indexing operation.

    \sa pause() state
*/
void QIfMediaIndexerControl::resume()
{
    Q_IF_BACKEND(QIfMediaIndexerControl, d->indexerBackend(), "Can't resume without a connected backend");

    backend->resume();
}

/*!
    \internal
*/
QIfMediaIndexerControl::QIfMediaIndexerControl(QIfMediaIndexerControlPrivate &dd, QObject *parent)
    : QIfAbstractFeature(dd, parent)
{
}

/*!
    \reimp
*/
void QIfMediaIndexerControl::connectToServiceObject(QIfServiceObject *serviceObject)
{
    Q_UNUSED(serviceObject);

    Q_D(QIfMediaIndexerControl);

    QIfMediaIndexerControlBackendInterface *backend = d->indexerBackend();
    if (!backend)
        return;

    QObjectPrivate::connect(backend, &QIfMediaIndexerControlBackendInterface::progressChanged,
                            d, &QIfMediaIndexerControlPrivate::onProgressChanged);
    QObjectPrivate::connect(backend, &QIfMediaIndexerControlBackendInterface::stateChanged,
                            d, &QIfMediaIndexerControlPrivate::onStateChanged);

    QIfAbstractFeature::connectToServiceObject(serviceObject);

    backend->initialize();
}

/*!
    \reimp
*/
void QIfMediaIndexerControl::clearServiceObject()
{
    Q_D(QIfMediaIndexerControl);
    d->clearToDefaults();
}

QDataStream &operator <<(QDataStream &out, QIfMediaIndexerControl::State var)
{
    out << int(var);
    return out;
}

QDataStream &operator>>(QDataStream &in, QIfMediaIndexerControl::State &var)
{
    int val;
    in >> val;
    QMetaEnum metaEnum = QMetaEnum::fromType<QIfMediaIndexerControl::State>();
    if (metaEnum.valueToKey(val) == nullptr)
        qWarning() << "Received an invalid enum value for type QIfMediaIndexerControl::State, value =" << val;
    var = QIfMediaIndexerControl::State(val);
    return in;
}

QT_END_NAMESPACE

#include "moc_qifmediaindexercontrol.cpp"
