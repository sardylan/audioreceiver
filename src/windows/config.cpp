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

#include <QtWidgets/QComboBox>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QDialogButtonBox>

#include "config.hpp"
#include "ui_config.h"

using namespace audioreceiver::windows;

Config::Config(audioreceiver::Config *config, audioreceiver::Status *status, QWidget *parent) : QDialog(parent),
                                                                                                ui(new Ui::Config) {
    ui->setupUi(this);

    Config::config = config;
    Config::status = status;

    signalConnect();
    initUi();

    load();
    checkStatus();
}

Config::~Config() {
    delete ui;
}

void Config::signalConnect() {
    connect(ui->buttonBox->button(QDialogButtonBox::Ok), &QPushButton::clicked, this, &Config::handleOK);
    connect(ui->buttonBox->button(QDialogButtonBox::Apply), &QPushButton::clicked, this, &Config::handleApply);
    connect(ui->buttonBox->button(QDialogButtonBox::Close), &QPushButton::clicked, this, &Config::handleClose);
    connect(ui->buttonBox->button(QDialogButtonBox::Reset), &QPushButton::clicked, this, &Config::handleReset);

    connect(ui->audioInputDeviceComboBox,
            static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
            this,
            &Config::updateInputDeviceOptions);

    connect(ui->audioOutputDeviceComboBox,
            static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
            this,
            &Config::updateOutputDeviceOptions);
}

void Config::initUi() {
    initDeviceComboBox(ui->audioInputDeviceComboBox, QAudio::AudioInput);
    initDeviceComboBox(ui->audioOutputDeviceComboBox, QAudio::AudioOutput);
}

void Config::load() {
    QMetaObject::invokeMethod(config, &audioreceiver::Config::load);

    updateDeviceComboBox(ui->audioInputDeviceComboBox, config->getAudioInputDevice());
    updateInputDeviceOptions();

    updateDeviceComboBox(ui->audioOutputDeviceComboBox, config->getAudioOutputDevice());
    updateOutputDeviceOptions();
}

void Config::save() {
    config->setAudioInputDevice(ui->audioInputDeviceComboBox->currentData().value<QAudioDeviceInfo>().deviceName());
    config->setAudioInputChannels(ui->audioInputChannelsComboBox->currentData().value<int>());
    config->setAudioInputSampleRate(ui->audioInputSampleRateComboBox->currentData().value<int>());
    config->setAudioInputSampleSize(ui->audioInputSampleSizeComboBox->currentData().value<int>());
    config->setAudioInputSampleType(ui->audioInputSampleTypeComboBox->currentData().value<QAudioFormat::SampleType>());
    config->setAudioInputCodec(ui->audioInputCodecComboBox->currentData().value<QString>());

    config->setAudioOutputDevice(ui->audioOutputDeviceComboBox->currentData().value<QAudioDeviceInfo>().deviceName());
    config->setAudioOutputChannels(ui->audioOutputChannelsComboBox->currentData().value<int>());
    config->setAudioOutputSampleRate(ui->audioOutputSampleRateComboBox->currentData().value<int>());
    config->setAudioOutputSampleSize(ui->audioOutputSampleSizeComboBox->currentData().value<int>());
    config->setAudioOutputSampleType(
            ui->audioOutputSampleTypeComboBox->currentData().value<QAudioFormat::SampleType>());
    config->setAudioOutputCodec(ui->audioOutputCodecComboBox->currentData().value<QString>());

    QMetaObject::invokeMethod(config, &audioreceiver::Config::save);
}

void Config::checkStatus() {
    bool enabled = !status->isRunning();

    ui->audioInputDeviceComboBox->setEnabled(enabled);
    ui->audioInputChannelsComboBox->setEnabled(enabled);
    ui->audioInputSampleRateComboBox->setEnabled(enabled);
    ui->audioInputSampleSizeComboBox->setEnabled(enabled);
    ui->audioInputSampleTypeComboBox->setEnabled(enabled);
    ui->audioInputCodecComboBox->setEnabled(enabled);

    ui->audioOutputDeviceComboBox->setEnabled(enabled);
    ui->audioOutputChannelsComboBox->setEnabled(enabled);
    ui->audioOutputSampleRateComboBox->setEnabled(enabled);
    ui->audioOutputSampleSizeComboBox->setEnabled(enabled);
    ui->audioOutputSampleTypeComboBox->setEnabled(enabled);
    ui->audioOutputCodecComboBox->setEnabled(enabled);
}

void Config::handleOK() {
    save();
    close();
}

void Config::handleApply() {
    save();
}

void Config::handleClose() {
    close();
}

void Config::handleReset() {
    load();
}

void Config::updateInputDeviceOptions() {
    const QAudioDeviceInfo &audioDeviceInfo = ui->audioInputDeviceComboBox->currentData().value<QAudioDeviceInfo>();

    updateChannelsComboBox(ui->audioInputChannelsComboBox, audioDeviceInfo, config->getAudioInputChannels());
    updateSampleRateComboBox(ui->audioInputSampleRateComboBox, audioDeviceInfo, config->getAudioInputSampleRate());
    updateSampleSizeComboBox(ui->audioInputSampleSizeComboBox, audioDeviceInfo, config->getAudioInputSampleSize());
    updateSampleTypeComboBox(ui->audioInputSampleTypeComboBox, audioDeviceInfo, config->getAudioInputSampleType());
    updateEndianComboBox(ui->audioInputEndianComboBox, audioDeviceInfo, config->getAudioInputEndian());
    updateCodecComboBox(ui->audioInputCodecComboBox, audioDeviceInfo, config->getAudioInputCodec());
}

