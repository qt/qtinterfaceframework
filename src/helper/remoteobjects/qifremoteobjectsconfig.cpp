// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qifremoteobjectsconfig.h"

#include <QtIfRemoteObjectsHelper/qifremoteobjectshelper.h>

using namespace Qt::StringLiterals;

QT_BEGIN_NAMESPACE

#if QT_VERSION < QT_VERSION_CHECK(6, 9, 0)
#  define Q_STATIC_LOGGING_CATEGORY(cat, rule, init) Q_LOGGING_CATEGORY(cat, rule, init)
#endif

Q_STATIC_LOGGING_CATEGORY(qLcQtIfRoConfig, "qt.if.remoteobjects.config",
                          QtInfoMsg)

/*!
    \class QIfRemoteObjectsConfig
    \inmodule QtIfRemoteObjectsHelper
    \brief The QIfRemoteObjectsConfig helps to maintain config options for remote object based
    servers.

    The QIfRemoteObjectsConfig class can used to retrieve a correctly configured QRemoteObjectHost
    for the provided interface.

    In the simplest form it can be used like this in your main.cpp:
    \code
    #include <QCoreApplication>

    #include "myapi.h"
    #include "core.h"

    #include <QIfRemoteObjectsConfig>

    using namespace Qt::StringLiterals;

    int main(int argc, char *argv[])
    {
        QCoreApplication app(argc, argv);

        QIfRemoteObjectsConfig config;

        MyApi service;
        config.enableRemoting(u"Example.If.RemoteModule"_s, "MyApi"_s, &service);

        return app.exec();
    }
    \endcode

    This will make the service accessible using the default url build from the module parameter.
    In this case: \e local://RemoteModule

    \section1 Manage Remoting URLs

    Usually multiple interfaces are defined within a module and by using the default urls those
    services are always served by a single module URL using a local socket.

    To change the URL of all services a default server can be set using the \l setDefaultServerUrl()
    function.

    For more flexibility, a config file can be provided using the \l parseConfigFile function. This
    allows to set the used URL on interface or module level. Defining module URLs allows the same
    config file to be reused by multiple servers where each server hosts all interfaces of a single
    module.

    \section1 Legacy Config support

    Until 6.7 the autogenerated code of a QtRemoteObject based server used a file called
    "server.conf" in the current directory to read the URLs used for remoting a service.

    Reading this config file can be enabled by using the \l parseLegacyConfigFile() function.

    See also the \l{config_server_qtro_useGeneratedMain}{useGeneratedMain QFace Annotation} to let
    \l ifcodegen generate a main.cpp with fully setup QIfRemoteObjectsConfig.
*/


/*!
    Sets the \a defaultServerUrl, which will be used for all services which don't have a server set
    either for the interface or the module inside a config file.
*/
void QIfRemoteObjectsConfig::setDefaultServerUrl(const QUrl &defaultServerUrl)
{
    m_defaultServer = defaultServerUrl;
}

/*!
    Parses the config file at \a confFilePath and uses the stored module and interface specific
    urls when requested by \l host().

    \sa host()
*/
void QIfRemoteObjectsConfig::parseConfigFile(const QString &confFilePath)
{
    m_settings.reset(new QSettings(confFilePath, QSettings::IniFormat));
}

/*!
    Parses the legacy \e "server.conf" file in the current directory or at the location set
    by the \e SERVER_CONF_PATH environment variable.
*/
void QIfRemoteObjectsConfig::parseLegacyConfigFile()
{
    QString configPath(u"./server.conf"_s);
    if (qEnvironmentVariableIsSet("SERVER_CONF_PATH")) {
        qCDebug(qLcQtIfRoConfig) << "Environment variable SERVER_CONF_PATH defined." << configPath;
        configPath = QString::fromLocal8Bit(qgetenv("SERVER_CONF_PATH"));
    }
    qCDebug(qLcQtIfRoConfig) << "Parsing local server.conf file:" << QDir::current().absoluteFilePath(configPath);
    m_settings.reset(new QSettings(configPath, QSettings::IniFormat));
}

/*!
    Enables logging of all errors reported by the created QRemoteObjectHost instances on stderr.

    Setting \a enabled to \c false disables the logging.

    \sa reportErrorsOnStdErr
*/
void QIfRemoteObjectsConfig::setReportErrorsOnStdErr(bool enabled)
{
    m_reportErrorsOnStdErr = enabled;
}

