// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qifsimulationglobalobject_p.h"
#include <QtDebug>
#include <QJsonDocument>

using namespace Qt::StringLiterals;

QT_BEGIN_NAMESPACE

// AXIVION DISABLE Qt-NonPodGlobalStatic
namespace qtif_helper {
    static const QString unsupportedLiteral = u"unsupported"_s;
    static const QString minLiteral = u"minimum"_s;
    static const QString maxLiteral = u"maximum"_s;
    static const QString rangeLiteral = u"range"_s;
    static const QString domainLiteral = u"domain"_s;
}
// AXIVION ENABLE Qt-NonPodGlobalStatic

using namespace qtif_helper;

/*!
      \qmlmodule IfSimulator 1.0
      \internal
    */

/*!
    \qmltype IfSimulator
    \inqmlmodule IfSimulator

    \brief The global object for parsing simulation data inside a QIfSimulationEngine.

    The IfSimulator global object provides access to the simulation data of a QIfSimulationEngine
    and provides helper functions for parsing and checking boundaries.

    \note This object is only available inside a QIfSimulationEngine and cannot be accessed
    outside of it.

    \section1 Data Format
    \target IfSimulatorDataFormat

    The IfSimulator expects its data already in a parsed form. Usually this is done by the
    QIfSimulationEngine::loadSimulationData() function, which expects the file to be in the JSON
    format.

    \section2 Interfaces

    Because a simulation QML file can use multiple interfaces, the simulation data supports
    multiple data sets as well. These are identified by the interface name in a reverse DNS
    notation:

    \badcode
    {
      "QIfClimateControl": {
        ...
      },
      "org.qt-project.QIfWindowControl": {
        ...
      }
    }
    \endcode

    The findData() method can be used to find the data for a specific interface.

    \section2 Properties

    The settings (e.g. boundaries) for every property of the interface are defined inside a data
    set.

    \badcode
    {
      "QIfClimateControl": {
        "airConditioningEnabled": {
          "default": true
        },
        "steeringWheelHeater": {
          "minimum": 0,
          "default": 0
        },
        "fanSpeed": {
          "range": [0, 5]
        }
      }
    }
    \endcode

    For the interface named \c QIfClimateControl, there are settings defined for the properties
    \c airConditioningEnabled, \c steeringWheelHeater and \c fanSpeed.

    The settings object can store multiple constraints which are called \e domains. The following
    domains are currently supported:

    \list
        \value default Holds the default value the property should have when the frontend is
               connected.
        \value minimum Every newly set value needs to be bigger than this value
        \value maximum Every newly set value needs to be smaller than this value
        \value range Every newly set value needs to be between the two values in this list
        \note The values in \c range always override the \c minimum and \c maximum domains
        \value domain Every newly set value needs to be part of this list
        \value unsupported Changing the property is not possible and will show an "unsupported"
               error message
    \endlist

    The value for a specific domain can be loaded using the parseDomainValue() function or using
    defaultValue() when only the \e default domain is of interest.

    \section2 Structures and Enums

    As JSON is not typesafe, structures and enums need to be stored in a special format. An enum
    can be stored like this:

    \badcode
    {
      "QIfClimateControl": {
        "recirculationMode": {
          "default": {
            "type": "enum",
            "value": "QtIfVehicleFunctionsModule::RecirculationOff"
          }
        }
      }
    }
    \endcode

    Structures can be stored in a similar fashion using the name of the structure as \e type:

    \badcode
    {
      "AddressBook": {
        "contactList": {
          "default": [
            {
              "type": "Contact",
              "value": [
                "foo",
                23,
                true
              ]
            },
            {
              "type": "Contact",
              "value": [
                "bar",
                12,
                false
              ]
            }
          ]
        }
      },
      }
    }
    \endcode

    To correctly initialize the structure with these values, the structure needs to provide a
    constructor taking a QVariant as argument. For the given \e contact example this constructor
    can look like this:

    \code
    Contact::Contact(const QVariant &variant)
        : Contact()
    {
        QVariant value = qtif_convertFromJSON(variant);
        // First try to convert the values to a Map or a List
        // This is needed as it could also store a QStringList or a Hash
        if (value.canConvert(QMetaType::fromType<QVariantMap>()))
            value.convert(QMetaType::fromType<QVariantMap>());
        if (value.canConvert(QMetaType::fromType<QVariantList>()))
            value.convert(QMetaType::fromType<QVariantList>());

        if (value.type() == QVariant::Map) {
            QVariantMap map = value.toMap();
            if (map.contains(u"name"_s))
                d->m_name = map.value(u"name"_s).value<QString>();
            if (map.contains(u"age"_s))
                d->m_age = map.value(u"age"_s).value<int>();
            if (map.contains(u"isMarried"_s))
                d->m_isMarried = map.value(u"isMarried"_s).value<bool>();
        } else if (value.type() == QVariant::List) {
            QVariantList values = value.toList();
            d->m_name = values.value(0).value<QString>();
            d->m_age = values.value(1).value<int>();
            d->m_isMarried = values.value(2).value<bool>();
        }
    }
    \endcode

    \section2 Zones

    For zoned interfaces, the supported zones are usually stored as list in the \e zones property:

    \badcode
    {
      "QIfClimateControl": {
        "zones": [
          "FrontLeft",
          "FrontRight",
          "Rear"
        ]
      }
    }
    \endcode

    For every supported domain it is also possible to provide zone specific values e.g.:

    \badcode
    {
      "QIfClimateControl": {
        "targetTemperature": {
          "maximum": 30.0,
          "default": {
            "FrontLeft": 21.0,
            "FrontRight": 22.5,
            "=": 0.0
          }
        }
      }
    }
    \endcode

    This defines that the maximum value for the \e targetTemperature property is \e 30, the default
    value is zone specific and is \e 21.5 for the \e FrontLeft zone, while it is \e 22.5 for the
    FrontRight zone. The unzoned \e targetTemperature temperature is initialized with \e 0.0.
*/
QIfSimulationGlobalObject::QIfSimulationGlobalObject(QObject *parent)
    : QObject(parent)
{
}

