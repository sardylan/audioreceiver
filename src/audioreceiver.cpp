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

#include <csignal>

#include <QtCore/QtGlobal>
#include <QtCore/QDebug>
#include <QtCore/QCoreApplication>
#include <QtCore/QList>
#include <QtCore/QFuture>

#include <QtConcurrent/QtConcurrentRun>

#include <QtWidgets/QApplication>
#include <QtWidgets/QMessageBox>

#include "audioreceiver.hpp"
#include "version.hpp"

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
    qRegisterMetaType<audioreceiver::model::Frame>("audioreceiver::model::Frame");

    QCoreApplication::setApplicationName(APPLICATION_NAME);
    QCoreApplication::setApplicationVersion(APPLICATION_VERSION);
    QCoreApplication::setOrganizationName(ORGANIZATION_NAME);
    QCoreApplication::setOrganizationDomain(ORGANIZATION_DOMAIN);

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

    Q_INIT_RESOURCE(windows);

    QApplication application(argc, argv);

    audioReceiver = new AudioReceiver();
    QApplication::connect(audioReceiver, &AudioReceiver::finished, []() { QCoreApplication::exit(); });
    audioReceiver->entryPoint();

#ifdef Q_OS_LINUX
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);
#endif

#ifdef Q_OS_WINDOWS
    if (SetConsoleCtrlHandler(ctrlHandler, TRUE) != TRUE)
        qWarning() << "Unable to set Control Handler";
#endif

    return QApplication::exec();
}

#define AUDIORECEIVER_SHM_KEY_SINGLE_INSTANCE "audioReceiver-singleInstance"

AudioReceiver::AudioReceiver(QObject *parent) : QObject(parent) {
    qSharedMemory = new QSharedMemory(AUDIORECEIVER_SHM_KEY_SINGLE_INSTANCE, this);

    config = new Config();

    worker = new Worker();

    mainWindow = new windows::Main(config);
    configWindow = new windows::Config(config);

    signalConnect();
}

AudioReceiver::~AudioReceiver() {
    delete mainWindow;
    delete configWindow;

    delete worker;

    delete qSharedMemory;
}

void AudioReceiver::entryPoint() {
//    if (!qSharedMemory->create(1, QSharedMemory::ReadWrite)) {
//        QMessageBox messageBox;
//        messageBox.setModal(true);
//        messageBox.setWindowTitle("Audio Receiver already running");
//        messageBox.setText("There is another instance of AudioReceiver running.");
//        messageBox.exec();
//
//        QMetaObject::invokeMethod(this, &AudioReceiver::finished, Qt::QueuedConnection);
//        return;
//    }

    start();
}

void AudioReceiver::start() {
    config->load();
    config->save();

    updateWorkerParams();
    handleNewWorkerStatus(false);

    mainWindow->show();
}

void AudioReceiver::stop() {
    QMetaObject::invokeMethod(this, &AudioReceiver::finished, Qt::QueuedConnection);
}

void AudioReceiver::signalConnect() {
    connect(mainWindow, &windows::Main::openConfigWindow, this, &AudioReceiver::openConfigWindow);
    connect(mainWindow, &windows::Main::audioWorkerToggle, [this](bool newStatus) {
        if (newStatus)
            QMetaObject::invokeMethod(worker, &Worker::start, Qt::QueuedConnection);
        else
            QMetaObject::invokeMethod(worker, &Worker::stop, Qt::QueuedConnection);
    });
    connect(mainWindow, &windows::Main::bfoToggle, worker, &Worker::setBFOStatus, Qt::QueuedConnection);
    connect(mainWindow, &windows::Main::newBFOFrequency, worker, &Worker::setBFOFrequency, Qt::QueuedConnection);
    connect(mainWindow, &windows::Main::newGainValue, worker, &Worker::setGain, Qt::QueuedConnection);

    connect(worker, &Worker::newStatus, this, &AudioReceiver::handleNewWorkerStatus);

    connect(worker, &Worker::bufferSize, mainWindow, &windows::Main::updateBufferSize, Qt::QueuedConnection);
    connect(worker, &Worker::newRMS, mainWindow, &windows::Main::updateVuMeter, Qt::QueuedConnection);
    connect(worker, &Worker::newFFT, mainWindow, &windows::Main::updateWaterfall, Qt::QueuedConnection);
}

