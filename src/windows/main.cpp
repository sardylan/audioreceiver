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

Main::Main(audioreceiver::Config *config, audioreceiver::Status *status, QWidget *parent) : QMainWindow(parent),
                                                                                            ui(new Ui::Main) {
    ui->setupUi(this);

    Main::config = config;
    Main::status = status;

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
    connect(status, &Status::updateRunning, this, &Main::updateRunning);

    connect(clockTimer, &QTimer::timeout, this, &Main::updateClock);

    connect(ui->actionExit, &QAction::triggered, this, &Main::close);
    connect(ui->actionConfig, &QAction::triggered, this, &Main::openConfigWindow);
    connect(ui->actionRun, &QAction::triggered, this, &Main::handleRunAction);
}

void Main::initUi() {
    setWindowTitle(QString("%1 %2").arg(QApplication::applicationName()).arg(QApplication::applicationVersion()));
    ui->vuMeterStackedWidget->addWidget(vuMeter);
}

void Main::updateClock() {
    statusBarClockLabel->setText(QDateTime::currentDateTimeUtc().toString("yyyy-MM-dd hh:mm:ss (UTC)"));
}

void Main::handleRunAction() {
    if (ui->actionRun->isChecked())
        QMetaObject::invokeMethod(this, &Main::startAudioWorker, Qt::QueuedConnection);
    else
        QMetaObject::invokeMethod(this, &Main::stopAudioWorker, Qt::QueuedConnection);
}

void Main::updateRunning(bool value) {
    ui->actionRun->setChecked(value);
}
