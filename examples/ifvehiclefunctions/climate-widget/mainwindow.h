// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtIfVehicleFunctions/QIfClimateControl>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}

class QButtonGroup;
class QAbstractButton;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void setAirRecirculationEnabled(bool enabled);
    void onAirRecirculationModeChanged(QtIfVehicleFunctions::RecirculationMode mode);
    void setupFlowDirectionRadioButtons(QtIfVehicleFunctions::AirflowDirections direction);
    void onFlowDirectionButtonToggled(QAbstractButton *button, bool checked);
private:

    Ui::MainWindow *ui;
    QButtonGroup *m_buttonGroup;
    QIfClimateControl *m_climateControl;
};

#endif // MAINWINDOW_H