void AudioReceiver::handleNewWorkerStatus(bool newStatus) {
    QMetaObject::invokeMethod(mainWindow, "updateWorkerStatus", Qt::QueuedConnection, Q_ARG(bool, newStatus));
    QMetaObject::invokeMethod(configWindow, "setLocked", Qt::QueuedConnection, Q_ARG(bool, newStatus));
}

void AudioReceiver::openConfigWindow() {
    configWindow->exec();
    QMetaObject::invokeMethod(this, &AudioReceiver::updateWorkerParams, Qt::QueuedConnection);
}

void AudioReceiver::updateWorkerParams() {
    QAudioDeviceInfo inputAudioDeviceInfo = QAudioDeviceInfo::defaultInputDevice();
    for (const QAudioDeviceInfo &audioDeviceInfo: QAudioDeviceInfo::availableDevices(QAudio::AudioInput))
        if (audioDeviceInfo.deviceName() == config->getAudioInputDevice())
            inputAudioDeviceInfo = audioDeviceInfo;

    worker->setInputAudioDeviceInfo(inputAudioDeviceInfo);

    QAudioFormat inputAudioFormat;
    inputAudioFormat.setChannelCount(config->getAudioInputChannels());
    inputAudioFormat.setSampleRate(config->getAudioInputSampleRate());
    inputAudioFormat.setSampleSize(config->getAudioInputSampleSize());
    inputAudioFormat.setSampleType(config->getAudioInputSampleType());
    inputAudioFormat.setByteOrder(config->getAudioInputEndian());
    inputAudioFormat.setCodec(config->getAudioInputCodec());

    inputAudioFormat = inputAudioDeviceInfo.nearestFormat(inputAudioFormat);
    worker->setInputAudioFormat(inputAudioFormat);

    QAudioDeviceInfo outputAudioDeviceInfo = QAudioDeviceInfo::defaultOutputDevice();
    for (const QAudioDeviceInfo &audioDeviceInfo: QAudioDeviceInfo::availableDevices(QAudio::AudioOutput))
        if (audioDeviceInfo.deviceName() == config->getAudioOutputDevice())
            outputAudioDeviceInfo = audioDeviceInfo;

    worker->setOutputAudioDeviceInfo(outputAudioDeviceInfo);

    QAudioFormat outputAudioFormat;
    outputAudioFormat.setChannelCount(config->getAudioOutputChannels());
    outputAudioFormat.setSampleRate(config->getAudioOutputSampleRate());
    outputAudioFormat.setSampleSize(config->getAudioOutputSampleSize());
    outputAudioFormat.setSampleType(config->getAudioOutputSampleType());
    outputAudioFormat.setByteOrder(config->getAudioOutputEndian());
    outputAudioFormat.setCodec(config->getAudioOutputCodec());

    outputAudioFormat = outputAudioDeviceInfo.nearestFormat(outputAudioFormat);
    worker->setOutputAudioFormat(outputAudioFormat);

    worker->setAudioChunkSize(config->getDSPSettingsAudioChunkSize());
    worker->setFftSampleSize(config->getDSPSettingsFFTSampleSize());

    QMetaObject::invokeMethod(mainWindow, "updateUiFromConfig", Qt::QueuedConnection,
                              Q_ARG(QAudioDeviceInfo, inputAudioDeviceInfo),
                              Q_ARG(QAudioFormat, inputAudioFormat),
                              Q_ARG(QAudioDeviceInfo, outputAudioDeviceInfo),
                              Q_ARG(QAudioFormat, outputAudioFormat)
    );
}
