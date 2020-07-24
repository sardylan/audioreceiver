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

#define CONFIG_NAME_AUDIO_INPUT "audioInput"
#define CONFIG_NAME_AUDIO_OUTPUT "audioOutput"
#define CONFIG_NAME_DSP_SETTINGS "dspSettings"

#define CONFIG_NAME_AUDIO_DEVICE "device"
#define CONFIG_NAME_AUDIO_CHANNELS "channels"
#define CONFIG_NAME_AUDIO_SAMPLE_RATE "sampleRate"
#define CONFIG_NAME_AUDIO_SAMPLE_SIZE "sampleSize"
#define CONFIG_NAME_AUDIO_SAMPLE_TYPE "sampleType"
#define CONFIG_NAME_AUDIO_ENDIAN "endian"
#define CONFIG_NAME_AUDIO_CODEC "codec"

#define CONFIG_NAME_AUDIO_CHUNK_SIZE "audioChunkSize"
#define CONFIG_NAME_FFT_SAMPLE_SIZE "fftSampleSize"

namespace audioreceiver {

    class Config : public QObject {
    Q_OBJECT

    public:

        explicit Config(QObject *parent = nullptr);

        ~Config() override;

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

        [[nodiscard]] const QString &getAudioInputCodec() const;

        void setAudioInputCodec(const QString &newValue);

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

        [[nodiscard]] const QString &getAudioOutputCodec() const;

        void setAudioOutputCodec(const QString &newValue);

        [[nodiscard]] int getDSPSettingsAudioChunkSize() const;

        void setDSPSettingsAudioChunkSize(int newValue);

        [[nodiscard]] int getDSPSettingsFFTSampleSize() const;

        void setDSPSettingsFFTSampleSize(int newValue);


    public slots:

        void load();

        void save() const;

    private:

        QString audioInputDevice;
        int audioInputChannels;
        int audioInputSampleRate;
        int audioInputSampleSize;
        QAudioFormat::SampleType audioInputSampleType;
        QAudioFormat::Endian audioInputEndian;
        QString audioInputCodec;

        QString audioOutputDevice;
        int audioOutputChannels;
        int audioOutputSampleRate;
        int audioOutputSampleSize;
        QAudioFormat::SampleType audioOutputSampleType;
        QAudioFormat::Endian audioOutputEndian;
        QString audioOutputCodec;

        int dspSettingsAudioChunkSize;
        int dspSettingsFFTSampleSize;

    };

}

#endif
