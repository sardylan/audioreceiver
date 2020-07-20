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

#ifndef __AUDIORECEIVER__WORKER_H
#define __AUDIORECEIVER__WORKER_H

#include <QtCore/QObject>
#include <QtMultimedia/QAudioDeviceInfo>
#include <QtMultimedia/QAudioFormat>

#include "utilities/service.hpp"

#include "audio/source.hpp"
#include "audio/destination.hpp"

#include "dsp/bfo.hpp"
#include "dsp/fft.hpp"
#include "dsp/fir.hpp"

#include "model/frame.hpp"

namespace audioreceiver {

    class Worker : public utilities::Service {
    Q_OBJECT

    public:

        explicit Worker(QObject *parent = nullptr);

        ~Worker() override;

        [[nodiscard]] const QAudioDeviceInfo &getInputAudioDeviceInfo() const;

        void setInputAudioDeviceInfo(const QAudioDeviceInfo &inputAudioDeviceInfo);

        [[nodiscard]] const QAudioFormat &getInputAudioFormat() const;

        void setInputAudioFormat(const QAudioFormat &inputAudioFormat);

        [[nodiscard]] const QAudioDeviceInfo &getOutputAudioDeviceInfo() const;

        void setOutputAudioDeviceInfo(const QAudioDeviceInfo &outputAudioDeviceInfo);

        [[nodiscard]] const QAudioFormat &getOutputAudioFormat() const;

        void setOutputAudioFormat(const QAudioFormat &outputAudioFormat);

    public slots:

        void start() override;

        void stop() override;

    private:

        audio::Source *audioSource;
        audio::Destination *audioDestination;

        dsp::BFO *bfo;
        dsp::FFT *fft;
        dsp::FIR *fir;

        QAudioDeviceInfo inputAudioDeviceInfo;
        QAudioFormat inputAudioFormat;

        QAudioDeviceInfo outputAudioDeviceInfo;
        QAudioFormat outputAudioFormat;

    private slots:

        void newFrame(const model::Frame &frame);

    signals:

        void started();

        void finished();

        void newRMS(const qreal &rms);

        void newFFT(const QList<qreal> &rms);

        void newValues(const QList<qreal> &rms);

    };

}

#endif
