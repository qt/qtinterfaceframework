// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qifsimulationengine.h"
#include "qifconfiguration.h"
#include <qifsimulationglobalobject_p.h>

#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QDebug>
#include <QQmlContext>
#include <QLoggingCategory>
#include <QRegularExpression>

using namespace Qt::StringLiterals;

QT_BEGIN_NAMESPACE

namespace qtif_helper {
    QUrl toQmlUrl(const QString &path) {
        static const QString qrcUrlLiteral = u"qrc:"_s;
        static const QString qrcLiteral = u"qrc"_s;
        static const QString resourceLiteral = u":/"_s;

        if (path.startsWith(qrcUrlLiteral))
            return path;
        else if (path.startsWith(resourceLiteral))
            return QString(path).prepend(qrcLiteral);
        else if (QFile::exists(path)) // If the file exists a normal path was passed (instead of a url)
            return QUrl::fromLocalFile(path);
        return path;
    }
}

using namespace qtif_helper;

/*!
    \class QIfSimulationEngine
    \inmodule QtInterfaceFramework
    \brief Provides a way to script a simulation backend from QML.

    This class is an extended QQmlApplicationEngine which can be used to load QML files. It is made
    especially for \l {Dynamic Backend System}{simulation backends} to script its behavior from
    QML. For an overview of its functionality, see \l{Qt Interface Framework Simulation System}.

    Compared to a normal QQmlEngine, the QIfSimulationEngine provides an extra template function
    called registerSimulationInstance(). Use this function to register a class instance as a QML
    type. Within a QML file, this QML type can be used to define the behavior for function calls,
    update properties, or emit signals.

    \section1 Register an Instance

    You can register any instance of a class derived from QObject to the QIfSimulationEngine by
    calling registerSimulationInstance(). Similar to qmlRegisterTypes, the URI, version, and name
    provided are used to import the type from within QML.

    \code
    class MyClass : public QObject
    {
        Q_OBJECT
        Q_PROPERTY(int currentTemperature READ currentTemperature WRITE setCurrentTemperature NOTIFY currentTemperatureChanged)

        ...
    }
    \endcode

    An instance of this simple class can be registered like this:

    \code
    QIfSimulationEngine engine;
    MyClass myClass;
    engine.registerSimulationInstance<MyClass>(&myClass, "Test", 1, 0, "MyClass");
    engine.loadSimulation("simulation.qml")
    \endcode

    The registered instance has the same constraints as any other C++ classes exposed to QML and
    needs to use Q_PROPERTY, Q_INVOKABLE, or slots to make the functionality available to QML.

    \section1 Use the Type from QML

    Once an instance is registered to the engine, the type can be used like any other QML element
    in a declarative form:

    \qml
    import QtQuick
    import Test

    Item {
        MyClass {
            id: myClass

            Component.onCompleted: currentTemperature = 10;
        }

        Timer {
            running: true
            repeat: true
            interval: 1000
            onTriggered: myClass.currentTemperature++;
        }
    }
    \endqml

    This QML file initializes the \c currentTemperature of \c myClass with a value of \c 10 and
    increases it every second.

    In the same way, values can be updated from the C++ side and the QML side can react to the
    change. For example, the following QML snippet prints the \c currentTemperature whenever it
    changes:

    \qml
    import QtQuick
    import Test

    MyClass {
        onCurrentTemperatureChanged: print(currentTemperature)
    }
    \endqml

    The slot is called once the \c myClass variable is updated:

    \code
    QIfSimulationEngine engine;
    MyClass myClass;
    engine.registerSimulationInstance<MyClass>(&myClass, "Test", 1, 0, "MyClass");
    engine.loadSimulation("simulation.qml")
    ...
    myClass.setCurrentTemperature(100);
    \endcode

    \section1 Forward Calls from the Instance to the Engine

    You can also provide the behavior for invokable functions in QML as well, but this use case
    requires you to extend the exposed class. For example, by adding the following line to the
    \c setCurrentTemperature setter:

    \code
    void MyClass::setCurrentTemperature(int currentTemperature)
    {
        QIF_SIMULATION_TRY_CALL(MyClass, "setCurrentTemperature", void, currentTemperature);

        if (m_currentTemperature == currentTemperature)
            return;
        m_currentTemperature = currentTemperature;
        emit currentTemperatureChanged(m_currentTemperature);
    }
    \endcode

    Calling \c setCurrentTemperature() now tries to forward the call to the QML instance, if a
    function matching the signature is defined in QML. When successful, \c setCurrentTemperature()
    uses its returned value and avoids running the original C++ function.

    By using the following QML snippet, the C++ setter is skipped and only an error is emitted on
    the console:

    \qml
    import QtQuick
    import Test

    MyClass {
        function setCurrentTemperature(temperature) {
            print("Updating the temperature is not possible")
        }
    }
    \endqml

    \section1 Reuse Existing Behavior in the Instance

    Replacing the C++ functionality with a QML behavior is not always desired. However, it's also
    possible to call the original C++ behavior from QML. In this case, the original C++ function
    needs to be a Q_INVOKABLE or a slot. Additionally, the functionality works in a similar manner
    to function overriding in C++, where the functionality of the overridden function can be
    accessed by calling \c <BaseClass>::<function>. In the exposed QML type this is possible by
    calling the function in the \c Base object.

    \qml
    import QtQuick
    import Test

    MyClass {
        function setCurrentTemperature(temperature) {
            print("Updating the temperature: " + temperature )
            Base.setCurrentTemperature(temperature)
        }
    }
    \endqml

    This QML snippet overrides the setCurrentTemperature() behavior in QML and prints a debug
    message for the new value. The original C++ behavior is called using
    \c{Base.setCurrentTemperature(temperature)}.

    \section1 Multiple QML Instances

    The registered instance is exposed as a normal QML type. This makes it possible to have
    multiple declarations in QML, and in turn, have multiple QML instances linked to the same C++
    instance. Updating and reacting to property changes and signal emissions is possible in all
    instances, but should be used with care as this can result in property update loops and other
    issues.

    Forwarding C++ function calls to QML is limited. Every call is forwarded to only one QML
    instance as the return value is used from this call. If multiple QML instances define the same
    method, the C++ call is always forwarded to the first registered QML instance.

    \section1 Runtime Override

    Each QIfSimulationEngine can take an extra identifier which can be used to override the
    simulation QML file or the simulation data file at runtime. The environment variables need to
    be in the following format:

    \badcode
    QTIF_SIMULATION_OVERRIDE=<identifier>=<file>[;<identifier>=<file>]
    QTIF_SIMULATION_DATA_OVERRIDE=<identifier>=<file>[;<identifier>=<file>]
    \endcode
*/

