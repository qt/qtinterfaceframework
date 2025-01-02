// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qifabstractfeature.h"
#include "qifabstractfeature_p.h"
#include "qifabstractzonedfeature.h"
#include "qifdefaultpropertyoverrider_p.h"
#include "qifserviceobject.h"

#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>

QT_BEGIN_NAMESPACE

QIfDefaultPropertyOverrider::PropertyOverride::PropertyOverride()
    : m_overridable(false)
    , m_overriding(false)
{
}

QIfDefaultPropertyOverrider::PropertyOverride::PropertyOverride(QIfDefaultPropertyOverrider::PropertyOverride &&other)
{
    *this = std::move(other);
}

QIfDefaultPropertyOverrider::PropertyOverride::PropertyOverride(const QMetaProperty &metaProperty, const QVariant &value, bool overridable)
    : m_metaProperty(metaProperty)
    , m_overridable(overridable)
    , m_overriding(false)
    , m_originalValue(value)
{
}

QIfDefaultPropertyOverrider::PropertyOverride &QIfDefaultPropertyOverrider::PropertyOverride::operator=(QIfDefaultPropertyOverrider::PropertyOverride &&other)
{
    m_metaProperty = other.m_metaProperty;
    m_originalValue = other.m_originalValue;
    m_overridenValue = other.m_overridenValue;
    m_overriding = other.m_overriding;
    m_overridable = other.m_overridable;
    return *this;
}

int QIfDefaultPropertyOverrider::PropertyOverride::propertyIndex() const
{
    return m_metaProperty.isValid() ? m_metaProperty.propertyIndex() : -1;
}

bool QIfDefaultPropertyOverrider::PropertyOverride::isValid() const
{
    return m_metaProperty.isValid();
}

bool QIfDefaultPropertyOverrider::PropertyOverride::isAvailable() const
{
    return m_metaProperty.isValid();
}

bool QIfDefaultPropertyOverrider::PropertyOverride::hasNotifySignal() const
{
    return m_metaProperty.hasNotifySignal();
}

bool QIfDefaultPropertyOverrider::PropertyOverride::isWritable() const
{
    return (m_metaProperty.isWritable() && !QMetaType(m_metaProperty.userType()).flags().testFlag(QMetaType::PointerToQObject));
}

bool QIfDefaultPropertyOverrider::PropertyOverride::isOverridable() const
{
    if (!m_overridable)
        return false;
    int propertyOffset = QIfAbstractFeature::staticMetaObject.propertyCount();
    return m_metaProperty.propertyIndex() >= propertyOffset;
}

bool QIfDefaultPropertyOverrider::PropertyOverride::isOverridden() const
{
    return m_overriding;
}

QString QIfDefaultPropertyOverrider::PropertyOverride::name() const
{
    return QString::fromUtf8(m_metaProperty.name());
}

QString QIfDefaultPropertyOverrider::PropertyOverride::typeName() const
{
    const int userType(m_metaProperty.userType());
    return QString::fromLatin1(QMetaType(userType).name());
}

QString QIfDefaultPropertyOverrider::PropertyOverride::displayText() const
{
    const QVariant &value = m_overriding ? m_overridenValue : m_originalValue;
    return value.toString();
}

QVariant QIfDefaultPropertyOverrider::PropertyOverride::editValue() const
{
    const QVariant &value = m_overriding ? m_overridenValue : m_originalValue;
    return value;
}

QVariant QIfDefaultPropertyOverrider::PropertyOverride::cppValue() const
{
    return m_overriding ? m_overridenValue : m_originalValue;
}

void QIfDefaultPropertyOverrider::PropertyOverride::setOverriden(bool override)
{
    if (override != m_overriding) {
        m_overriding = override;
        if (m_overriding)
            m_overridenValue = m_originalValue;
    }
}

bool QIfDefaultPropertyOverrider::PropertyOverride::setOverridenValue(const QVariant &value, QIfAbstractFeature *carrier)
{
    Q_ASSERT(isAvailable());

    const bool wasOverride = isOverridden();
    bool isOverride = wasOverride || !isWritable();

    if (isOverride && !wasOverride && isOverridable())
        setOverriden(isOverride);
    if (isOverride)
        m_overridenValue = value;

    if (m_metaProperty.isWritable())
        return m_metaProperty.write(carrier, value);
    else
        return notifyOverridenValue(value, carrier);
}

