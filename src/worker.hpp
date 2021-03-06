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
#include <QtCore/QList>
#include <QtCore/QQueue>

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

        [[nodiscard]] int getAudioChunkSize() const;

        void setAudioChunkSize(int newValue);

        [[nodiscard]] int getFftSampleSize() const;

        void setFftSampleSize(int newValue);

        [[nodiscard]] qreal getGain() const;

        [[nodiscard]] bool isBfoStatus() const;

        [[nodiscard]] unsigned int getBfoFrequency() const;

    public slots:

        void start() override;

        void stop() override;

        void setGain(qreal newValue);

        void setBFOStatus(bool newValue);

        void setBFOFrequency(unsigned int newValue);

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

        int audioChunkSize;
        int fftSampleSize;

        qreal gain;
        bool bfoStatus;
        unsigned int bfoFrequency;

        QQueue<qreal> fftBuffer;

    private slots:

        void newFrame(const model::Frame &frame);

    signals:

        void newStatus(bool newStatus);

        void bufferSize(const int &size);

        void newRMS(const qreal &values);

        void newFFT(const QList<qreal> &values);

        void newValues(const QList<qreal> &values);

    };

}

#endif
