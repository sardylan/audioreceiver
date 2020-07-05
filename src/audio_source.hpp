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

#ifndef __AUDIORECEVIER__AUDIO_SOURCE_H
#define __AUDIORECEVIER__AUDIO_SOURCE_H

#include <QtCore/QObject>
#include <QtCore/QIODevice>

#include <QtMultimedia/QAudioDeviceInfo>
#include <QtMultimedia/QAudioFormat>
#include <QtMultimedia/QAudioInput>

#include "service.hpp"

namespace audioreceiver {

    class AudioSource : public Service {
    Q_OBJECT

    public:

        explicit AudioSource(QObject *parent = nullptr);

        ~AudioSource() override;

        [[nodiscard]] const QAudioDeviceInfo &getDeviceInfo() const;

        void setDeviceInfo(const QAudioDeviceInfo &value);

        [[nodiscard]] const QAudioFormat &getAudioFormat() const;

        void setAudioFormat(const QAudioFormat &value);

        [[nodiscard]] quint64 getFrames() const;

        [[nodiscard]] quint64 getBytes() const;

    public slots:

        void start() override;

        void stop() override;

    private:

        QAudioDeviceInfo deviceInfo;
        QAudioFormat audioFormat;

        QAudioInput *audioInput;
        QIODevice *audioIODevice;

        quint64 frames;
        quint64 bytes;

    private slots:

        void readAudioBytes();

    signals:

        void newFrame(const QByteArray &data);

    };

}

#endif
