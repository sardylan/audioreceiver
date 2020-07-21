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

#include <QtCore/QDebug>
#include <QtCore/QVector>
#include <utility>

#include "fir.hpp"

using namespace audioreceiver::dsp;

FIR::FIR(QList<qreal> kernel, QObject *parent) : QObject(parent), kernel(qMove(kernel)) {
    enabled = false;
}

FIR::~FIR() = default;

bool FIR::isEnabled() const {
    return enabled;
}

void FIR::setEnabled(bool value) {
    FIR::enabled = value;
}

QList<qreal> FIR::compute(const QList<qreal> &input) {
    if (input.isEmpty() || !enabled || kernel.isEmpty())
        return input;

    int len = input.length() + (kernel.length() - 1);
    if (len <= 0)
        return input;

//    qDebug() << "FIR start";

    QVector<qreal> output;
    output.resize(len);

    for (int i = 0; i < input.length(); i++) {
        auto &inputSample = const_cast<qreal &>(input[i]);

        for (int j = 0; j < kernel.length(); j++) {
            int pos = i + j;
            qreal &oldValue = output[pos];
            qreal &kernelValue = kernel[j];
            qreal newValue = oldValue + (inputSample * kernelValue);
            output[pos] = newValue;
        }
    }

//    qDebug() << "FIR end";

    return output.toList();
}
