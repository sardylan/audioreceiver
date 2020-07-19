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

#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>
#include <QtCore/QList>
#include <QtCore/QFuture>
#include <QtConcurrent/QtConcurrentRun>

#include "audioreceiver.hpp"

#include "dsp/utility.hpp"

#include "model/frame.hpp"

using namespace audioreceiver;

AudioReceiver *audioReceiver;

#ifdef Q_OS_LINUX

void signalHandler(int signal) {
    switch (signal) {
        case SIGINT:
        case SIGTERM:
            audioReceiver->stop();
            break;

        default:
            return;
    }
}

#endif

#ifdef Q_OS_WINDOWS

BOOL WINAPI ctrlHandler(DWORD fdwCtrlType) {
    switch (fdwCtrlType) {
        case CTRL_C_EVENT:
        case CTRL_CLOSE_EVENT:
            audioReceiver->stop();
            return TRUE;

        default:
            return FALSE;
    }
}

#endif

int main(int argc, char **argv) {
    qSetMessagePattern("\x1b[94;1m[\x1b[96;1m%{time yyyy-MM-dd hh:mm:ss.zzz}\x1b[94;1m]\x1b[39;0m "
                       "PID:\x1b[31m%{pid}\x1b[39m "
                       "TID:\x1b[91m%{threadid}\x1b[39m "
                       "["
                       "%{if-debug}\x1b[37m DEBUG  \x1b[39m%{endif}"
                       "%{if-info}\x1b[92m INFO   \x1b[39m%{endif}"
                       "%{if-warning}\x1b[93mWARNING \x1b[39m%{endif}"
                       "%{if-critical}\x1b[91mCRITICAL\x1b[39m%{endif}"
                       "%{if-fatal}\x1b[91;5m FATAL  \x1b[39;25m%{endif}"
                       "]: "
                       "%{file}:%{line} "
                       "[\x1b[97m%{function}()\x1b[39m] "
                       "%{message}");

    qRegisterMetaType<audioreceiver::model::Frame>("audioreceiver::model::Frame");

    QCoreApplication coreApplication(argc, argv);

    audioReceiver = new AudioReceiver();
    QCoreApplication::connect(audioReceiver, &AudioReceiver::finished, []() { QCoreApplication::exit(); });

    audioReceiver->start();

#ifdef Q_OS_LINUX
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);
#endif

#ifdef Q_OS_WINDOWS
    if (SetConsoleCtrlHandler(ctrlHandler, TRUE) != TRUE)
        qWarning() << "Unable to set Control Handler";
#endif

    return QCoreApplication::exec();
}

AudioReceiver::AudioReceiver(QObject *parent) : QObject(parent) {
    config = new Config(this);

    audioSource = new audio::Source(AUDIORECEIVER_FRAME_SIZE);
    audioDestination = new audio::Destination();

    bfo = nullptr;
    fft = nullptr;
    fir = nullptr;

    connect(audioSource, &audio::Source::newFrame, this, &AudioReceiver::newFrame);
}

AudioReceiver::~AudioReceiver() {
    delete config;

    delete audioSource;
    delete audioDestination;

    delete bfo;
    delete fft;
};

void AudioReceiver::start() {
    qInfo() << "Starting Audio Receiver";

    config->load();
    config->save();

    QAudioFormat inputAudioFormat = prepareInputAudio();
    QAudioFormat outputAudioFormat = prepareOutputAudio();

    bfo = new dsp::BFO(inputAudioFormat.sampleRate(), this);
    bfo->setEnabled(true);
    bfo->setFrequency(1);

    fft = new dsp::FFT(AUDIORECEIVER_FRAME_SIZE);

    QList<qreal> kernel;
    fir = new dsp::FIR(kernel);
    fir->setEnabled(true);

    QMetaObject::invokeMethod(audioDestination, &audio::Destination::start, Qt::QueuedConnection);
    QMetaObject::invokeMethod(audioSource, &audio::Source::start, Qt::QueuedConnection);

    QMetaObject::invokeMethod(this, &AudioReceiver::started, Qt::QueuedConnection);
}