/*!
    \qmlproperty object IfSimulator::simulationData

    Provides the simulation data parsed in QIfSimulationEngine::loadSimulationData()
*/
QVariant QIfSimulationGlobalObject::simulationData() const
{
    return m_simulationData;
}

void QIfSimulationGlobalObject::setSimulationData(const QVariant &simulationData)
{
    m_simulationData = simulationData;
}

/*!
    \qmlmethod IfSimulator::findData(object data, string interface)

    Searches for the key \a interface within \a data and returns the stored values. Returns
    undefined if no data was found for this \a interface.

    If \a interface is a reverse NDS name, it first searches for the full string. If no key was
    found, it starts to search again with a reduced name until it finds a key with this name.

    E.g. for the interface \e org.qt-project.ClimateControl it searches for keys in the following
    order:

    \list 1
        \li org.qt-project.ClimateControl
        \li qt-project.ClimateControl
        \li ClimateControl
    \endlist
*/
QVariantMap QIfSimulationGlobalObject::findData(const QVariantMap &data, const QString &interface)
{
    QString key = interface;
    forever {
        if (data.contains(key))
            return data.value(key).toMap();

        qsizetype index = key.indexOf(QLatin1Char('.'));
        if (index == -1)
            break;
        key = key.right(key.size() - index - 1);
    }

    return QVariantMap();
}

