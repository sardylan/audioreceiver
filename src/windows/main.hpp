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

#ifndef __AUDIORECEIVER__WINDOWS__MAIN_H
#define __AUDIORECEIVER__WINDOWS__MAIN_H

#include <QtCore/QtGlobal>
#include <QtCore/QtDebug>
#include <QtCore/QList>
#include <QtCore/QDateTime>
#include <QtCore/QTimer>

#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>

#include <QtMultimedia/QAudioDeviceInfo>
#include <QtMultimedia/QAudioFormat>

#include "../config.hpp"

#include "../widgets/vumeter.hpp"
#include "../widgets/waterfall.hpp"

#define AUDIORECEIVER_AUDIO_BUFFER_SIZE_MAX 32768

#define AUDIORECEIVER_AUDIO_GAIN_MAX_DB 30
#define AUDIORECEIVER_AUDIO_GAIN_RESOLUTION 100

namespace Ui {
    class Main;
}

namespace audioreceiver::windows {

    class Main : public QMainWindow {
    Q_OBJECT

    public:

        explicit Main(audioreceiver::Config *config, QWidget *parent = nullptr);

        ~Main() override;

    public slots:

        void
        updateAudioDevicesParams(const QAudioDeviceInfo &inputAudioDeviceInfo, const QAudioFormat &inputAudioFormat,
                                 const QAudioDeviceInfo &outputAudioDeviceInfo, const QAudioFormat &outputAudioFormat);

        void updateWorkerStatus(bool value);

        void updateBufferSize(const int &size);

        void updateVuMeter(const qreal &value);

        void updateWaterfall(const QList<qreal> &values);

    private:

        Ui::Main *ui;

        audioreceiver::Config *config;

        QLabel *statusBarClockLabel;
        QLabel *statusBarVersionLabel;
        QLabel *statusBarAudioInputDevice;
        QLabel *statusBarAudioInputFormat;

        QTimer *clockTimer;

        widgets::VUMeter *vuMeter;
        widgets::Waterfall *waterfall;

        void initStatusBar();

        void signalConnect();

        void initUi();

        static QString prepareAudioFormatString(const QAudioFormat &inputAudioFormat);

    private slots:

        void updateClock();

        void updateGainValue();

        void updateBFOEnabled();

        void updateBFOValue(int value);

    signals:

        void openConfigWindow();

        void audioWorkerToggle(bool status);

        void bfoToggle(bool status);

        void newBFOFrequency(unsigned int value);

        void newGainValue(qreal gain);

    };

}

#endif
