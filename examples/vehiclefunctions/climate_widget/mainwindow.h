/****************************************************************************
**
** Copyright (C) 2015 Pelagicore AG
** Contact: http://www.qt.io/ or http://www.pelagicore.com/
**
** This file is part of the QtIVI module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL3-PELAGICORE$
** Commercial License Usage
** Licensees holding valid commercial Qt IVI licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Pelagicore. For licensing terms
** and conditions, contact us at http://www.pelagicore.com.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPLv3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** $QT_END_LICENSE$
**
** SPDX-License-Identifier: LGPL-3.0
**
****************************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtIVIVehicleFunctions/QtIVIClimateControl>

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
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void onAirRecirculationAttributeChanged(const QtIVIPropertyAttribute<bool> &attribute);
    void onHeaterAttributeChanged(const QtIVIPropertyAttribute<bool> &attribute);
    void onAirConditioningAttributeChanged(const QtIVIPropertyAttribute<bool> &attribute);
    void setupFlowDirectionRadioButtons(QtIVIClimateControl::AirflowDirection direction);
    void setupFlowDirectionAttribute(const QtIVIPropertyAttribute<QtIVIClimateControl::AirflowDirection> &attribute);
    void onFlowDirectionButtonToggled(QAbstractButton * button, bool checked);
private:

    Ui::MainWindow *ui;
    QButtonGroup *m_radioButtonGroup;
    QtIVIClimateControl* m_climateControl;
};

#endif // MAINWINDOW_H
