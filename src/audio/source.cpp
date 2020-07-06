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

#include "source.hpp"

using namespace audioreceiver::audio;

Source::Source(QObject *parent) : Service(parent) {
    deviceInfo = QAudioDeviceInfo::defaultInputDevice();

    audioInput = nullptr;
    audioIODevice = nullptr;

    frames = 0;
    bytes = 0;
}

Source::~Source() {
    delete audioInput;
    delete audioIODevice;
}

const QAudioDeviceInfo &Source::getDeviceInfo() const {
    return deviceInfo;
}

void Source::setDeviceInfo(const QAudioDeviceInfo &value) {
    Source::deviceInfo = value;
}

const QAudioFormat &Source::getAudioFormat() const {
    return audioFormat;
}

void Source::setAudioFormat(const QAudioFormat &value) {
    Source::audioFormat = deviceInfo.nearestFormat(value);
}

quint64 Source::getFrames() const {
    return frames;
}

quint64 Source::getBytes() const {
    return bytes;
}

void Source::start() {
    qDebug() << "Source Audio device:" << deviceInfo.deviceName();
    qDebug() << "Source Audio format:" << audioFormat;

    audioInput = new QAudioInput(deviceInfo, audioFormat);
    audioInput->setBufferSize(AUDIO_BUFFER_INPUT);

    audioIODevice = audioInput->start();
    connect(audioIODevice, &QIODevice::readyRead, this, &Source::readAudioBytes);
}

void Source::stop() {
    disconnect(audioIODevice, &QIODevice::readyRead, this, &Source::readAudioBytes);
    audioInput->stop();

    audioInput->deleteLater();
}

void Source::readAudioBytes() {
    frames++;

    QByteArray rawData = audioIODevice->readAll();
    int length = rawData.length();
    if (length == 0)
        return;

    auto bytesRead = (qint32) length;
    bytes += bytesRead;

    QMetaObject::invokeMethod(this, "newFrame", Qt::QueuedConnection, Q_ARG(const QByteArray, rawData));
}
