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

#ifndef __AUDIORECEVIER__AUDIO__SOURCE_H
#define __AUDIORECEVIER__AUDIO__SOURCE_H

#include <QtCore/QObject>
#include <QtCore/QIODevice>
#include <QtCore/QQueue>

#include <QtMultimedia/QAudioDeviceInfo>
#include <QtMultimedia/QAudioFormat>
#include <QtMultimedia/QAudioInput>

#include "../utilities/service.hpp"

#include "../model/frame.hpp"

#define AUDIO_BUFFER_INPUT 65536

using namespace audioreceiver;

namespace audioreceiver::audio {

    class Source : public utilities::Service {
    Q_OBJECT

    public:

        explicit Source(const int &frameSize, QObject *parent = nullptr);

        ~Source() override;

        [[nodiscard]] int getFrameSize() const;

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

        const int frameSize;

        QAudioDeviceInfo deviceInfo;
        QAudioFormat audioFormat;

        QAudioInput *audioInput;
        QIODevice *audioIODevice;

        quint64 frames;
        quint64 bytes;

        QQueue<char> *buffer;

    private slots:

        void readAudioBytes();

    signals:

        void newFrame(const audioreceiver::model::Frame &frame);

    };

}

#endif
