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

#include <QtMultimedia/QAudioFormat>
#include <QtMultimedia/QAudioDeviceInfo>

#include "config.hpp"
#include "ui_config.h"

using namespace audioreceiver::windows;

Config::Config(QWidget *parent) : QDialog(parent), ui(new Ui::Config) {
    ui->setupUi(this);

    Config::config = audioreceiver::Config::getInstance();
    Config::status = audioreceiver::Status::getInstance();

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
    ui->audioInputDeviceComboBox->clear();
    ui->audioInputDeviceComboBox->addItem("");

    for (const QAudioDeviceInfo &audioDeviceInfo: QAudioDeviceInfo::availableDevices(QAudio::AudioInput))
        ui->audioInputDeviceComboBox->addItem(audioDeviceInfo.deviceName(), QVariant::fromValue(audioDeviceInfo));

    ui->audioOutputDeviceComboBox->clear();
    ui->audioOutputDeviceComboBox->addItem("");

    for (const QAudioDeviceInfo &audioDeviceInfo: QAudioDeviceInfo::availableDevices(QAudio::AudioOutput))
        ui->audioOutputDeviceComboBox->addItem(audioDeviceInfo.deviceName(), QVariant::fromValue(audioDeviceInfo));
}

void Config::load() {
    QMetaObject::invokeMethod(config, &audioreceiver::Config::load);

    int index = -1;
    for (int i = 0; i < ui->audioInputDeviceComboBox->count(); i++)
        if (ui->audioInputDeviceComboBox->itemData(i).value<QAudioDeviceInfo>().deviceName() ==
            config->getAudioInputDevice()) {
            index = i;
            break;
        }

    if (index != -1) {
        ui->audioInputDeviceComboBox->setCurrentIndex(index);
    } else {
        ui->audioInputDeviceComboBox->setItemText(0, QString("%1 [NOT PRESENT]").arg(config->getAudioInputDevice()));
        ui->audioInputDeviceComboBox->setCurrentIndex(0);
    }

    updateInputDeviceOptions();

    index = -1;
    for (int i = 0; i < ui->audioOutputDeviceComboBox->count(); i++)
        if (ui->audioOutputDeviceComboBox->itemData(i).value<QAudioDeviceInfo>().deviceName() ==
            config->getAudioOutputDevice()) {
            index = i;
            break;
        }

    if (index != -1) {
        ui->audioOutputDeviceComboBox->setCurrentIndex(index);
    } else {
        ui->audioOutputDeviceComboBox->setItemText(0, QString("%1 [NOT PRESENT]").arg(config->getAudioOutputDevice()));
        ui->audioOutputDeviceComboBox->setCurrentIndex(0);
    }

    updateOutputDeviceOptions();
}

void Config::save() {
    config->setAudioInputDevice(ui->audioInputDeviceComboBox->currentData().value<QAudioDeviceInfo>().deviceName());
    config->setAudioInputChannels(ui->audioInputChannelsComboBox->currentData().value<int>());
    config->setAudioInputSampleRate(ui->audioInputSampleRateComboBox->currentData().value<int>());
    config->setAudioInputSampleSize(ui->audioInputSampleSizeComboBox->currentData().value<int>());
    config->setAudioInputSampleType(ui->audioInputSampleTypeComboBox->currentData().value<QAudioFormat::SampleType>());

    config->setAudioOutputDevice(ui->audioOutputDeviceComboBox->currentData().value<QAudioDeviceInfo>().deviceName());
    config->setAudioOutputChannels(ui->audioOutputChannelsComboBox->currentData().value<int>());
    config->setAudioOutputSampleRate(ui->audioOutputSampleRateComboBox->currentData().value<int>());
    config->setAudioOutputSampleSize(ui->audioOutputSampleSizeComboBox->currentData().value<int>());
    config->setAudioOutputSampleType(
            ui->audioOutputSampleTypeComboBox->currentData().value<QAudioFormat::SampleType>());

    QMetaObject::invokeMethod(config, &audioreceiver::Config::save);
}

