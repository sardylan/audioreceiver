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

#include <dsp/utility.hpp>

#include "worker.hpp"

using namespace audioreceiver;

#define AUDIORECEIVER_FRAME_SIZE 1024

Worker::Worker(QObject *parent) : Service(parent) {
    audioSource = new audio::Source(AUDIORECEIVER_FRAME_SIZE);
    audioDestination = new audio::Destination();

    bfo = nullptr;
    fft = nullptr;
    fir = nullptr;

    connect(audioSource, &audio::Source::newFrame, this, &Worker::newFrame);
}

Worker::~Worker() {
    delete audioSource;
    delete audioDestination;

    delete bfo;
    delete fft;
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

void Worker::start() {
    qInfo() << "Starting Audio Worker";

    audioSource->setDeviceInfo(inputAudioDeviceInfo);
    audioSource->setAudioFormat(inputAudioFormat);

    audioDestination->setDeviceInfo(outputAudioDeviceInfo);
    audioDestination->setAudioFormat(outputAudioFormat);

    bfo = new dsp::BFO(inputAudioFormat.sampleRate(), this);
    bfo->setEnabled(true);
    bfo->setFrequency(1);

    fft = new dsp::FFT(AUDIORECEIVER_FRAME_SIZE);

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

    QMetaObject::invokeMethod(this, "newStatus", Qt::QueuedConnection, Q_ARG(bool, false));
}

void Worker::newFrame(const model::Frame &frame) {
    QFuture<QList<qreal>> fftFuture = QtConcurrent::run(fft, &dsp::FFT::compute, frame.getValues());
    QFuture<QList<qreal>> bfoFuture = QtConcurrent::run(bfo, &dsp::BFO::compute, frame.getValues());
    QFuture<qreal> rmsFuture = QtConcurrent::run(dsp::Utility::rms, frame.getValues());

    QList<qreal> ifValues = bfoFuture.result();
    QFuture<QList<qreal>> firFuture = QtConcurrent::run(fir, &dsp::FIR::compute, ifValues);

    qreal rms = rmsFuture.result();
    QList<qreal> newValues = firFuture.result();
    QList<qreal> fftValues = fftFuture.result();

    QMetaObject::invokeMethod(this, "newRMS", Qt::QueuedConnection, Q_ARG(const qreal, rms));
    QMetaObject::invokeMethod(this, "newFFT", Qt::QueuedConnection, Q_ARG(const QList<qreal>, fftValues));
    QMetaObject::invokeMethod(this, "newValues", Qt::QueuedConnection, Q_ARG(const QList<qreal>, newValues));

    QMetaObject::invokeMethod(
            audioDestination,
            "newValues",
            Qt::QueuedConnection,
            Q_ARG(const QList<qreal>, newValues)
    );

//    qDebug()
//            << "Frame:" << frame
//            << "-"
//            << "RMS:" << rms;
}

void Worker::setBFOStatus(bool newStatus) {
    QMetaObject::invokeMethod(bfo, "setEnabled", Qt::QueuedConnection, Q_ARG(bool, newStatus));
}

void Worker::setBFOFrequency(unsigned int frequency) {
    QMetaObject::invokeMethod(bfo, "setFrequency", Qt::QueuedConnection, Q_ARG(unsigned int, frequency));
}
