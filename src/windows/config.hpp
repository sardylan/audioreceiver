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

#ifndef __AUDIORECEIVER__WINDOWS__CONFIG_H
#define __AUDIORECEIVER__WINDOWS__CONFIG_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QDialog>
#include <QtWidgets/QComboBox>

#include <QtMultimedia/QAudioFormat>
#include <QtMultimedia/QAudioDeviceInfo>

#include "../config.hpp"

namespace Ui {
    class Config;
}

namespace audioreceiver::windows {

    class Config : public QDialog {
    Q_OBJECT

    public:

        explicit Config(audioreceiver::Config *config, QWidget *parent = nullptr);

        ~Config() override;

        [[nodiscard]] bool isLocked() const;

    public slots:

        void setLocked(bool newValue);

    private:

        Ui::Config *ui;

        audioreceiver::Config *config;

        bool locked;

        void signalConnect();

        void initUi();

        void load();

        void save();

        void checkStatus();

        static void
        initDeviceComboBox(QComboBox *comboBox,
                           QAudio::Mode mode);

        static void
        updateDeviceComboBox(QComboBox *comboBox,
                             const QString& currentValue);

        static void
        updateChannelsComboBox(QComboBox *comboBox, const QAudioDeviceInfo &audioDeviceInfo,
                               int currentValue);

        static void
        updateSampleRateComboBox(QComboBox *comboBox, const QAudioDeviceInfo &audioDeviceInfo,
                                 int currentValue);

        static void
        updateSampleSizeComboBox(QComboBox *comboBox, const QAudioDeviceInfo &audioDeviceInfo,
                                 int currentValue);

        static void
        updateSampleTypeComboBox(QComboBox *comboBox, const QAudioDeviceInfo &audioDeviceInfo,
                                 QAudioFormat::SampleType currentValue);

        static void
        updateEndianComboBox(QComboBox *comboBox, const QAudioDeviceInfo &audioDeviceInfo,
                             QAudioFormat::Endian currentValue);

        static void
        updateCodecComboBox(QComboBox *comboBox, const QAudioDeviceInfo &audioDeviceInfo,
                            const QString &currentValue);

    private slots:

        void handleOK();

        void handleApply();

        void handleClose();

        void handleReset();

        void updateInputDeviceOptions();

        void updateOutputDeviceOptions();

    };

}

#endif
