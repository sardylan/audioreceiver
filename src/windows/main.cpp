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
    waterfall = new widgets::Waterfall(this);

    clockTimer = new QTimer(this);

    statusBarClockLabel = new QLabel(this);
    statusBarVersionLabel = new QLabel(this);
    statusBarAudioInputDeviceLabel = new QLabel(this);
    statusBarAudioInputFormatLabel = new QLabel(this);
    statusBarDSPSettingsLabel = new QLabel(this);

    signalConnect();
    initUi();
    initStatusBar();
}

Main::~Main() {
    delete waterfall;
    delete vuMeter;

    delete clockTimer;

    delete statusBarClockLabel;
    delete statusBarVersionLabel;
    delete statusBarAudioInputDeviceLabel;
    delete statusBarAudioInputFormatLabel;
    delete statusBarDSPSettingsLabel;

    delete ui;
}

void Main::updateUiFromConfig(const QAudioDeviceInfo &inputAudioDeviceInfo,
                              const QAudioFormat &inputAudioFormat,
                              const QAudioDeviceInfo &outputAudioDeviceInfo,
                              const QAudioFormat &outputAudioFormat) {
    statusBarAudioInputDeviceLabel->setText(inputAudioDeviceInfo.deviceName());
    statusBarAudioInputFormatLabel->setText(prepareAudioFormatString(inputAudioFormat));

    statusBarDSPSettingsLabel->setText(QString("Audio: %1 - FFT: %2")
                                               .arg(config->getDSPSettingsAudioChunkSize())
                                               .arg(config->getDSPSettingsFFTSampleSize()));

    int sampleFrequency = inputAudioFormat.sampleRate() / 2;

    ui->bfoFrequencySlider->setMaximum(sampleFrequency);
    waterfall->setFrequency(sampleFrequency);
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

    waterfall->setEnabled(value);
}

void Main::updateBufferSize(const int &size) {
    ui->audioInputBufferValue->setText(QString("%1").arg(size));

    qreal value = (qreal) size / AUDIORECEIVER_AUDIO_BUFFER_SIZE_MAX;
    if (value < 0)
        value = 0;
    if (value > 1)
        value = 1;

    ui->audioInputBufferProgressBar->setValue((int) (value * 100));
}

void Main::updateVuMeter(const qreal &value) {
    vuMeter->setValue(value);
}

void Main::updateWaterfall(const QList<qreal> &values) {
    QMetaObject::invokeMethod(waterfall, "addData", Qt::QueuedConnection, Q_ARG(const QList<qreal>, values));
}

void Main::updateGainValue() {
    int value = ui->gainSlider->value();
    qreal gain = (qreal) value / AUDIORECEIVER_AUDIO_GAIN_RESOLUTION;

    ui->gainValue->setText(QString::asprintf("%.02f", gain));

    QMetaObject::invokeMethod(this, "newGainValue", Qt::QueuedConnection, Q_ARG(qreal, gain));
}

void Main::updateBFOEnabled() {
    bool status = ui->bfoEnableCheckBox->isChecked();

    waterfall->setBfoEnabled(status);

    ui->bfoFrequencySlider->setEnabled(status);
    ui->bfoFrequencyValue->setEnabled(status);
}

void Main::updateBFOValue(int value) {
    ui->bfoFrequencyValue->setText(QString("%1 Hz").arg(value));
    waterfall->setBfoFrequency(value);
    QMetaObject::invokeMethod(this, "newBFOFrequency", Qt::QueuedConnection, Q_ARG(unsigned int, value));
}

