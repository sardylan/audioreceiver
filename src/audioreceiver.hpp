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

#ifndef __AUDIORECEVIER__AUDIORECEIVER_H
#define __AUDIORECEVIER__AUDIORECEIVER_H

#ifdef Q_OS_LINUX

#include <csignal>

void signalHandler(int signal);

#endif

#ifdef Q_OS_WINDOWS

#include <windows.h>

BOOL WINAPI ctrlHandler(DWORD ctrlHandler);

#endif

#include <QtCore/QObject>

#include "config.hpp"

#include "audio/source.hpp"
#include "audio/destination.hpp"

#include "dsp/bfo.hpp"
#include "dsp/fft.hpp"
#include "dsp/fir.hpp"

#include "windows/main.hpp"

#include "model/frame.hpp"

#define AUDIORECEIVER_FRAME_SIZE 1024

namespace audioreceiver {

    class AudioReceiver : public QObject {
    Q_OBJECT

    public:

        explicit AudioReceiver(QObject *parent = nullptr);

        ~AudioReceiver() override;

        void start();

        void stop();

    private:

        Config *config;

        audio::Source *audioSource;
        audio::Destination *audioDestination;

        dsp::BFO *bfo;
        dsp::FFT *fft;
        dsp::FIR *fir;

        windows::Main *mainWindow;

        [[nodiscard]] QAudioFormat prepareInputAudio() const;

        [[nodiscard]] QAudioFormat prepareOutputAudio() const;

    private slots:

        void newFrame(const model::Frame &frame);

    signals:

        void started();

        void finished();

    };

}

Q_DECLARE_METATYPE(QList<qreal>);

#endif