bool QIfDefaultPropertyOverrider::PropertyOverride::notifyOverridenValue(const QVariant &value, QIfAbstractFeature *carrier)
{
    QIfAbstractFeaturePrivate *d = QIfAbstractFeaturePrivate::get(carrier);
    if (d && d->notify(m_metaProperty.name(), value))
        return false;

    QMetaMethod notifySignal = m_metaProperty.notifySignal();
    if (!notifySignal.isValid() || notifySignal.parameterCount() != 1)
        return false;

    switch (value.metaType().id()) {
    case QMetaType::Int: return notifySignal.invoke(carrier, Q_ARG(int, value.value<int>()));
    case QMetaType::QString: return notifySignal.invoke(carrier, Q_ARG(QString, value.value<QString>()));
    case QMetaType::Double: return notifySignal.invoke(carrier, Q_ARG(double, value.value<double>()));
    case QMetaType::Bool: return notifySignal.invoke(carrier, Q_ARG(double, value.value<bool>()));
    default:
        return false;
    }
}

void QIfDefaultPropertyOverrider::PropertyOverride::setOriginalValue(const QVariant &editValue)
{
    m_originalValue = editValue;
}

bool QIfDefaultPropertyOverrider::PropertyOverride::operator==(const QByteArray &property) const
{
    return this->m_metaProperty.name() == property;
}


QIfDefaultPropertyOverrider::QIfDefaultPropertyOverrider(QIfAbstractFeature *carrier, QObject *parent)
    : QObject(parent)
    , m_serviceObject(nullptr)
{
    if (carrier) {
        m_serviceObject = carrier->serviceObject();
        init(carrier);
        m_carriers.push_back(carrier);
    }
}

QIfDefaultPropertyOverrider::~QIfDefaultPropertyOverrider()
{
    for (auto &c : std::as_const(m_carriers))
        setCarrierOverride(false, c);
}

void QIfDefaultPropertyOverrider::init(QIfAbstractFeature *carrier)
{
    if (!carrier || !m_properties.empty())
        return;

    const QMetaObject *mo = carrier->metaObject();
    const int propertyOffset = QIfAbstractFeature::staticMetaObject.propertyCount();
    const int propertyCount = mo->propertyCount() - propertyOffset;

    QIfAbstractFeaturePrivate *carrierPrivate = QIfAbstractFeaturePrivate::get(carrier);
    const bool canOveride = carrierPrivate && carrierPrivate->m_supportsPropertyOverriding;

    const QByteArray normalizedSignal(QMetaObject::normalizedSignature("propertyChanged()"));
    const int propertyChangedSignalIndex(QIfDefaultPropertyOverrider::staticMetaObject.indexOfSignal(normalizedSignal));

    for (int i = 0; i < propertyCount; ++i) {
        const QMetaProperty metaProperty(mo->property(i + propertyOffset));
        qDebug() << i << metaProperty.name() << metaProperty.isValid();
        m_properties.push_back(PropertyOverride(metaProperty, metaProperty.read(carrier), canOveride));
        if (metaProperty.hasNotifySignal()) {
            QMetaObject::connect(carrier, metaProperty.notifySignalIndex(), this, propertyChangedSignalIndex);
        }
    }
}

void QIfDefaultPropertyOverrider::addCarrier(QIfAbstractFeature *carrier)
{
    if (!carrier)
        return;

    init(carrier);
    m_carriers.push_back(carrier);
    setCarrierOverride(true, carrier);
}

void QIfDefaultPropertyOverrider::removeCarrier(QIfAbstractFeature *carrier)
{
    setCarrierOverride(false, carrier);
    m_carriers.erase(std::remove_if(m_carriers.begin(), m_carriers.end(), [carrier](QIfAbstractFeature *c) {
        return carrier == c;
    }), m_carriers.end());
}

