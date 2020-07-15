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

#ifndef __AUDIORECEVIER__DSP__FFT_H
#define __AUDIORECEVIER__DSP__FFT_H

#include <complex>

#ifdef __cplusplus
extern "C" {
#endif

#include <fftw3.h>

#ifdef __cplusplus
}
#endif

#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QSemaphore>

#include "../utilities/service.hpp"

using namespace audioreceiver;

namespace audioreceiver::dsp {

    class FFT : public utilities::Service {
    Q_OBJECT

    public:

        explicit FFT(const unsigned int &size, QObject *parent = nullptr);

        ~FFT() override;

        QList<qreal> execute(const QList<qreal> &data);

    private:

        const unsigned int size;

        double *input;
        double *output;

        fftw_plan plan;

        QSemaphore fftLock;

    };

}

#endif
