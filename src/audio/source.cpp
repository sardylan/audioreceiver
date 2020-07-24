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

#include "../dsp/utility.hpp"

using namespace audioreceiver::audio;

Source::Source(const int &frameSize, QObject *parent) : Service(parent), frameSize(frameSize) {
    frameNumber = 0;

    deviceInfo = QAudioDeviceInfo::defaultInputDevice();

    audioInput = nullptr;
    audioIODevice = nullptr;

    frames = 0;
    bytes = 0;

    buffer = new QQueue<char>();
}

Source::~Source() {
    delete buffer;

    delete audioInput;
}

int Source::getFrameSize() const {
    return frameSize;
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

    buffer->clear();
    emitBufferSize();

    audioIODevice = audioInput->start();
    connect(audioIODevice, &QIODevice::readyRead, this, &Source::readAudioBytes);
}

void Source::stop() {
    disconnect(audioIODevice, &QIODevice::readyRead, this, &Source::readAudioBytes);
    audioInput->stop();

    audioIODevice = nullptr;

    audioInput->deleteLater();

    buffer->clear();
    emitBufferSize();
}

void Source::readAudioBytes() {
    frames++;

    QByteArray rawData = audioIODevice->readAll();
    int length = rawData.length();
    if (length == 0)
        return;

    auto bytesRead = (qint32) length;
    bytes += bytesRead;

    for (char c: rawData)
        buffer->append(c);

    int bytesToRead = audioFormat.bytesForFrames(frameSize);

    while (buffer->size() >= bytesToRead) {
        QByteArray frameData;
        for (int i = 0; i < bytesToRead; i++)
            frameData.append(buffer->dequeue());

        const QList<qreal> &values = dsp::Utility::bytesToValues(frameData, audioFormat);

        model::Frame frame = prepareFrame(values);

        QMetaObject::invokeMethod(
                this,
                "newFrame",
                Qt::QueuedConnection,
                Q_ARG(const audioreceiver::model::Frame, frame)
        );
    }

    emitBufferSize();
}

void Source::emitBufferSize() {
    QMetaObject::invokeMethod(this, "bufferSize", Qt::QueuedConnection, Q_ARG(const int, buffer->size()));
}

model::Frame Source::prepareFrame(const QList<qreal> &values) {
    frameNumber++;
    return model::Frame(frameNumber, values);
}
