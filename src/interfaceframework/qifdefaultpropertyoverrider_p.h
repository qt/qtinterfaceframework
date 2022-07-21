/****************************************************************************
**
** Copyright (C) 2021 The Qt Company Ltd.
** Copyright (C) 2017 Klaralvdalens Datakonsult AB (KDAB).
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

#include <QtCore/QMetaProperty>
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
