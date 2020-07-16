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

#include "fir.hpp"
#include "utility.hpp"

using namespace audioreceiver::dsp;

FIR::FIR(const QList<qreal> &kernel, QObject *parent) : Service(parent), kernel(kernel) {

}

FIR::~FIR() = default;

void FIR::execute(const QList<qreal> &data) {
    QList<qreal> output = Utility::convolve(data);

    QMetaObject::invokeMethod(this, "newValues", Qt::QueuedConnection, Q_ARG(QList<qreal>, output));
}