void QIfDefaultPropertyOverrider::setCarrierOverride(bool override, QIfAbstractFeature *carrier)
{
    if (!carrier)
        return;
    QIfAbstractFeaturePrivate *const pPriv = QIfAbstractFeaturePrivate::get(carrier);
    if (!pPriv)
        return;
    if (override && pPriv->m_propertyOverride == nullptr) {
        pPriv->m_propertyOverride = this;
    } else if (!override && pPriv->m_propertyOverride == this) {
        pPriv->m_propertyOverride = nullptr;
    }
}

int QIfDefaultPropertyOverrider::numCarriers() const
{
    return (int) m_carriers.size();
}

QVariant QIfDefaultPropertyOverrider::property(int propertyIndex) const
{
    if (m_carriers.empty())
        return {};

    const PropertyOverride &property = propertyForIndex(propertyIndex);
    return property.cppValue();
}

void QIfDefaultPropertyOverrider::setProperty(int propertyIndex, const QVariant &value)
{
    if (m_carriers.empty())
        return;

    PropertyOverride &property = propertyForIndex(propertyIndex);
    property.setOriginalValue(value);
}

bool QIfDefaultPropertyOverrider::isOverridden(int propertyIndex) const
{
    if (m_carriers.empty())
        return false;

    const PropertyOverride &property = propertyForIndex(propertyIndex);
    return property.isOverridden();
}

bool QIfDefaultPropertyOverrider::isWritableAt(int index) const
{
    if (m_carriers.empty())
        return false;

    const PropertyOverride &property = m_properties.at(index);
    return property.isWritable();
}

bool QIfDefaultPropertyOverrider::isOverridableAt(int index) const
{
    if (m_carriers.empty())
        return false;

    const PropertyOverride &property = m_properties.at(index);
    return property.isOverridable();
}

bool QIfDefaultPropertyOverrider::isAvailableAt(int index) const
{
    if (m_carriers.empty())
        return false;

    const PropertyOverride &property = m_properties.at(index);
    return property.isAvailable();
}

bool QIfDefaultPropertyOverrider::isOverriddenAt(int index) const
{
    if (m_carriers.empty())
        return false;

    const PropertyOverride &property = m_properties.at(index);
    return property.isOverridden();
}

bool QIfDefaultPropertyOverrider::hasNotifySignalAt(int index) const
{
    if (m_carriers.empty())
        return false;

    const PropertyOverride &property = m_properties.at(index);
    return property.hasNotifySignal();
}

QString QIfDefaultPropertyOverrider::nameAt(int index) const
{
    if (m_carriers.empty())
        return {};

    const PropertyOverride &property = m_properties.at(index);
    return property.name();
}

QString QIfDefaultPropertyOverrider::typeNameAt(int index) const
{
    if (m_carriers.empty())
        return {};

    const PropertyOverride &property = m_properties.at(index);
    return property.typeName();
}

QString QIfDefaultPropertyOverrider::displayTextAt(int index) const
{
    if (m_carriers.empty())
        return {};

    const PropertyOverride &property = m_properties.at(index);
    return property.displayText();
}

QVariant QIfDefaultPropertyOverrider::ifConstraintsAt(int index) const
{
    if (m_carriers.empty())
        return {};

    auto carrier = m_carriers.front();
    const PropertyOverride &property = m_properties.at(index);

    QByteArray constraintsJSON;
    for (int i=0; i<carrier->metaObject()->classInfoCount(); i++) {
        QMetaClassInfo ci = carrier->metaObject()->classInfo(i);
        if (QLatin1String(ci.name()) == QLatin1String("IfPropertyDomains")) {
            constraintsJSON = QByteArray(ci.value());
            break;
        }
    }
    if (constraintsJSON.isEmpty())
        return {};

    QJsonDocument loadDoc = QJsonDocument::fromJson(constraintsJSON);
    if (loadDoc.isNull() || loadDoc.isEmpty() || !loadDoc.isObject())
        return {};

    if (!loadDoc.object().contains(property.name()))
        return {};

    QJsonValue constraints = loadDoc.object().value(property.name());
    QLatin1String range("range");
    if (constraints.toObject().contains(range)) {
        QJsonArray vals = constraints.toObject().value(range).toArray();
        return QVariantList() << range << vals.at(0).toDouble() << vals.at(1).toDouble();
    }
    QLatin1String minimum("minimum");
    if (constraints.toObject().contains(minimum)) {
        double val = constraints.toObject().value(minimum).toDouble();
        return QVariantList() << minimum << val;
    }
    QLatin1String maximum("maximum");
    if (constraints.toObject().contains(maximum)) {
        double val = constraints.toObject().value(maximum).toDouble();
        return QVariantList() << maximum << val;
    }
    QLatin1String domain("domain");
    if (constraints.toObject().contains(domain)) {
        const QVariantList vals = constraints.toObject().value(domain).toArray().toVariantList();
        QVariantList res;
        res << domain << vals;
        return res;
    }
    return {};
}