void Config::updateOutputDeviceOptions() {
    const QAudioDeviceInfo &audioDeviceInfo = ui->audioOutputDeviceComboBox->currentData().value<QAudioDeviceInfo>();

    updateChannelsComboBox(ui->audioOutputChannelsComboBox, audioDeviceInfo, config->getAudioOutputChannels());
    updateSampleRateComboBox(ui->audioOutputSampleRateComboBox, audioDeviceInfo, config->getAudioOutputSampleRate());
    updateSampleSizeComboBox(ui->audioOutputSampleSizeComboBox, audioDeviceInfo, config->getAudioOutputSampleSize());
    updateSampleTypeComboBox(ui->audioOutputSampleTypeComboBox, audioDeviceInfo, config->getAudioOutputSampleType());
    updateEndianComboBox(ui->audioOutputEndianComboBox, audioDeviceInfo, config->getAudioOutputEndian());
    updateCodecComboBox(ui->audioOutputCodecComboBox, audioDeviceInfo, config->getAudioOutputCodec());
}

void Config::initDeviceComboBox(QComboBox *comboBox, QAudio::Mode mode) {
    comboBox->clear();
    comboBox->addItem("");

    for (const QAudioDeviceInfo &audioDeviceInfo: QAudioDeviceInfo::availableDevices(mode))
        comboBox->addItem(audioDeviceInfo.deviceName(), QVariant::fromValue(audioDeviceInfo));
}

void Config::updateDeviceComboBox(QComboBox *comboBox, QString currentValue) {
    int index = -1;
    for (int i = 0; i < comboBox->count(); i++)
        if (comboBox->itemData(i).value<QAudioDeviceInfo>().deviceName() == currentValue) {
            index = i;
            break;
        }

    if (index != -1) {
        comboBox->setCurrentIndex(index);
    } else {
        comboBox->setItemText(0, QString("%1 [NOT PRESENT]").arg(currentValue));
        comboBox->setCurrentIndex(0);
    }
}

void Config::updateChannelsComboBox(QComboBox *comboBox, const QAudioDeviceInfo &audioDeviceInfo, int currentValue) {
    comboBox->clear();
    for (int channelCount: audioDeviceInfo.supportedChannelCounts())
        comboBox->addItem(QString("%1").arg(channelCount), QVariant::fromValue(channelCount));

    for (int i = 0; i < comboBox->count(); i++)
        if (comboBox->itemData(i).value<int>() == currentValue) {
            comboBox->setCurrentIndex(i);
            break;
        }
}

void Config::updateSampleRateComboBox(QComboBox *comboBox, const QAudioDeviceInfo &audioDeviceInfo, int currentValue) {
    comboBox->clear();
    for (int sampleRate: audioDeviceInfo.supportedSampleRates())
        comboBox->addItem(QString("%1 Hz").arg(sampleRate), QVariant::fromValue(sampleRate));

    for (int i = 0; i < comboBox->count(); i++)
        if (comboBox->itemData(i).value<int>() == currentValue) {
            comboBox->setCurrentIndex(i);
            break;
        }
}

void Config::updateSampleSizeComboBox(QComboBox *comboBox, const QAudioDeviceInfo &audioDeviceInfo, int currentValue) {
    comboBox->clear();
    for (int sampleSize: audioDeviceInfo.supportedSampleSizes())
        comboBox->addItem(QString("%1 bit").arg(sampleSize), QVariant::fromValue(sampleSize));

    for (int i = 0; i < comboBox->count(); i++)
        if (comboBox->itemData(i).value<int>() == currentValue) {
            comboBox->setCurrentIndex(i);
            break;
        }
}

void Config::updateSampleTypeComboBox(QComboBox *comboBox, const QAudioDeviceInfo &audioDeviceInfo,
                                      QAudioFormat::SampleType currentValue) {
    comboBox->clear();
    for (QAudioFormat::SampleType sampleType: audioDeviceInfo.supportedSampleTypes()) {
        QString text;

        switch (sampleType) {
            case QAudioFormat::Unknown:
                text = "Unknown";
                break;
            case QAudioFormat::SignedInt:
                text = "Signed";
                break;
            case QAudioFormat::UnSignedInt:
                text = "Unsigned";
                break;
            case QAudioFormat::Float:
                text = "Float";
                break;
        }

        comboBox->addItem(text, QVariant::fromValue(sampleType));
    }

    for (int i = 0; i < comboBox->count(); i++)
        if (comboBox->itemData(i).value<QAudioFormat::SampleType>() == currentValue) {
            comboBox->setCurrentIndex(i);
            break;
        }

}

void Config::updateEndianComboBox(QComboBox *comboBox, const QAudioDeviceInfo &audioDeviceInfo,
                                  QAudioFormat::Endian currentValue) {
    comboBox->clear();
    for (QAudioFormat::Endian sampleType: audioDeviceInfo.supportedByteOrders()) {
        QString text;

        switch (sampleType) {
            case QAudioFormat::BigEndian:
                text = "BigEndian";
                break;
            case QAudioFormat::LittleEndian:
                text = "LittleEndian";
                break;
        }

        comboBox->addItem(text, QVariant::fromValue(sampleType));
    }

    for (int i = 0; i < comboBox->count(); i++)
        if (comboBox->itemData(i).value<QAudioFormat::Endian>() == currentValue) {
            comboBox->setCurrentIndex(i);
            break;
        }

}

void Config::updateCodecComboBox(QComboBox *comboBox, const QAudioDeviceInfo &audioDeviceInfo,
                                 const QString &currentValue) {
    comboBox->clear();
    for (const QString &codec: audioDeviceInfo.supportedCodecs())
        comboBox->addItem(codec, QVariant::fromValue(codec));

    for (int i = 0; i < comboBox->count(); i++)
        if (comboBox->itemData(i).value<QString>() == currentValue) {
            comboBox->setCurrentIndex(i);
            break;
        }
}