void Main::signalConnect() {
    connect(clockTimer, &QTimer::timeout, this, &Main::updateClock);

    connect(ui->actionExit, &QAction::triggered, this, &Main::close);
    connect(ui->actionConfig, &QAction::triggered, this, &Main::openConfigWindow, Qt::QueuedConnection);
    connect(ui->actionRun, &QAction::triggered, this, &Main::audioWorkerToggle, Qt::QueuedConnection);

    connect(ui->bfoEnableCheckBox, &QCheckBox::toggled, this, &Main::updateBFOEnabled);
    connect(ui->bfoEnableCheckBox, &QCheckBox::toggled, this, &Main::bfoToggle, Qt::QueuedConnection);

    connect(ui->gainSlider, &QSlider::valueChanged, this, &Main::updateGainValue);
    connect(ui->bfoFrequencySlider, &QSlider::valueChanged, this, &Main::updateBFOValue);

    connect(waterfall, &widgets::Waterfall::newClickFrequency, ui->bfoFrequencySlider, &QSlider::setValue);
    connect(waterfall, &widgets::Waterfall::newClickFrequency, this, &Main::updateBFOValue);
}

void Main::initUi() {
    ui->bfoFrequencySlider->setMinimum(0);
    ui->bfoFrequencySlider->setMaximum(1);
    ui->bfoFrequencySlider->setValue(0);
    updateBFOValue(ui->bfoFrequencySlider->value());

    ui->bfoEnableCheckBox->setChecked(false);
    updateBFOEnabled();

    setWindowTitle(QString("%1 %2").arg(QApplication::applicationName()).arg(QApplication::applicationVersion()));

    ui->gainSlider->setMinimum(0);
    ui->gainSlider->setMaximum(AUDIORECEIVER_AUDIO_GAIN_MAX_DB * AUDIORECEIVER_AUDIO_GAIN_RESOLUTION);
    ui->gainSlider->setValue(1 * AUDIORECEIVER_AUDIO_GAIN_RESOLUTION);
    updateGainValue();

    ui->vuMeterStackedWidget->addWidget(vuMeter);
    ui->waterfallStackedWidget->addWidget(waterfall);
}

void Main::initStatusBar() {
    statusBarWidgetStyle(statusBarVersionLabel);
    statusBarVersionLabel->setText(QString("ver %1").arg(QApplication::applicationVersion()));
    ui->statusBar->addPermanentWidget(statusBarVersionLabel);

    clockTimer->setInterval(1000);
    clockTimer->setSingleShot(false);
    clockTimer->setTimerType(Qt::VeryCoarseTimer);
    clockTimer->start();
    statusBarWidgetStyle(statusBarClockLabel);
    ui->statusBar->addPermanentWidget(statusBarClockLabel);

    statusBarWidgetStyle(statusBarAudioInputDeviceLabel);
    ui->statusBar->addWidget(statusBarAudioInputDeviceLabel);

    statusBarWidgetStyle(statusBarAudioInputFormatLabel);
    ui->statusBar->addWidget(statusBarAudioInputFormatLabel);

    statusBarWidgetStyle(statusBarDSPSettingsLabel);
    ui->statusBar->addWidget(statusBarDSPSettingsLabel);
}

void Main::statusBarWidgetStyle(QLabel *label) {
    label->setFrameShape(QFrame::StyledPanel);
    label->setAlignment(Qt::AlignRight);
    label->setMinimumWidth(50);
    label->setStyleSheet("padding-left: 3px; padding-right: 3px;");
}

void Main::updateClock() {
    statusBarClockLabel->setText(QDateTime::currentDateTimeUtc().toString("yyyy-MM-dd hh:mm:ss (UTC)"));
}

QString Main::prepareAudioFormatString(const QAudioFormat &inputAudioFormat) {
    QString type;
    switch (inputAudioFormat.sampleType()) {
        case QAudioFormat::SignedInt:
            type = "s";
            break;
        case QAudioFormat::UnSignedInt:
            type = "u";
            break;
        case QAudioFormat::Float:
            type = "f";
            break;
        default:
            type = "";
    }

    QString endian = inputAudioFormat.byteOrder() == QAudioFormat::BigEndian ? "be" : "le";

    return QString("%1Hz %2ch %3%4%5 %6")
            .arg(inputAudioFormat.sampleRate())
            .arg(inputAudioFormat.channelCount())
            .arg(type)
            .arg(inputAudioFormat.sampleSize())
            .arg(endian)
            .arg(inputAudioFormat.codec());
}
