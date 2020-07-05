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
#include "dsp.hpp"

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
    QCoreApplication coreApplication(argc, argv);

    audioReceiver = new AudioReceiver();
    QCoreApplication::connect(audioReceiver, &AudioReceiver::finished, []() { QCoreApplication::exit(); });

    audioReceiver->start();

#ifdef Q_OS_LINUX
    signal(SIGINT, signalHandler);
#endif

#ifdef Q_OS_WINDOWS
    if (SetConsoleCtrlHandler(ctrlHandler, TRUE) != TRUE)
        qWarning() << "Unable to set Control Handler";
#endif

    return QCoreApplication::exec();
}

AudioReceiver::AudioReceiver(QObject *parent) : QObject(parent) {
    audioSource = new AudioSource();
    audioDestination = new AudioDestination();

    connect(audioSource, &AudioSource::newFrame, this, &AudioReceiver::newFrame);
//    connect(audioSource, &AudioSource::newFrame, audioDestination, &AudioDestination::newFrame);
}

AudioReceiver::~AudioReceiver() {
    delete audioSource;
    delete audioDestination;
};

void AudioReceiver::start() {
    qInfo() << "Starting Audio Receiver";

    audioDestination->setDeviceInfo(QAudioDeviceInfo::defaultOutputDevice());
    audioDestination->start();

    audioSource->setDeviceInfo(QAudioDeviceInfo::defaultInputDevice());
    audioSource->start();

    QMetaObject::invokeMethod(this, &AudioReceiver::started, Qt::QueuedConnection);
}

void AudioReceiver::stop() {
    qInfo() << "Audio Receiver Stop";
    audioSource->stop();

    QMetaObject::invokeMethod(this, &AudioReceiver::finished, Qt::QueuedConnection);
}

void AudioReceiver::newFrame(const QByteArray &data) {
    QList<qreal> values = DSP::bytesToValues(data, audioSource->getAudioFormat());

    qreal rms = DSP::rms(values);
    qDebug() << "RMS" << rms;

    QByteArray outputData = DSP::valuesToBytes(values, audioDestination->getAudioFormat());

    QMetaObject::invokeMethod(audioDestination, "newFrame", Qt::QueuedConnection, Q_ARG(const QByteArray, outputData));
}
