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

#include "audioreceiver.hpp"
#include "dsp/utility.hpp"

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
    audioSource = new audio::Source();
    audioDestination = new audio::Destination();

    bfo = nullptr;

    connect(audioSource, &audio::Source::newFrame, this, &AudioReceiver::newFrame);
}

AudioReceiver::~AudioReceiver() {
    delete audioSource;
    delete audioDestination;

    delete bfo;
};

void AudioReceiver::start() {
    qInfo() << "Starting Audio Receiver";

    QAudioFormat requestedAudioFormat;
    requestedAudioFormat.setChannelCount(1);
    requestedAudioFormat.setSampleRate(48000);
    requestedAudioFormat.setSampleSize(16);
    requestedAudioFormat.setSampleType(QAudioFormat::SignedInt);
    requestedAudioFormat.setByteOrder(QAudioFormat::BigEndian);
    requestedAudioFormat.setCodec("audio/pcm");

    audioSource->setDeviceInfo(QAudioDeviceInfo::defaultInputDevice());
    audioSource->setAudioFormat(requestedAudioFormat);

    QAudioFormat audioFormat = audioSource->getAudioFormat();

    audioDestination->setDeviceInfo(QAudioDeviceInfo::defaultOutputDevice());
    audioDestination->setAudioFormat(audioFormat);

    bfo = new dsp::BFO(audioFormat.sampleRate(), this);
    bfo->setEnabled(true);
    bfo->setFrequency(16500);

    connect(bfo, &dsp::BFO::newComputedValues, this, &AudioReceiver::newMixedValues);

    QMetaObject::invokeMethod(audioDestination, &audio::Destination::start, Qt::QueuedConnection);
    QMetaObject::invokeMethod(audioSource, &audio::Source::start, Qt::QueuedConnection);

    QMetaObject::invokeMethod(this, &AudioReceiver::started, Qt::QueuedConnection);
}

void AudioReceiver::stop() {
    qInfo() << "Audio Receiver Stop";

    QMetaObject::invokeMethod(audioDestination, &audio::Destination::stop, Qt::QueuedConnection);
    QMetaObject::invokeMethod(audioSource, &audio::Source::stop, Qt::QueuedConnection);

    bfo->deleteLater();

    QMetaObject::invokeMethod(this, &AudioReceiver::finished, Qt::QueuedConnection);
}

void AudioReceiver::newFrame(const QByteArray &data) {
    QList<qreal> values = dsp::Utility::bytesToValues(data, audioSource->getAudioFormat());
    qreal inputRms = dsp::Utility::rms(values);

    QMetaObject::invokeMethod(bfo, "mix", Qt::QueuedConnection, Q_ARG(QList<qreal>, values));

    qDebug()
            << "Input size:" << data.length()
            << "-"
            << "Input RMS:" << inputRms;
}

void AudioReceiver::newMixedValues(const QList<qreal> &newValues) {
    QByteArray outputData = dsp::Utility::valuesToBytes(newValues, audioDestination->getAudioFormat());

    QMetaObject::invokeMethod(audioDestination, "newFrame", Qt::QueuedConnection, Q_ARG(const QByteArray, outputData));

    qDebug()
            << "New values:" << newValues.length()
            << "-"
            << "Output size:" << outputData.length();
}