/*!
    Returns \c true, if error logging to stderr for the created QRemoteObjectHost instances is
    enabled. Defaults to \c true.
*/
bool QIfRemoteObjectsConfig::reportErrorsOnStdErr() const
{
    return m_reportErrorsOnStdErr;
}

/*!
    Returns a QRemoteObjectHost with a preconfigured url.

    The url is specific to the provided \a module and \a interface arguments and can be modified
    using the \l setDefaultServerUrl() function or by using one of the config files.

    If multiple values are configured the urls are resolved in the following order:
    \list numbered
        \li interface
        \li module
        \li legacy \e Registry key
        \li defaultServer
        \li \a fallbackUrl argument
    \endlist

    If none of these settings have been configured, or invalid urls were passed, a default url is
    built using the \a module argument.

    \sa host
*/
QRemoteObjectHost *QIfRemoteObjectsConfig::host(const QString &module, const QString &interface, const QUrl &fallbackUrl)
{
    QUrl url;
    if (m_settings) {
        m_settings->beginGroup(module + u"/" + interface);
        url = m_settings->value(u"connectionUrl").toUrl();
        m_settings->endGroup();
        if (url.isValid())
            return host(url);

        m_settings->beginGroup(module);
        url = m_settings->value(u"connectionUrl").toUrl();
        m_settings->endGroup();
        if (url.isValid())
            return host(url);

        // Debug registry is deprecated please use the serverUrl instead.
        if (m_settings->contains(u"Registry")) {
            qCInfo(qLcQtIfRoConfig) << "Using the 'Registry' key is deprecated and will be removed"
                                       "in future Qt versions.";
            qCInfo(qLcQtIfRoConfig) << "Please use the 'connectionUrl' key instead."
                                       "This can be set per module or per interface in the conf file.";
            url = m_settings->value(u"Registry").toUrl();
        }
        if (url.isValid())
            return host(url);

    }

    // No settings for the interface/module were provided
    // Use the defaultServer if that is set
    if (m_defaultServer.isValid())
        return host(m_defaultServer);

    // If valid use fallback URL
    if (fallbackUrl.isValid())
        return host(fallbackUrl);

    // Create default url (using localsockets) for the module
    return host(QUrl(QIfRemoteObjectsHelper::buildDefaultUrl(module.split(u'.').last().toLower())));
}

/*!
    Returns a QRemoteObjectHost with a preconfigured \a url.

    \sa host
*/
QRemoteObjectHost *QIfRemoteObjectsConfig::host(const QUrl &url)
{
    if (m_hostHash.contains(url))
        return m_hostHash.value(url);

    auto host = new QRemoteObjectHost(url);
    m_hostHash.insert(url, host);
    qCDebug(qLcQtIfRoConfig) << "Listening on" << url;
    host->connect(host, &QRemoteObjectNode::error, host, [this](QRemoteObjectNode::ErrorCode code) {
        // No logging category on purpose, this is not related to the config class
        if (m_reportErrorsOnStdErr)
            qWarning() << "QRemoteObjects Error: " << code;
    });
    return host;
}

/*!
    Enables remoting of the passed \a object using a QRemoteObjectHost instance
    with a preconfigured url.

    Returns \c true if remoting is successfully enabled for the passed \a object.

    See host() for more information on how \a module, \a interface and \a fallbackUrl influence
    the effective url.

    \sa enableRemoting
*/
bool QIfRemoteObjectsConfig::enableRemoting(const QString &module, const QString &interface, const QUrl &fallbackUrl, QObject *object)
{
    return host(module, interface, fallbackUrl)->enableRemoting(object, module + u"."_s + interface);
}

/*!
    Enables remoting of the passed \a object using a QRemoteObjectHost instance
    with a preconfigured url.

    Returns \c true if remoting is successfully enabled for the passed \a object.

    See host() for more information on how \a module and \a interface influence the effective url.

    \sa enableRemoting
*/
bool QIfRemoteObjectsConfig::enableRemoting(const QString &module, const QString &interface, QObject *object)
{
    return enableRemoting(module, interface, QString(), object);
}

QT_END_NAMESPACE

#include "moc_qifremoteobjectsconfig.cpp"
