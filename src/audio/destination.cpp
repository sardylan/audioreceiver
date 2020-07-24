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

#include "destination.hpp"

#include "../dsp/utility.hpp"

using namespace audioreceiver::audio;

Destination::Destination(QObject *parent) : Service(parent) {
    deviceInfo = QAudioDeviceInfo::defaultOutputDevice();

    audioOutput = nullptr;
    audioIODevice = nullptr;

    frames = 0;
    bytes = 0;
}

Destination::~Destination() {
    delete audioOutput;
}

const QAudioDeviceInfo &Destination::getDeviceInfo() const {
    return deviceInfo;
}

void Destination::setDeviceInfo(const QAudioDeviceInfo &value) {
    Destination::deviceInfo = value;
}

const QAudioFormat &Destination::getAudioFormat() const {
    return audioFormat;
}

void Destination::setAudioFormat(const QAudioFormat &value) {
    Destination::audioFormat = deviceInfo.nearestFormat(value);
}

quint64 Destination::getFrames() const {
    return frames;
}

quint64 Destination::getBytes() const {
    return bytes;
}

void Destination::start() {
    qDebug() << "Destination Audio device:" << deviceInfo.deviceName();
    qDebug() << "Destination Audio format:" << audioFormat;

    audioOutput = new QAudioOutput(deviceInfo, audioFormat);
    audioOutput->setBufferSize(AUDIO_BUFFER_OUTPUT);

    audioIODevice = audioOutput->start();
}

void Destination::stop() {
    audioOutput->stop();

    audioIODevice = nullptr;

    audioOutput->deleteLater();
}

void Destination::newValues(const QList<qreal> &values) {
    if (audioIODevice == nullptr)
        return;

    QByteArray data = dsp::Utility::valuesToBytes(values, audioFormat);

    frames++;

    int dataLength = data.length();
    bytes += dataLength;

    qint64 writtenBytes = audioIODevice->write(data);

    if (writtenBytes != dataLength)
        qWarning() << "Unable to write all data to audio device"
                   << "-"
                   << writtenBytes << "of" << dataLength << "bytes written"
                   << "-"
                   << "Difference:" << dataLength - writtenBytes;
}
