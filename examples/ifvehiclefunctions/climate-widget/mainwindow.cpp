// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QSpinBox>
#include <QButtonGroup>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_buttonGroup(new QButtonGroup(this)),
    m_climateControl(nullptr)
{
    ui->setupUi(this);

    m_buttonGroup->setExclusive(false);
    m_buttonGroup->addButton(ui->cb_windshield);
    m_buttonGroup->addButton(ui->cb_dashboard);
    m_buttonGroup->addButton(ui->cb_floor);

//![1]
    m_climateControl = new QIfClimateControl(QString(), this);
    m_climateControl->setDiscoveryMode(QIfAbstractFeature::LoadOnlySimulationBackends);
    m_climateControl->startAutoDiscovery();

    if (!m_climateControl->isValid())
        QMessageBox::critical(this, tr("Auto Discovery Failed !"), tr("No Climate Backend available"));
//![1]

//![2]
    //Air Flow Direction
    setupFlowDirectionRadioButtons(m_climateControl->airflowDirections());
    connect(m_buttonGroup, static_cast<void (QButtonGroup::*)(QAbstractButton *, bool)>(&QButtonGroup::buttonToggled),
            this, &MainWindow::onFlowDirectionButtonToggled);

    connect(m_climateControl, &QIfClimateControl::airflowDirectionsChanged,
            this, &MainWindow::setupFlowDirectionRadioButtons);

    //Air Condition
    ui->cb_airCondition->setChecked(m_climateControl->isAirConditioningEnabled());
    connect(m_climateControl, &QIfClimateControl::airConditioningEnabledChanged,
            ui->cb_airCondition, &QCheckBox::setChecked);
    connect(ui->cb_airCondition, &QCheckBox::clicked,
            m_climateControl, &QIfClimateControl::setAirConditioningEnabled);

    //Air Recirculation
    ui->cb_airRecirculation->setChecked(m_climateControl->recirculationMode() == QtIfVehicleFunctions::RecirculationOn);
    connect(m_climateControl, &QIfClimateControl::recirculationModeChanged,
            this, &MainWindow::onAirRecirculationModeChanged);
    connect(ui->cb_airRecirculation, &QCheckBox::clicked,
            this, &MainWindow::setAirRecirculationEnabled);

    //Heater
    ui->cb_heater->setChecked(m_climateControl->isHeaterEnabled());
    connect(m_climateControl, &QIfClimateControl::heaterEnabledChanged,
            ui->cb_heater, &QCheckBox::setChecked);
    connect(ui->cb_heater, &QCheckBox::clicked,
            m_climateControl, &QIfClimateControl::setHeaterEnabled);
}
//![2]

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setAirRecirculationEnabled(bool enabled)
{
    if (enabled)
        m_climateControl->setRecirculationMode(QtIfVehicleFunctions::RecirculationOn);
    else
        m_climateControl->setRecirculationMode(QtIfVehicleFunctions::RecirculationOff);
}

void MainWindow::onAirRecirculationModeChanged(QtIfVehicleFunctions::RecirculationMode mode)
{
    ui->cb_airRecirculation->setChecked(mode == QtIfVehicleFunctions::RecirculationOn);
}

//![3]
void MainWindow::setupFlowDirectionRadioButtons(QtIfVehicleFunctions::AirflowDirections direction)
{
    ui->cb_windshield->setChecked(direction.testFlag(QtIfVehicleFunctions::Windshield));
    ui->cb_dashboard->setChecked(direction.testFlag(QtIfVehicleFunctions::Dashboard));
    ui->cb_floor->setChecked(direction.testFlag(QtIfVehicleFunctions::Floor));
}

void MainWindow::onFlowDirectionButtonToggled(QAbstractButton *button, bool checked)
{
    Q_UNUSED(button)
    Q_UNUSED(checked)

    QtIfVehicleFunctions::AirflowDirections direction;

    if (ui->cb_windshield->isChecked())
        direction |= QtIfVehicleFunctions::Windshield;
    if (ui->cb_dashboard->isChecked())
        direction |= QtIfVehicleFunctions::Dashboard;
    if (ui->cb_floor->isChecked())
        direction |= QtIfVehicleFunctions::Floor;

    m_climateControl->setAirflowDirections(direction);
}
//![3]
