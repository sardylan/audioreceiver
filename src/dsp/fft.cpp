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

#include <QtCore/QtMath>

#include "fft.hpp"

using namespace audioreceiver::dsp;

FFT::FFT(const unsigned int &size, QObject *parent) : Service(parent), size(size) {
    input = (double *) fftw_malloc(sizeof(double) * FFT::size);
    output = (double *) fftw_malloc(sizeof(double) * FFT::size);

    plan = fftw_plan_r2r_1d((int) FFT::size, FFT::input, FFT::output, FFTW_R2HC, FFTW_ESTIMATE);
}

FFT::~FFT() {
    fftw_destroy_plan(plan);

    fftw_free(input);
    fftw_free(output);
}

QList<qreal> FFT::execute(const QList<qreal> &data) {
    fftLock.acquire();

    for (unsigned int i = 0; i < size; i++)
        input[i] = data[(int) i];

    fftw_execute(plan);

    QList<qreal> fft;

    unsigned int samples = size / 2;

    for (unsigned int i = 0; i <= samples; i++) {
        qreal re = output[i];
        qreal im = output[size - i];
        qreal magnitude = qSqrt((re * re) + (im * im));
        fft.append(magnitude);
    }

    fftLock.release();

    return fft;
}
