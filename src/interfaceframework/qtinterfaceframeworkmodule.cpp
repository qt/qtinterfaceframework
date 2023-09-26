// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qtinterfaceframeworkmodule.h"
#include "qifpendingreply.h"
#include "qifabstractfeature.h"
#include "qifabstractzonedfeature.h"
#include "qifservicemanager.h"
#include "qifserviceobject.h"
#include "qifpagingmodel.h"
#include "qiffilterandbrowsemodel.h"
#include "qifconfiguration.h"

#include <QQmlEngine>

/*! \internal */
QObject* qtinterfaceframeworkmodule_singletontype_provider(QQmlEngine*, QJSEngine*)
{
    return new QtInterfaceFrameworkModule();
}

/*!
    \class QtInterfaceFrameworkModule
    \inmodule QtInterfaceFramework

    \brief The QtInterfaceFrameworkModule class holds enums which are used by multiple classes of QtInterfaceFramework
    and provides convenience functions to register types to QML.
*/

/*!
    \qmltype QtInterfaceFramework
    \instantiates QtInterfaceFrameworkModule
    \inqmlmodule QtInterfaceFramework

    \brief The QtInterfaceFramework singleton holds enums, which are used by multiple Items of the QtInterfaceFramework module.

    The following enums are exported from this object:

    \section3 ModelCapability
    \value NoExtras
           The backend does only support the minimum feature set and is stateful.
    \value SupportsGetSize
           The backend can return the final number of items for a specific request. This makes it possible to support the QIfFilterAndBrowseModel::DataChanged loading
           type.
    \value SupportsFiltering
           The backend supports filtering of the content. QIfFilterAndBrowseModelInterface::availableContentTypesChanged() and QIfFilterAndBrowseModelInterface::queryIdentifiersChanged() will be used as input for the
           \l {Qt Interface Framework Query Language}.
    \value SupportsSorting
           The backend supports sorting of the content. QIfFilterAndBrowseModelInterface::availableContentTypesChanged() and QIfFilterAndBrowseModelInterface::queryIdentifiersChanged() will be used as input for the
           \l {Qt Interface Framework Query Language}.
    \value SupportsAndConjunction
           The backend supports handling multiple filters at the same time and these filters can be combined by using the AND conjunction.
    \value SupportsOrConjunction
           The backend supports handling multiple filters at the same time and these filters can be combined by using the OR conjunction.
    \value SupportsStatelessNavigation
           The backend is stateless and supports handling multiple instances of a QIfFilterAndBrowseModel requesting different data at the same time.
           E.g. One request for artists, sorted by name and another request for tracks. The backend has to consider that both request come from models which are
           currently visible at the same time.
    \value SupportsInsert
           The backend supports inserting new items at a given position.
    \value SupportsMove
           The backend supports moving items within the model.
    \value SupportsRemove
           The backend supports removing items from the model.
*/
QtInterfaceFrameworkModule::QtInterfaceFrameworkModule(QObject *parent)
    : QObject(parent)
{
}


/*!
    \enum QtInterfaceFrameworkModule::ModelCapability
    \target ModelCapabilities
    \value NoExtras
           The backend does only support the minimum feature set and is stateful.
    \value SupportsGetSize
           The backend can return the final number of items for a specific request. This makes it possible to support the QIfFilterAndBrowseModel::DataChanged loading
           type.
    \value SupportsFiltering
           The backend supports filtering of the content. QIfFilterAndBrowseModelInterface::availableContentTypesChanged() and QIfFilterAndBrowseModelInterface::queryIdentifiersChanged() will be used as input for the
           \l {Qt Interface Framework Query Language}.
    \value SupportsSorting
           The backend supports sorting of the content. QIfFilterAndBrowseModelInterface::availableContentTypesChanged() and QIfFilterAndBrowseModelInterface::queryIdentifiersChanged() will be used as input for the
           \l {Qt Interface Framework Query Language}.
    \value SupportsAndConjunction
           The backend supports handling multiple filters at the same time and these filters can be combined by using the AND conjunction.
    \value SupportsOrConjunction
           The backend supports handling multiple filters at the same time and these filters can be combined by using the OR conjunction.
    \value SupportsStatelessNavigation
           The backend is stateless and supports handling multiple instances of a QIfFilterAndBrowseModel requesting different data at the same time.
           E.g. One request for artists, sorted by name and another request for tracks. The backend has to consider that both request come from models which are
           currently visible at the same time.
    \value SupportsInsert
           The backend supports inserting new items at a given position.
    \value SupportsMove
           The backend supports moving items within the model.
    \value SupportsRemove
           The backend supports removing items from the model.
*/

/*!
    Registers all enums in this class in the Qt MetaType system
*/
void QtInterfaceFrameworkModule::registerTypes()
{
    static bool once = false;
    if (once)
        return;
    once = true;

    qRegisterMetaType<QIfServiceObject*>();
    qRegisterMetaType<QList<QIfServiceObject*>>("QList<QIfServiceObject*>");
    qRegisterMetaType<QtInterfaceFrameworkModule::ModelCapabilities>();
    qifRegisterPendingReplyBasicTypes();
    qIfRegisterPendingReplyType<QtInterfaceFrameworkModule::ModelCapabilities>();
}

/*!
    Registers the QtInterfaceFramework classes to the QML System in the library imported from \a uri having
    the version number composed from \a majorVersion and \a minorVersion.
*/
void QtInterfaceFrameworkModule::registerQmlTypes(const QString &uri, int majorVersion, int minorVersion)
{
    registerTypes();
    QByteArray u = uri.toLatin1();
    qmlRegisterSingletonType<QtInterfaceFrameworkModule>(u, majorVersion, minorVersion,
                                        "QtInterfaceFrameworkModule",
                                        qtinterfaceframeworkmodule_singletontype_provider);
    qmlRegisterUncreatableType<QIfAbstractFeatureListModel>(u, majorVersion, minorVersion, "AbstractFeatureListModel", QStringLiteral("AbstractFeatureListModel is not accessible directly"));
    qmlRegisterUncreatableType<QIfAbstractFeature>(u, majorVersion, minorVersion, "AbstractFeature", QStringLiteral("AbstractFeature is not accessible directly"));
    qmlRegisterUncreatableType<QIfAbstractZonedFeature>(u, majorVersion, minorVersion, "AbstractZonedFeature", QStringLiteral("AbstractZonedFeature is not accessible directly"));
    qmlRegisterType<QIfPagingModel>(u, majorVersion, minorVersion, "PagingModel");
    qmlRegisterType<QIfFilterAndBrowseModel>(u, majorVersion, minorVersion, "FilterAndBrowseModel");
    qmlRegisterType<QIfConfiguration>(u, majorVersion, minorVersion, "InterfaceFrameworkConfiguration");
    qmlRegisterSingletonType<QIfServiceManager>(u, majorVersion, minorVersion, "ServiceManager", &QIfServiceManager::create);
}
