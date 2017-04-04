/****************************************************************************
**
** Copyright (C) 2017 Pelagicore AG
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtIvi module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL-QTAS$
** Commercial License Usage
** Licensees holding valid commercial Qt Automotive Suite licenses may use
** this file in accordance with the commercial license agreement provided
** with the Software or, alternatively, in accordance with the terms
** contained in a written agreement between you and The Qt Company.  For
** licensing terms and conditions see https://www.qt.io/terms-conditions.
** For further information use the contact form at https://www.qt.io/contact-us.
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
** SPDX-License-Identifier: LGPL-3.0
**
****************************************************************************/

#include "usbbrowsebackend.h"

#include <QDir>
#include <QtDebug>

UsbBrowseBackend::UsbBrowseBackend(const QString &path, QObject *parent)
    : QIviSearchAndBrowseModelInterface(parent)
    , m_rootFolder(path)
{
    qRegisterMetaType<SearchAndBrowseItem>();
    registerContentType<SearchAndBrowseItem>("file");
}

void UsbBrowseBackend::initialize()
{
    emit initializationDone();
}

void UsbBrowseBackend::fetchData(const QUuid &identifier, const QString &type, QIviAbstractQueryTerm *term, const QList<QIviOrderTerm> &orderTerms, int start, int count)
{
    emit supportedCapabilitiesChanged(identifier, QIviSearchAndBrowseModel::Capabilities(
                                          QIviSearchAndBrowseModel::SupportsSorting |
                                          QIviSearchAndBrowseModel::SupportsStatelessNavigation |
                                          QIviSearchAndBrowseModel::SupportsGetSize
                                          ));

    Q_UNUSED(term);
    Q_UNUSED(orderTerms);

    QVariantList list;
    QString folder = m_rootFolder;
    if (type != "file")
        folder += type;
    QDir dir(folder);
    QFileInfoList infoList = dir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot | QDir::NoSymLinks);

    emit countChanged(identifier, infoList.count());

    for (int i = start; i < infoList.count() && i < count; i++) {
        SearchAndBrowseItem item;
        item.setType("file");
        item.setName(infoList.at(i).fileName());
        list.append(QVariant::fromValue(item));
    }

    emit dataFetched(identifier, list, start, list.count() >= count);
}

bool UsbBrowseBackend::canGoBack(const QUuid &identifier, const QString &type)
{
    Q_UNUSED(identifier)
    return type != "file";
}

QString UsbBrowseBackend::goBack(const QUuid &identifier, const QString &type)
{
    Q_UNUSED(identifier)
    QStringList types = type.split('/');

    if (types.count() < 2 && type != "file")
        return "file";

    types.removeLast();
    types.replace(types.count() - 1, types.at(types.count() - 1).split('?').at(0));

    return types.join('/');
}

bool UsbBrowseBackend::canGoForward(const QUuid &identifier, const QString &type, const QString &itemId)
{
    Q_UNUSED(identifier);
    if (type != "file")
        return QDir(m_rootFolder + "/" + type + "/" + itemId).count();
    else
        return QDir(m_rootFolder + "/" + itemId).count();
}

QString UsbBrowseBackend::goForward(const QUuid &identifier, const QString &type, const QString &itemId)
{
    Q_UNUSED(identifier)
    if (type != "file")
        return type + "/" + itemId;
    else
        return itemId;
}

void UsbBrowseBackend::insert(const QUuid &identifier, const QString &type, int index, const QIviSearchAndBrowseModelItem *item)
{
    Q_UNUSED(identifier)
    Q_UNUSED(type)
    Q_UNUSED(index)
    Q_UNUSED(item)
}

void UsbBrowseBackend::remove(const QUuid &identifier, const QString &type, int index)
{
    Q_UNUSED(identifier)
    Q_UNUSED(type)
    Q_UNUSED(index)
}

void UsbBrowseBackend::move(const QUuid &identifier, const QString &type, int currentIndex, int newIndex)
{
    Q_UNUSED(identifier)
    Q_UNUSED(type)
    Q_UNUSED(currentIndex)
    Q_UNUSED(newIndex)
}

int UsbBrowseBackend::indexOf(const QUuid &identifier, const QString &type, const QIviSearchAndBrowseModelItem *item)
{
    Q_UNUSED(identifier)
    Q_UNUSED(type)
    Q_UNUSED(item)

    return -1;
}
