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

#include <QtCore/QDebug>

#include "audio_source.hpp"

using namespace audioreceiver;

AudioSource::AudioSource(QObject *parent) : Service(parent) {
    deviceInfo = QAudioDeviceInfo::defaultInputDevice();

    audioInput = nullptr;
    audioIODevice = nullptr;

    frames = 0;
    bytes = 0;
}

AudioSource::~AudioSource() {
    delete audioInput;
    delete audioIODevice;
}

const QAudioDeviceInfo &AudioSource::getDeviceInfo() const {
    return deviceInfo;
}

void AudioSource::setDeviceInfo(const QAudioDeviceInfo &value) {
    AudioSource::deviceInfo = value;
}

const QAudioFormat &AudioSource::getAudioFormat() const {
    return audioFormat;
}

void AudioSource::setAudioFormat(const QAudioFormat &value) {
    AudioSource::audioFormat = deviceInfo.nearestFormat(value);
}

quint64 AudioSource::getFrames() const {
    return frames;
}

quint64 AudioSource::getBytes() const {
    return bytes;
}

void AudioSource::start() {
    qDebug() << "Source Audio device:" << deviceInfo.deviceName();
    qDebug() << "Source Audio format:" << audioFormat;

    audioInput = new QAudioInput(deviceInfo, audioFormat);
    audioInput->setBufferSize(AUDIO_BUFFER_INPUT);

    audioIODevice = audioInput->start();
    connect(audioIODevice, &QIODevice::readyRead, this, &AudioSource::readAudioBytes);
}

void AudioSource::stop() {
    disconnect(audioIODevice, &QIODevice::readyRead, this, &AudioSource::readAudioBytes);
    audioInput->stop();

    audioInput->deleteLater();
}

void AudioSource::readAudioBytes() {
    frames++;

    QByteArray rawData = audioIODevice->readAll();
    int length = rawData.length();
    if (length == 0)
        return;

    auto bytesRead = (qint32) length;
    bytes += bytesRead;

    QMetaObject::invokeMethod(this, "newFrame", Qt::QueuedConnection, Q_ARG(const QByteArray, rawData));
}