/*!
    \qmlmethod IfSimulator::initializeDefault(object data, QObject* object)

    Applies the default values read from \a data to \a object.

    If \a object supports zoneing, the default value is only applied to the correct zone.
*/
void QIfSimulationGlobalObject::initializeDefault(const QVariantMap &data, QObject *object)
{
    for (auto i = data.constBegin(); i != data.constEnd(); ++i) {
        QByteArray key = i.key().toLatin1();
        const QVariant defVal = defaultValue(i.value().toMap());
        if (defVal.isValid()) {
            QVariant currentValue = object->property(key);
            if (QIfPagingModelInterface *model = currentValue.value<QIfPagingModelInterface*>()) {
                QVariantList list = defVal.toList();
                for (auto i = list.crbegin(); i != list.crend(); ++i)
                    QMetaObject::invokeMethod(model, "insert", createArgument(int(0)), createArgument(*i));
            } else {
                object->setProperty(key, defVal);
            }
        }

        QVariant currentValue = object->property("zones");
        QQmlPropertyMap *map = currentValue.value<QQmlPropertyMap*>();
        if (!map)
            continue;
        const QStringList zones = data.value(u"zones"_s).toStringList();
        for (const QString &zone : zones) {
            const QVariant defVal = defaultValue(i.value().toMap(), zone);
            if (defVal.isValid()) {
                QObject *zoneObject = map->value(zone).value<QObject*>();
                if (zoneObject)
                    zoneObject->setProperty(key, defVal);
            }
        }
    }
}

/*!
    \qmlmethod IfSimulator::defaultValue(object data, string zone)

    Provides the default value stored in \a data for the given \a zone. If \a zone is undefined or
    the data doesn't provide a default value for the given \a zone, it returns the unzoned default
    value if available.

    This is just a convenience function calling parseDomainValue() with the domain \e default.
*/
QVariant QIfSimulationGlobalObject::defaultValue(const QVariantMap &data, const QString &zone)
{
    return parseDomainValue(data, u"default"_s, zone);
}

/*!
    \qmlmethod IfSimulator::constraint(object data, string zone)

    Searches for all boundary settings in \a data for the given \a zone and returns the constraint
    (which is enforced for newly set values) in a human readable form.

    This is useful for error messages in connection with checkSettings().

    \sa checkSettings()
*/
QString QIfSimulationGlobalObject::constraint(const QVariantMap &data, const QString &zone)
{
    const QVariant unsupportedDomain = parseDomainValue(data, unsupportedLiteral, zone);
    QVariant minDomain = parseDomainValue(data, minLiteral, zone);
    QVariant maxDomain = parseDomainValue(data, maxLiteral, zone);
    const QVariant rangeDomain = parseDomainValue(data, rangeLiteral, zone);
    if (rangeDomain.isValid()) {
        QVariantList range = rangeDomain.toList();
        if (range.count() == 2) {
            minDomain = range.at(0);
            maxDomain = range.at(1);
        } else {
            qtif_qmlOrCppWarning(this, "Domain 'range' needs to be list of exactly two values");
        }
    }
    const QVariant domainDomain = parseDomainValue(data, domainLiteral, zone);

    if (unsupportedDomain.isValid())
        return unsupportedLiteral;
    if (minDomain.isValid() && maxDomain.isValid())
        return u"["_s + minDomain.toString() + u"-"_s + maxDomain.toString() + u"]"_s ;
    if (minDomain.isValid())
        return u">= "_s + minDomain.toString();
    if (maxDomain.isValid())
        return u"<= "_s + maxDomain.toString();
    if (domainDomain.isValid())
        return QString::fromUtf8(QJsonDocument::fromVariant(domainDomain).toJson(QJsonDocument::Compact));

    return QString();
}

