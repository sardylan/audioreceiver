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

#ifndef __AUDIORECEIVER__AUDIORECEIVER_H
#define __AUDIORECEIVER__AUDIORECEIVER_H

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
#include "status.hpp"
#include "worker.hpp"

#include "audio/source.hpp"
#include "audio/destination.hpp"

#include "dsp/bfo.hpp"
#include "dsp/fft.hpp"
#include "dsp/fir.hpp"

#include "windows/main.hpp"
#include "windows/config.hpp"

#include "model/frame.hpp"

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
        Status *status;

        Worker *worker;

        windows::Main *mainWindow;
        windows::Config *configWindow;

        void signalConnect();

    private slots:

        void updateWorkerParams();

        void openConfigWindow();


    signals:

        void started();

        void finished();

    };

}

Q_DECLARE_METATYPE(QList<qreal>);

#endif
