{#
# Copyright (C) 2021 The Qt Company Ltd.
# Copyright (C) 2019 Luxoft Sweden AB
# SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0
#}
{% include "common/generated_comment.cpp.tpl" %}
#include <QCoreApplication>
#include <QCommandLineParser>
#include <QDir>
#include <QLockFile>

#include <QtIfRemoteObjectsHelper/QIfRemoteObjectsConfig>

using namespace Qt::StringLiterals;

{% set ns = module|namespace %}
{% if ns|length %}
using namespace {{ns}};
{% endif %}

void serverMain(QIfRemoteObjectsConfig &config);

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    QCommandLineParser parser;
    parser.addHelpOption();

    QCommandLineOption serverUrlOption(u"serverUrl"_s, u"The serverUrl to use for all Remote Objects hosted in this server"_s, u"url"_s);
    parser.addOption(serverUrlOption);

    QCommandLineOption confOption(u"serverConf"_s, u"A config file which host url for all Remote Objects"_s, u"file"_s);
    parser.addOption(confOption);

    parser.process(qApp->arguments());

    // single instance guard
    QLockFile lockFile(u"%1/%2.lock"_s.arg(QDir::tempPath(), qApp->applicationName()));
    if (!lockFile.tryLock(100)) {
        qCritical("%s already running, aborting...", qPrintable(qApp->applicationName()));
        return EXIT_FAILURE;
    }

    QIfRemoteObjectsConfig config;
    if (parser.isSet(serverUrlOption))
        config.setDefautServerUrl(parser.value(serverUrlOption));
    if (parser.isSet(confOption))
        config.parseConfigFile(parser.value(confOption));
    if (!parser.isSet(serverUrlOption) && !parser.isSet(confOption))
        config.parseLegacyConfigFile();

    serverMain(config);

    return qApp->exec();
}