QIfSimulationEngine::QIfSimulationEngine(QObject *parent)
    : QIfSimulationEngine(QString(), parent)
{
}

QIfSimulationEngine::QIfSimulationEngine(const QString &identifier, QObject *parent)
    : QQmlApplicationEngine (parent)
    , m_globalObject(new QIfSimulationGlobalObject)
    , m_identifier(identifier)
{
    rootContext()->setContextProperty(u"IfSimulator"_s, m_globalObject);
    setOutputWarningsToStandardError(false);

    connect(this, &QQmlApplicationEngine::warnings, this, [](const QList<QQmlError> &warnings) {
        for (const QQmlError &error : warnings)
            qCWarning(qLcIfSimulationEngine, "%s", qPrintable(error.toString()));
    });
}

/*!
    Loads the simulation data file provided as \a dataFile.

    The given file must be in JSON format and is parsed here for errors before it's passed to the
    IfSimulator global object where it can be accessed from QML. This file can be overridden
    at runtime using the following environment variable:

    \badcode
    QTIF_SIMULATION_DATA_OVERRIDE=<identifier>=<file>[;<identifier>=<file>]
    \endcode

    The simulation engine's identifier can be set in its constructor.

    \sa IfSimulator
*/
void QIfSimulationEngine::loadSimulationData(const QString &dataFile)
{
    QString filePath = dataFile;
    if (!m_identifier.isEmpty() && QIfConfiguration::isSimulationDataFileSet(m_identifier)) {
        filePath = QIfConfiguration::simulationDataFile(m_identifier);
        qCWarning(qLcIfSimulationEngine, "Using simulation data override from QIfConfiguration(%s): %s", qPrintable(m_identifier), qPrintable(filePath));
    }

    qCDebug(qLcIfSimulationEngine, "loading SimulationData for engine %s: %s", qPrintable(m_identifier), qPrintable(filePath));

    QFile file(filePath);
    if (!file.open(QFile::ReadOnly)) {
        qCCritical(qLcIfSimulationEngine, "Cannot open the simulation data file %s: %s", qPrintable(filePath), qPrintable(file.errorString()));
        return;
    }

    QJsonParseError pe;
    QByteArray data = file.readAll();
    QJsonDocument document = QJsonDocument::fromJson(data, &pe);
    if (pe.error != QJsonParseError::NoError) {
        qCCritical(qLcIfSimulationEngine, "Error parsing the simulation data in %s: %s", qPrintable(filePath), qPrintable(pe.errorString()));
        qCCritical(qLcIfSimulationEngine, "Error context:\n %s", data.mid(qMax(pe.offset - 20, 0), 40).data());
    }
    m_globalObject->setSimulationData(document.toVariant());
}

