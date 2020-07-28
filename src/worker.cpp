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

#include <QtCore/QFuture>

#include <QtConcurrent/QtConcurrent>

#include "worker.hpp"

#include "dsp/utility.hpp"

using namespace audioreceiver;

Worker::Worker(QObject *parent) : Service(parent) {
    audioSource = nullptr;
    audioDestination = nullptr;

    bfo = nullptr;
    fft = nullptr;
    fir = nullptr;

    audioChunkSize = 1024;
    fftSampleSize = 1024;

    gain = 1;
    bfoStatus = false;
    bfoFrequency = 0;

    fftBuffer.clear();
}

Worker::~Worker() {
    delete audioSource;
    delete audioDestination;

    delete bfo;
    delete fft;
    delete fir;
}

const QAudioDeviceInfo &Worker::getInputAudioDeviceInfo() const {
    return inputAudioDeviceInfo;
}

void Worker::setInputAudioDeviceInfo(const QAudioDeviceInfo &newValue) {
    Worker::inputAudioDeviceInfo = newValue;
}

const QAudioFormat &Worker::getInputAudioFormat() const {
    return inputAudioFormat;
}

void Worker::setInputAudioFormat(const QAudioFormat &newValue) {
    Worker::inputAudioFormat = newValue;
}

const QAudioDeviceInfo &Worker::getOutputAudioDeviceInfo() const {
    return outputAudioDeviceInfo;
}

void Worker::setOutputAudioDeviceInfo(const QAudioDeviceInfo &newValue) {
    Worker::outputAudioDeviceInfo = newValue;
}

const QAudioFormat &Worker::getOutputAudioFormat() const {
    return outputAudioFormat;
}

void Worker::setOutputAudioFormat(const QAudioFormat &newValue) {
    Worker::outputAudioFormat = newValue;
}

int Worker::getAudioChunkSize() const {
    return audioChunkSize;
}

void Worker::setAudioChunkSize(int newValue) {
    Worker::audioChunkSize = newValue;
}

int Worker::getFftSampleSize() const {
    return fftSampleSize;
}

void Worker::setFftSampleSize(int newValue) {
    Worker::fftSampleSize = newValue;
}


qreal Worker::getGain() const {
    return gain;
}

bool Worker::isBfoStatus() const {
    return bfoStatus;
}

unsigned int Worker::getBfoFrequency() const {
    return bfoFrequency;
}

void Worker::start() {
    qInfo() << "Starting Audio Worker";

    audioSource = new audio::Source(audioChunkSize);
    audioSource->setDeviceInfo(inputAudioDeviceInfo);
    audioSource->setAudioFormat(inputAudioFormat);

    connect(audioSource, &audio::Source::newFrame, this, &Worker::newFrame);
    connect(audioSource, &audio::Source::bufferSize, this, &Worker::bufferSize, Qt::QueuedConnection);

    audioDestination = new audio::Destination();
    audioDestination->setDeviceInfo(outputAudioDeviceInfo);
    audioDestination->setAudioFormat(outputAudioFormat);

    bfo = new dsp::BFO(inputAudioFormat.sampleRate());
    bfo->setEnabled(bfoStatus);
    bfo->setFrequency(bfoFrequency);

    fft = new dsp::FFT(fftSampleSize);

    QList<qreal> kernel;
    fir = new dsp::FIR(kernel);
    fir->setEnabled(true);

    QMetaObject::invokeMethod(audioDestination, &audio::Destination::start, Qt::QueuedConnection);
    QMetaObject::invokeMethod(audioSource, &audio::Source::start, Qt::QueuedConnection);

    QMetaObject::invokeMethod(this, "newStatus", Qt::QueuedConnection, Q_ARG(bool, true));
}

void Worker::stop() {
    qInfo() << "Audio Worker Stop";

    QMetaObject::invokeMethod(audioDestination, &audio::Destination::stop, Qt::QueuedConnection);
    QMetaObject::invokeMethod(audioSource, &audio::Source::stop, Qt::QueuedConnection);

    bfo->deleteLater();
    fft->deleteLater();
    fir->deleteLater();

    audioSource->deleteLater();
    audioDestination->deleteLater();

    QMetaObject::invokeMethod(this, "newStatus", Qt::QueuedConnection, Q_ARG(bool, false));
}

void Worker::newFrame(const model::Frame &frame) {
    QFuture<QList<qreal>> gainFuture = QtConcurrent::run(dsp::Utility::gain, frame.getValues(), gain);
    QList<qreal> values = gainFuture.result();

    fftBuffer.append(values);

    QList<QFuture<QList<qreal>>> fftFutureList;

    while (fftBuffer.size() >= fftSampleSize) {
        QList<qreal> fftSample;

        while (fftSample.size() < fftSampleSize && !fftBuffer.empty())
            fftSample.append(fftBuffer.dequeue());

        if (fftSample.size() == fftSampleSize) {
            QFuture<QList<qreal>> fftFuture = QtConcurrent::run(fft, &dsp::FFT::computeLog, fftSample);
            fftFutureList.append(fftFuture);
        }
    }

    QFuture<QList<qreal>> bfoFuture = QtConcurrent::run(bfo, &dsp::BFO::compute, values);
    QFuture<qreal> rmsFuture = QtConcurrent::run(dsp::Utility::rmsLog, values);

    QList<qreal> ifValues = bfoFuture.result();
    QFuture<QList<qreal>> firFuture = QtConcurrent::run(fir, &dsp::FIR::compute, ifValues);

    qreal rms = rmsFuture.result();
    QList<qreal> newValues = firFuture.result();

    for (const QFuture<QList<qreal>> &fftFuture: fftFutureList) {
        QList<qreal> fftValues = fftFuture.result();
        QMetaObject::invokeMethod(this, "newFFT", Qt::QueuedConnection, Q_ARG(const QList<qreal>, fftValues));
    }

    QMetaObject::invokeMethod(this, "newRMS", Qt::QueuedConnection, Q_ARG(const qreal, rms));
    QMetaObject::invokeMethod(this, "newValues", Qt::QueuedConnection, Q_ARG(const QList<qreal>, newValues));

    QMetaObject::invokeMethod(
            audioDestination,
            "newValues",
            Qt::QueuedConnection,
            Q_ARG(const QList<qreal>, newValues)
    );
}

void Worker::setGain(qreal newValue) {
    Worker::gain = newValue;
}

void Worker::setBFOStatus(bool newValue) {
    Worker::bfoStatus = newValue;
    QMetaObject::invokeMethod(bfo, "setEnabled", Qt::QueuedConnection, Q_ARG(bool, Worker::bfoStatus));
}

void Worker::setBFOFrequency(unsigned int newValue) {
    Worker::bfoFrequency = newValue;
    QMetaObject::invokeMethod(bfo, "setFrequency", Qt::QueuedConnection, Q_ARG(unsigned int, Worker::bfoFrequency));
}
