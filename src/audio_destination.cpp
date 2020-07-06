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

#include "audio_destination.hpp"

using namespace audioreceiver;

AudioDestination::AudioDestination(QObject *parent) : Service(parent) {
    deviceInfo = QAudioDeviceInfo::defaultOutputDevice();

    audioOutput = nullptr;
    audioIODevice = nullptr;

    frames = 0;
    bytes = 0;
}

AudioDestination::~AudioDestination() {
    delete audioOutput;
    delete audioIODevice;
}

const QAudioDeviceInfo &AudioDestination::getDeviceInfo() const {
    return deviceInfo;
}

void AudioDestination::setDeviceInfo(const QAudioDeviceInfo &value) {
    AudioDestination::deviceInfo = value;
}

const QAudioFormat &AudioDestination::getAudioFormat() const {
    return audioFormat;
}

void AudioDestination::setAudioFormat(const QAudioFormat &value) {
    AudioDestination::audioFormat = deviceInfo.nearestFormat(value);
}

quint64 AudioDestination::getFrames() const {
    return frames;
}

quint64 AudioDestination::getBytes() const {
    return bytes;
}

void AudioDestination::start() {
    qDebug() << "Destination Audio device:" << deviceInfo.deviceName();
    qDebug() << "Destination Audio format:" << audioFormat;

    audioOutput = new QAudioOutput(deviceInfo, audioFormat);
    audioOutput->setBufferSize(AUDIO_BUFFER_OUTPUT);

    audioIODevice = audioOutput->start();
}

void AudioDestination::stop() {
    audioOutput->stop();

    audioIODevice = nullptr;

    audioOutput->deleteLater();
}

void AudioDestination::newFrame(const QByteArray &data) {
    if (audioIODevice == nullptr)
        return;

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