/*!
    Loads the QML \a file as the simulation behavior.

    In addition to QQmlApplicationEngine::load(), this function provides functionality to change
    the simulation file used via an environment variable in the following format:

    \badcode
    QTIF_SIMULATION_OVERRIDE=<identifier>=<file>[;<identifier>=<file>]
    \endcode

    The simulation engine's identifier can be set in its constructor.
*/
void QIfSimulationEngine::loadSimulation(const QUrl &file)
{
    QUrl filePath = file;
    if (!m_identifier.isEmpty() && QIfConfiguration::isSimulationFileSet(m_identifier)) {
        filePath = toQmlUrl(QIfConfiguration::simulationFile(m_identifier));
        qCWarning(qLcIfSimulationEngine, "Using simulation override from QIfConfiguration(%s): %s", qPrintable(m_identifier), qPrintable(filePath.toString()));
    }

    qCDebug(qLcIfSimulationEngine, "loading simulation for engine %s: %s", qPrintable(m_identifier), qPrintable(filePath.toString()));

    load(filePath);
}

/*!
    \fn template <typename T> void QIfSimulationEngine::registerSimulationInstance(T* instance, const char *uri, int versionMajor, int versionMinor, const char *qmlName)

    Registers the specified \a instance in the QML system with the name \a qmlName, in the library
    imported from \a uri, with a version number composed from \a versionMajor and \a versionMinor.

    \note The registered instance is only available to this QIfSimulationEngine instance.
    Using it from another QIfSimulationEngine or a QQmlEngine won't work and produces an error.

    \sa qmlRegisterType
*/

/*!
    \macro QIF_SIMULATION_TRY_CALL_FUNC(instance_type, function, ret_func, ...)
    \relates QIfSimulationEngine

    Tries to call \a function in the QML instances registered for the \a instance_type. The variadic
    arguments are passed as arguments to the function in QML.

    If the call is successful, the code passed via \a ret_func is run. This can be useful for
    situations when the return value needs to be converted first. The original return value is
    available as \c return_value.

    \code
    QIF_SIMULATION_TRY_CALL_FUNC(MyClass, "contactList", return return_value.toStringList());
    \endcode

    \sa QIF_SIMULATION_TRY_CALL {Forward Calls from the Instance to the Engine}
*/

/*!
    \macro QIF_SIMULATION_TRY_CALL(instance_type, function, ret_type, ...)
    \relates QIfSimulationEngine

    Tries to call \a function in the QML instances registered for the \a instance_type. The variadic
    arguments are passed as arguments to the function in QML.

    If the call is successful, a return value of \a ret_type is returned and all code after this
    macro \b{won't} run.

    \sa QIF_SIMULATION_TRY_CALL_FUNC {Forward Calls from the Instance to the Engine}
*/

QT_END_NAMESPACE

#include "qifsimulationengine.moc"
//#include "moc_qifsimulationengine.cpp"
