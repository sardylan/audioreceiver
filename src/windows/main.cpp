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
    statusBarAudioInputDevice = new QLabel(this);
    statusBarAudioInputFormat = new QLabel(this);
    statusBarAudioOutputDevice = new QLabel(this);
    statusBarAudioOutputFormat = new QLabel(this);


    signalConnect();
    initUi();
    initStatusBar();
}

Main::~Main() {
    delete vuMeter;

    delete clockTimer;

    delete statusBarClockLabel;
    delete statusBarVersionLabel;
    delete statusBarAudioInputDevice;
    delete statusBarAudioInputFormat;
    delete statusBarAudioOutputDevice;
    delete statusBarAudioOutputFormat;

    delete ui;
}

void Main::updateAudioDevicesParams(QAudioDeviceInfo inputAudioDeviceInfo, QAudioFormat inputAudioFormat,
                                    QAudioDeviceInfo outputAudioDeviceInfo, QAudioFormat outputAudioFormat) {
    statusBarAudioInputDevice->setText(inputAudioDeviceInfo.deviceName());
    statusBarAudioInputFormat->setText(inputAudioFormat.codec());
    statusBarAudioOutputDevice->setText(outputAudioDeviceInfo.deviceName());
    statusBarAudioInputFormat->setText(outputAudioFormat.codec());

    ui->bfoFrequencySlider->setMaximum(inputAudioFormat.sampleRate() / 2);
}

void Main::updateWorkerStatus(bool value) {
    ui->gainValue->setEnabled(value);
    ui->gainSlider->setEnabled(value);
    ui->gainLabel->setEnabled(value);

    ui->bfoEnableCheckBox->setEnabled(value);
    if (value) {
        updateBFOEnabled();
    } else {
        ui->bfoFrequencySlider->setEnabled(false);
        ui->bfoFrequencyValue->setEnabled(false);
    }

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

    ui->statusBar->addWidget(statusBarAudioInputDevice);

    ui->statusBar->addWidget(statusBarAudioInputFormat);

    ui->statusBar->addWidget(statusBarAudioOutputDevice);

    ui->statusBar->addWidget(statusBarAudioOutputFormat);
}

void Main::updateBFOEnabled() {
    bool status = ui->bfoEnableCheckBox->isChecked();

    ui->bfoFrequencySlider->setEnabled(status);
    ui->bfoFrequencyValue->setEnabled(status);
}

void Main::updateBFOValue() {
    int value = ui->bfoFrequencySlider->value();
    ui->bfoFrequencyValue->setText(QString("%1 Hz").arg(value));
    QMetaObject::invokeMethod(this, "newBFOFrequency", Qt::QueuedConnection, Q_ARG(unsigned int, value));
}

void Main::signalConnect() {
    connect(clockTimer, &QTimer::timeout, this, &Main::updateClock);

    connect(ui->actionExit, &QAction::triggered, this, &Main::close);
    connect(ui->actionConfig, &QAction::triggered, this, &Main::openConfigWindow, Qt::QueuedConnection);
    connect(ui->actionRun, &QAction::triggered, this, &Main::audioWorkerToggle, Qt::QueuedConnection);

    connect(ui->bfoEnableCheckBox, &QCheckBox::toggled, this, &Main::updateBFOEnabled);
    connect(ui->bfoEnableCheckBox, &QCheckBox::toggled, this, &Main::bfoToggle, Qt::QueuedConnection);

    connect(ui->bfoFrequencySlider, &QSlider::valueChanged, this, &Main::updateBFOValue);
}

void Main::initUi() {
    ui->bfoFrequencySlider->setMinimum(0);
    ui->bfoFrequencySlider->setMaximum(1);
    ui->bfoFrequencySlider->setValue(0);
    updateBFOValue();

    ui->bfoEnableCheckBox->setChecked(false);
    updateBFOEnabled();

    setWindowTitle(QString("%1 %2").arg(QApplication::applicationName()).arg(QApplication::applicationVersion()));
    ui->vuMeterStackedWidget->addWidget(vuMeter);
}

void Main::updateClock() {
    statusBarClockLabel->setText(QDateTime::currentDateTimeUtc().toString("yyyy-MM-dd hh:mm:ss (UTC)"));
}
