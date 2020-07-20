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

#ifndef __AUDIORECEIVER__CONFIG_H
#define __AUDIORECEIVER__CONFIG_H

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtMultimedia/QAudioFormat>

namespace audioreceiver {

    class Config : public QObject {
    Q_OBJECT

    public:

        static Config* getInstance();

        [[nodiscard]] const QString &getAudioInputDevice() const;

        void setAudioInputDevice(const QString &newValue);

        [[nodiscard]] int getAudioInputChannels() const;

        void setAudioInputChannels(int newValue);

        [[nodiscard]] int getAudioInputSampleRate() const;

        void setAudioInputSampleRate(int newValue);

        [[nodiscard]] int getAudioInputSampleSize() const;

        void setAudioInputSampleSize(int newValue);

        [[nodiscard]] QAudioFormat::SampleType getAudioInputSampleType() const;

        void setAudioInputSampleType(QAudioFormat::SampleType newValue);

        [[nodiscard]] QAudioFormat::Endian getAudioInputEndian() const;

        void setAudioInputEndian(QAudioFormat::Endian audioInputEndian);

        [[nodiscard]] const QString &getAudioOutputDevice() const;

        void setAudioOutputDevice(const QString &newValue);

        [[nodiscard]] int getAudioOutputChannels() const;

        void setAudioOutputChannels(int newValue);

        [[nodiscard]] int getAudioOutputSampleRate() const;

        void setAudioOutputSampleRate(int newValue);

        [[nodiscard]] int getAudioOutputSampleSize() const;

        void setAudioOutputSampleSize(int newValue);

        [[nodiscard]] QAudioFormat::SampleType getAudioOutputSampleType() const;

        void setAudioOutputSampleType(QAudioFormat::SampleType newValue);

        [[nodiscard]] QAudioFormat::Endian getAudioOutputEndian() const;

        void setAudioOutputEndian(QAudioFormat::Endian newValue);

    public slots:

        void load();

        void save() const;

    private:

        explicit Config(QObject *parent = nullptr);

        ~Config() override;

        static Config *instance;

        QString audioInputDevice;
        int audioInputChannels;
        int audioInputSampleRate;
        int audioInputSampleSize;
        QAudioFormat::SampleType audioInputSampleType;
        QAudioFormat::Endian audioInputEndian;

        QString audioOutputDevice;
        int audioOutputChannels;
        int audioOutputSampleRate;
        int audioOutputSampleSize;
        QAudioFormat::SampleType audioOutputSampleType;
        QAudioFormat::Endian audioOutputEndian;

    };

}

#endif
