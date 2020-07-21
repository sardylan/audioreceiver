/*
 * audioreceiver
 * Copyright (C) 2020  Luca Cireddu IS0GVH (sardylan@gmail.com, is0gvh@gmail.com)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */

#include "main.hpp"
#include "ui_main.h"

using namespace audioreceiver::windows;

Main::Main(audioreceiver::Config *config, QWidget *parent) : QMainWindow(parent), ui(new Ui::Main) {
    ui->setupUi(this);

    Main::config = config;

    vuMeter = new widgets::VUMeter(this);

    clockTimer = new QTimer(this);

    statusBarClockLabel = new QLabel(this);
    statusBarVersionLabel = new QLabel(this);

    signalConnect();
    initUi();
    initStatusBar();
}

Main::~Main() {
    delete vuMeter;

    delete clockTimer;

    delete statusBarClockLabel;
    delete statusBarVersionLabel;

    delete ui;
}

void Main::updateWorkerStatus(bool value) {
    ui->gainValue->setEnabled(value);
    ui->gainSlider->setEnabled(value);
    ui->gainLabel->setEnabled(value);

    ui->bfoEnableCheckBox->setEnabled(value);
    ui->bfoFrequencySlider->setEnabled(value);
    ui->bfoFrequencyValue->setEnabled(value);

    if (!value) {
        vuMeter->setMax(1);
        vuMeter->setMin(0);
        vuMeter->setValue(0);
        vuMeter->setEnabled(false);
    }
}

void Main::updateVuMeter(const qreal &value) {
    vuMeter->setValue(value);
}

void Main::initStatusBar() {
    statusBarVersionLabel->setFrameShape(QFrame::StyledPanel);
    statusBarVersionLabel->setAlignment(Qt::AlignRight);
    statusBarVersionLabel->setMinimumWidth(50);
    statusBarVersionLabel->setStyleSheet("padding-left: 3px; padding-right: 3px;");
    statusBarVersionLabel->setText(QString("ver %1").arg(QApplication::applicationVersion()));
    ui->statusBar->addPermanentWidget(statusBarVersionLabel);

    clockTimer->setInterval(1000);
    clockTimer->setSingleShot(false);
    clockTimer->setTimerType(Qt::VeryCoarseTimer);
    clockTimer->start();
    statusBarClockLabel->setFrameShape(QFrame::StyledPanel);
    statusBarClockLabel->setAlignment(Qt::AlignCenter);
    statusBarClockLabel->setMinimumWidth(50);
    statusBarClockLabel->setStyleSheet("padding-left: 3px; padding-right: 3px;");
    ui->statusBar->addPermanentWidget(statusBarClockLabel);
}

void Main::signalConnect() {
    connect(clockTimer, &QTimer::timeout, this, &Main::updateClock);

    connect(ui->actionExit, &QAction::triggered, this, &Main::close);
    connect(ui->actionConfig, &QAction::triggered, this, &Main::openConfigWindow, Qt::QueuedConnection);
    connect(ui->actionRun, &QAction::triggered, this, &Main::audioWorkerToggle, Qt::QueuedConnection);
}

void Main::initUi() {
    setWindowTitle(QString("%1 %2").arg(QApplication::applicationName()).arg(QApplication::applicationVersion()));
    ui->vuMeterStackedWidget->addWidget(vuMeter);
}

void Main::updateClock() {
    statusBarClockLabel->setText(QDateTime::currentDateTimeUtc().toString("yyyy-MM-dd hh:mm:ss (UTC)"));
}
