// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2017 Klaralvdalens Datakonsult AB (KDAB).
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QIFDEFAULTPROPERTYOVERRIDER_P_H
#define QIFDEFAULTPROPERTYOVERRIDER_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail. This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <QMetaProperty>
#include <private/qtifglobal_p.h>
#include <vector>

#include "qifabstractfeature_p.h"

QT_BEGIN_NAMESPACE

class Q_QTINTERFACEFRAMEWORK_EXPORT QIfDefaultPropertyOverrider: public QObject, public QIfPropertyOverrider
{
    Q_OBJECT

public:
    explicit QIfDefaultPropertyOverrider(QIfAbstractFeature *carrier, QObject *parent = nullptr);
    ~QIfDefaultPropertyOverrider() override;

    void addCarrier(QIfAbstractFeature *carrier);
    void removeCarrier(QIfAbstractFeature *carrier);
    void setCarrierOverride(bool override, QIfAbstractFeature *carrier);
    int numCarriers() const;

    bool handles(const QObject *carrier) const;
    QString serviceId() const;

    QVariant property(int propertyIndex) const override;
    void setProperty(int propertyIndex, const QVariant &value) override;
    bool isOverridden(int propertyIndex) const override;

    bool isWritableAt(int index) const;
    bool isOverridableAt(int index) const;
    bool isAvailableAt(int index) const;
    bool isOverriddenAt(int index) const;
    bool hasNotifySignalAt(int index) const;

    virtual QString nameAt(int index) const;
    virtual QString typeNameAt(int index) const;
    virtual QString displayTextAt(int index) const;
    virtual QVariant ifConstraintsAt(int index) const;
    virtual QVariant editValueAt(int index) const;

    virtual QString label() const;
    virtual QString description() const;
    virtual QString typeName() const;

    bool setOverride(int index, bool isOverride);
    virtual bool setOverridenValue(int index, const QVariant &value);
    int propertyCount() const;

    int indexOfProperty(const QByteArray &property) const;

signals:
    void propertyChanged();

protected:

    class Q_QTINTERFACEFRAMEWORK_EXPORT PropertyOverride
    {
        friend class QIfDefaultPropertyOverrider;

    public:
        PropertyOverride();
        explicit PropertyOverride(const QMetaProperty &metaProperty, const QVariant &value, bool overridable);
        PropertyOverride(PropertyOverride &&other);

        int propertyIndex() const;
        bool isValid() const;
        bool isAvailable() const;
        bool hasNotifySignal() const;
        bool isWritable() const;
        bool isOverridable() const;
        bool isOverridden() const;
        QString name() const;
        QString typeName() const;
        QString displayText() const;
        QVariant editValue() const;
        QVariant cppValue() const;
        QVariant ifConstraints(QIfAbstractFeature *carrier) const;

        void setOverriden(bool override);
        bool setOverridenValue(const QVariant &value, QIfAbstractFeature *carrier);
        bool notifyOverridenValue(const QVariant &value, QIfAbstractFeature *carrier);
        void setOriginalValue(const QVariant &editValue);

        PropertyOverride &operator=(PropertyOverride &&other);
        bool operator==(const QByteArray &property) const;

    private:
        QMetaProperty m_metaProperty;
        bool m_overridable;
        bool m_overriding;
        QVariant m_originalValue;
        QVariant m_overridenValue;
    };

    const PropertyOverride &propertyForIndex(int index) const;
    PropertyOverride &propertyForIndex(int index);
    void init(QIfAbstractFeature *carrier);

    QIfServiceObject *m_serviceObject;
    std::vector<QIfAbstractFeature *> m_carriers;
    std::vector<PropertyOverride> m_properties;
};

QT_END_NAMESPACE

#endif // QIFDEFAULTPROPERTYOVERRIDER_P_H