void Config::checkStatus() {
    bool enabled = !status->isRunning();

    ui->audioInputDeviceComboBox->setEnabled(enabled);
    ui->audioInputChannelsComboBox->setEnabled(enabled);
    ui->audioInputSampleRateComboBox->setEnabled(enabled);
    ui->audioInputSampleSizeComboBox->setEnabled(enabled);
    ui->audioInputSampleTypeComboBox->setEnabled(enabled);

    ui->audioOutputDeviceComboBox->setEnabled(enabled);
    ui->audioOutputChannelsComboBox->setEnabled(enabled);
    ui->audioOutputSampleRateComboBox->setEnabled(enabled);
    ui->audioOutputSampleSizeComboBox->setEnabled(enabled);
    ui->audioOutputSampleTypeComboBox->setEnabled(enabled);
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

    ui->audioInputChannelsComboBox->clear();
    for (int channelCount: audioDeviceInfo.supportedChannelCounts())
        ui->audioInputChannelsComboBox->addItem(QString("%1").arg(channelCount), QVariant::fromValue(channelCount));

    for (int i = 0; i < ui->audioInputChannelsComboBox->count(); i++)
        if (ui->audioInputChannelsComboBox->itemData(i).value<int>() == config->getAudioInputChannels()) {
            ui->audioInputChannelsComboBox->setCurrentIndex(i);
            break;
        }

    ui->audioInputSampleRateComboBox->clear();
    for (int sampleRate: audioDeviceInfo.supportedSampleRates())
        ui->audioInputSampleRateComboBox->addItem(QString("%1 Hz").arg(sampleRate), QVariant::fromValue(sampleRate));

    for (int i = 0; i < ui->audioInputSampleRateComboBox->count(); i++)
        if (ui->audioInputSampleRateComboBox->itemData(i).value<int>() == config->getAudioInputSampleRate()) {
            ui->audioInputSampleRateComboBox->setCurrentIndex(i);
            break;
        }

    ui->audioInputSampleSizeComboBox->clear();
    for (int sampleSize: audioDeviceInfo.supportedSampleSizes())
        ui->audioInputSampleSizeComboBox->addItem(QString("%1 bit").arg(sampleSize), QVariant::fromValue(sampleSize));

    for (int i = 0; i < ui->audioInputSampleSizeComboBox->count(); i++)
        if (ui->audioInputSampleSizeComboBox->itemData(i).value<int>() == config->getAudioInputSampleSize()) {
            ui->audioInputSampleSizeComboBox->setCurrentIndex(i);
            break;
        }

    ui->audioInputSampleTypeComboBox->clear();
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

        ui->audioInputSampleTypeComboBox->addItem(text, QVariant::fromValue(sampleType));
    }

    for (int i = 0; i < ui->audioInputSampleTypeComboBox->count(); i++)
        if (ui->audioInputSampleTypeComboBox->itemData(i).value<QAudioFormat::SampleType>() ==
            config->getAudioInputSampleType()) {
            ui->audioInputSampleTypeComboBox->setCurrentIndex(i);
            break;
        }
}

void Config::updateOutputDeviceOptions() {
    const QAudioDeviceInfo &audioDeviceInfo = ui->audioOutputDeviceComboBox->currentData().value<QAudioDeviceInfo>();

    ui->audioOutputChannelsComboBox->clear();
    for (int channelCount: audioDeviceInfo.supportedChannelCounts())
        ui->audioOutputChannelsComboBox->addItem(QString("%1").arg(channelCount), QVariant::fromValue(channelCount));

    for (int i = 0; i < ui->audioOutputChannelsComboBox->count(); i++)
        if (ui->audioOutputChannelsComboBox->itemData(i).value<int>() == config->getAudioOutputChannels()) {
            ui->audioOutputChannelsComboBox->setCurrentIndex(i);
            break;
        }

    ui->audioOutputSampleRateComboBox->clear();
    for (int sampleRate: audioDeviceInfo.supportedSampleRates())
        ui->audioOutputSampleRateComboBox->addItem(QString("%1 Hz").arg(sampleRate), QVariant::fromValue(sampleRate));

    for (int i = 0; i < ui->audioOutputSampleRateComboBox->count(); i++)
        if (ui->audioOutputSampleRateComboBox->itemData(i).value<int>() == config->getAudioOutputSampleRate()) {
            ui->audioOutputSampleRateComboBox->setCurrentIndex(i);
            break;
        }

    ui->audioOutputSampleSizeComboBox->clear();
    for (int sampleSize: audioDeviceInfo.supportedSampleSizes())
        ui->audioOutputSampleSizeComboBox->addItem(QString("%1 bit").arg(sampleSize), QVariant::fromValue(sampleSize));

    for (int i = 0; i < ui->audioOutputSampleSizeComboBox->count(); i++)
        if (ui->audioOutputSampleSizeComboBox->itemData(i).value<int>() == config->getAudioOutputSampleSize()) {
            ui->audioOutputSampleSizeComboBox->setCurrentIndex(i);
            break;
        }

    ui->audioOutputSampleTypeComboBox->clear();
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

        ui->audioOutputSampleTypeComboBox->addItem(text, QVariant::fromValue(sampleType));
    }

    for (int i = 0; i < ui->audioOutputSampleTypeComboBox->count(); i++)
        if (ui->audioOutputSampleTypeComboBox->itemData(i).value<QAudioFormat::SampleType>() ==
            config->getAudioOutputSampleType()) {
            ui->audioOutputSampleTypeComboBox->setCurrentIndex(i);
            break;
        }
}
