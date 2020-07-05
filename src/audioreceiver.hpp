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

#include <QtCore/QObject>

#include "audio_source.hpp"
#include "audio_destination.hpp"

#ifdef Q_OS_LINUX
#include <csignal>
void signalHandler(int signal);
#endif

#ifdef Q_OS_WINDOWS

#include <windows.h>

BOOL WINAPI ctrlHandler(DWORD ctrlHandler);

#endif

namespace audioreceiver {

    class AudioReceiver : public QObject {
    Q_OBJECT

    public:

        explicit AudioReceiver(QObject *parent = nullptr);

        ~AudioReceiver() override;

        void start();

        void stop();

    private:

        AudioSource *audioSource;
        AudioDestination *audioDestination;

    private slots:

        void newFrame(const QByteArray &data);

    signals:

        void started();

        void finished();

    };

}

#endif