/*!
    \qmlmethod IfSimulator::checkSettings(object data, var value, string zone)

    Searches for all boundary settings in \a data for the given \a zone and returns whether the
    provided \a value meets this constraint.

    To show meaningful error messages when the value is not within the boundaries, the constraint()
    function can be used.

    \sa constraint()
*/
bool QIfSimulationGlobalObject::checkSettings(const QVariantMap &data, const QVariant &value, const QString &zone)
{
    const QVariant unsupportedDomain = parseDomainValue(data, unsupportedLiteral, zone);
    QVariant minDomain = parseDomainValue(data, minLiteral, zone);
    QVariant maxDomain = parseDomainValue(data, maxLiteral, zone);
    const QVariant rangeDomain = parseDomainValue(data, rangeLiteral, zone);
    if (rangeDomain.isValid()) {
        QVariantList range = rangeDomain.toList();
        if (range.isEmpty())
            return true;

        if (range.count() == 2) {
            minDomain = range.at(0);
            maxDomain = range.at(1);
        } else {
            qtif_qmlOrCppWarning(this, "Domain 'range' needs to be list of exactly two values");
            return false;
        }
    }
    const QVariant domainDomain = parseDomainValue(data, domainLiteral, zone);
    bool doubleValueOk = false;
    double doubleValue = value.toDouble(&doubleValueOk);
    bool minDomainDoubleOk = false;
    double minDomainDouble = minDomain.toDouble(&minDomainDoubleOk);
    bool maxDomainDoubleOk = false;
    double maxDomainDouble = maxDomain.toDouble(&maxDomainDoubleOk);

    if (unsupportedDomain.isValid() && unsupportedDomain.canConvert<bool>()) {
        return !unsupportedDomain.toBool();
    } else if (minDomain.isValid() && minDomain.canConvert<double>() && maxDomain.isValid() && maxDomain.canConvert<double>()) {
        if (!doubleValueOk || !minDomainDoubleOk || !maxDomainDoubleOk) {
            QString errorString;
            QDebug(&errorString) << "Can't compare values:" << value << "minimum:" << minDomain << "maximum:" << maxDomain;
            qtif_qmlOrCppWarning(this, errorString.trimmed());
            return false;
        }
        return !(doubleValue < minDomainDouble || doubleValue > maxDomainDouble);
    } else if (minDomain.isValid() && minDomain.canConvert<double>()) {
        if (!doubleValueOk || !minDomainDoubleOk) {
            QString errorString;
            QDebug(&errorString) << "Can't compare values:" << value << minDomain;
            qtif_qmlOrCppWarning(this, errorString.trimmed());
            return false;
        }
        return doubleValue >= minDomainDouble;
    } else if (maxDomain.isValid() && maxDomain.canConvert<double>()) {
        if (!doubleValueOk || !maxDomainDoubleOk) {
            QString errorString;
            QDebug(&errorString) << "Can't compare values:" << value << maxDomain;
            qtif_qmlOrCppWarning(this, errorString.trimmed());
            return false;
        }
        return doubleValue <= maxDomainDouble;
    } if (domainDomain.isValid() && domainDomain.canConvert<QVariantList>()) {
        return domainDomain.toList().contains(value);
    }

    return true;
}

/*!
    \qmlmethod IfSimulator::parseDomainValue(object data, string domain, string zone)

    Search for the \a domain in \a data for the given \a zone. If \a zone is undefined or
    the data doesn't provide this domain for the given \a zone, it returns the unzoned domain
    value if available.
*/
QVariant QIfSimulationGlobalObject::parseDomainValue(const QVariantMap &data, const QString &domain, const QString &zone)
{
    if (!data.contains(domain))
        return QVariant();

    const QVariant domainData = data.value(domain);
    if (domainData.metaType().id() == QMetaType::QVariantMap) {
        const QVariantMap domainMap = domainData.toMap();
        QString z = zone;
        if (zone.isEmpty())
            z = u"="_s;

        if (domainMap.contains(z))
            return qtif_convertFromJSON(domainMap.value(z));
    }

    return qtif_convertFromJSON(domainData);
}

QGenericArgument QIfSimulationGlobalObject::createArgument(const QVariant &variant)
{
    return QGenericArgument(variant.typeName(), variant.data());
}

QT_END_NAMESPACE