QAudioFormat AudioReceiver::prepareInputAudio() const {
    QAudioDeviceInfo inputAudioDeviceInfo = QAudioDeviceInfo::defaultInputDevice();

    for (const QAudioDeviceInfo &audioDeviceInfo: QAudioDeviceInfo::availableDevices(QAudio::AudioInput))
        if (audioDeviceInfo.deviceName() == config->getAudioInputDevice())
            inputAudioDeviceInfo = audioDeviceInfo;

    QAudioFormat requestedAudioFormat;
    requestedAudioFormat.setChannelCount(config->getAudioInputChannels());
    requestedAudioFormat.setSampleRate(config->getAudioInputSampleRate());
    requestedAudioFormat.setSampleSize(config->getAudioInputSampleSize());
    requestedAudioFormat.setSampleType(config->getAudioInputSampleType());
    requestedAudioFormat.setByteOrder(config->getAudioInputEndian());
    requestedAudioFormat.setCodec("audio/pcm");

    audioSource->setDeviceInfo(inputAudioDeviceInfo);
    audioSource->setAudioFormat(requestedAudioFormat);

    return audioSource->getAudioFormat();
}

QAudioFormat AudioReceiver::prepareOutputAudio() const {
    QAudioDeviceInfo outputAudioDeviceInfo = QAudioDeviceInfo::defaultOutputDevice();

    for (const QAudioDeviceInfo &audioDeviceInfo: QAudioDeviceInfo::availableDevices(QAudio::AudioOutput))
        if (audioDeviceInfo.deviceName() == config->getAudioOutputDevice())
            outputAudioDeviceInfo = audioDeviceInfo;

    QAudioFormat requestedAudioFormat;
    requestedAudioFormat.setChannelCount(config->getAudioOutputChannels());
    requestedAudioFormat.setSampleRate(config->getAudioOutputSampleRate());
    requestedAudioFormat.setSampleSize(config->getAudioOutputSampleSize());
    requestedAudioFormat.setSampleType(config->getAudioOutputSampleType());
    requestedAudioFormat.setByteOrder(config->getAudioOutputEndian());
    requestedAudioFormat.setCodec("audio/pcm");

    audioDestination->setDeviceInfo(outputAudioDeviceInfo);
    audioDestination->setAudioFormat(requestedAudioFormat);

    return audioDestination->getAudioFormat();
}

void AudioReceiver::stop() {
    qInfo() << "Audio Receiver Stop";

    QMetaObject::invokeMethod(audioDestination, &audio::Destination::stop, Qt::QueuedConnection);
    QMetaObject::invokeMethod(audioSource, &audio::Source::stop, Qt::QueuedConnection);

    bfo->deleteLater();
    fft->deleteLater();
    fir->deleteLater();

    QMetaObject::invokeMethod(this, &AudioReceiver::finished, Qt::QueuedConnection);
}

void AudioReceiver::newFrame(const model::Frame &frame) {
    QFuture<QList<qreal>> fftFuture = QtConcurrent::run(fft, &dsp::FFT::compute, frame.getValues());
    QFuture<QList<qreal>> bfoFuture = QtConcurrent::run(bfo, &dsp::BFO::compute, frame.getValues());
    QFuture<qreal> rmsFuture = QtConcurrent::run(dsp::Utility::rms, frame.getValues());

    QList<qreal> ifValues = bfoFuture.result();
    QFuture<QList<qreal>> firFuture = QtConcurrent::run(fir, &dsp::FIR::compute, ifValues);

    qreal rms = rmsFuture.result();
    QList<qreal> newValues = firFuture.result();
    QList<qreal> fftValues = fftFuture.result();

    QMetaObject::invokeMethod(
            audioDestination,
            "newValues",
            Qt::QueuedConnection,
            Q_ARG(const QList<qreal>, newValues)
    );

    qDebug()
            << "Frame:" << frame
            << "-"
            << "RMS:" << rms;
}
