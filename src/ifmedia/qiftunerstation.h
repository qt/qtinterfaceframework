// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QIFTUNERSTATION_H
#define QIFTUNERSTATION_H

#include <QtIfMedia/qtifmediaglobal.h>
#include <QtIfMedia/QIfAmFmTuner>
#include <QtInterfaceFramework/QIfStandardItem>

QT_BEGIN_NAMESPACE

class QIfTunerStationPrivate;
class QIfAmFmTunerStationPrivate;

class Q_QTIFMEDIA_EXPORT QIfTunerStation : public QIfStandardItem
{
    Q_GADGET

    Q_PROPERTY(QString stationName READ stationName WRITE setStationName)
    Q_PROPERTY(int frequency READ frequency WRITE setFrequency)
    Q_PROPERTY(QString stationLogoUrl READ stationLogoUrl WRITE setStationLogoUrl)
    Q_PROPERTY(QString category READ category WRITE setCategory)
    Q_PROPERTY(QString radioText READ radioText WRITE setRadioText)
    Q_PROPERTY(int receptionQuality READ receptionQuality WRITE setReceptionQuality)

public:
    QIfTunerStation();
    QIfTunerStation(const QIfTunerStation &);
    QIfTunerStation &operator=(const QIfTunerStation &);
    ~QIfTunerStation() override;

    virtual QString stationName() const;
    virtual void setStationName(const QString &stationName);
    virtual int frequency() const;
    virtual void setFrequency(int frequency);
    QString stationLogoUrl() const;
    void setStationLogoUrl(const QString &stationLogoUrl);
    QString category() const;
    void setCategory(const QString &category);
    QString radioText() const;
    void setRadioText(const QString &radioText);
    int receptionQuality() const;
    void setReceptionQuality(int receptionQuality);
    QString name() const override;
    QString type() const override;

    bool operator==(const QIfTunerStation &other) const;
    inline bool operator!=(const QIfTunerStation &other) const { return !(*this == other); }

private:
    QSharedDataPointer<QIfTunerStationPrivate> d;
};
Q_DECLARE_TYPEINFO(QIfTunerStation, Q_MOVABLE_TYPE);

class Q_QTIFMEDIA_EXPORT QIfAmFmTunerStation : public QIfTunerStation
{
    Q_GADGET

    Q_PROPERTY(QIfAmFmTuner::Band band READ band WRITE setBand)

public:
    QIfAmFmTunerStation();
    QIfAmFmTunerStation(const QIfAmFmTunerStation &);
    QIfAmFmTunerStation &operator=(const QIfAmFmTunerStation &);
    ~QIfAmFmTunerStation() override;

    virtual QIfAmFmTuner::Band band() const;
    virtual void setBand(QIfAmFmTuner::Band band);
    QString type() const override;

    bool operator==(const QIfAmFmTunerStation &other) const;
    inline bool operator!=(const QIfAmFmTunerStation &other) const { return !(*this == other); }

private:
    QSharedDataPointer<QIfAmFmTunerStationPrivate> d;
};
Q_DECLARE_TYPEINFO(QIfAmFmTunerStation, Q_MOVABLE_TYPE);

QT_END_NAMESPACE

Q_DECLARE_METATYPE(QIfTunerStation)
Q_DECLARE_METATYPE(QIfAmFmTunerStation)

#endif // QIFTUNERSTATION_H