QVariant QIfDefaultPropertyOverrider::editValueAt(int index) const
{
    if (m_carriers.empty())
        return {};

    const PropertyOverride &property = m_properties.at(index);
    return property.editValue();
}

QString QIfDefaultPropertyOverrider::label() const
{
    if (m_carriers.empty())
        return {};
    QString name = m_serviceObject ? m_serviceObject->objectName() : QString();
    if (name.isEmpty())
        name = typeName();
    if (const QIfAbstractZonedFeature *zoned = qobject_cast<const QIfAbstractZonedFeature *>(m_carriers.front()))
        // not translated; the zone API is fixed to English, too
        name += QString::fromLatin1(" [Zone: %1]").arg(zoned->zone());
    return name;
}

QString QIfDefaultPropertyOverrider::description() const
{
    if (!m_serviceObject)
        return {};
    return QString(QLatin1String("Backend Type: %1\nInstances in zone: %2"))
            .arg(QString::fromLatin1(m_serviceObject->metaObject()->className()))
            .arg(m_carriers.size());
}

QString QIfDefaultPropertyOverrider::typeName() const
{
    if (m_carriers.empty())
        return {};
    return QString::fromLatin1(m_carriers.front()->metaObject()->className());
}

bool QIfDefaultPropertyOverrider::setOverride(int index, bool isOverride)
{
    PropertyOverride &property = m_properties.at(index);
    if (property.isOverridable() && isOverride != property.isOverridden()) {
        if (!isOverride) {
            QByteArray flag = QString(QLatin1String("%1DirtyOverride")).arg(property.name()).toLatin1();
            for (const auto &carrier : m_carriers) {
                carrier->setProperty(flag.data(), true);
                property.setOverridenValue(property.m_originalValue, carrier);
            }
        }
        property.setOverriden(isOverride);
        return true;
    }
    return false;
}

bool QIfDefaultPropertyOverrider::setOverridenValue(int index, const QVariant &value)
{
    PropertyOverride &property = m_properties.at(index);
    bool res = false;
    for (const auto &carrier : m_carriers) {
        if (property.setOverridenValue(value, carrier))
            res = true;
    }
    return res;
}

int QIfDefaultPropertyOverrider::propertyCount() const
{
    return (int) m_properties.size();
}

const QIfDefaultPropertyOverrider::PropertyOverride &QIfDefaultPropertyOverrider::propertyForIndex(int index) const
{
    static QIfDefaultPropertyOverrider::PropertyOverride dummy;
    for (const auto &p: m_properties) {
        if (p.propertyIndex() == index)
            return p;
    }
    return dummy;
}

QIfDefaultPropertyOverrider::PropertyOverride &QIfDefaultPropertyOverrider::propertyForIndex(int index)
{
    static QIfDefaultPropertyOverrider::PropertyOverride dummy;
    for (auto &p: m_properties) {
        if (p.propertyIndex() == index)
            return p;
    }
    return dummy;
}

int QIfDefaultPropertyOverrider::indexOfProperty(const QByteArray &property) const
{
    for (uint i = 0; i < m_properties.size(); i++) {
        if (m_properties.at(i) == property) {
            return i;
        }
    }
    return -1;
}

bool QIfDefaultPropertyOverrider::handles(const QObject *carrier) const
{
    return std::find_if(m_carriers.begin(), m_carriers.end(), [carrier](const QIfAbstractFeature *p) {
        return p == carrier;
    }) != m_carriers.end();
}

QString QIfDefaultPropertyOverrider::serviceId() const
{
    return m_serviceObject ? m_serviceObject->id() : QString();
}

QT_END_NAMESPACE

#include "moc_qifdefaultpropertyoverrider_p.cpp"
