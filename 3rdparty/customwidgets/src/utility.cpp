/*
 * Custom QT5 Widgets
 * Copyright (C) 2019  Luca Cireddu
 * sardylan@gmail.com
 * https://www.lucacireddu.it
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "utility.hpp"

using namespace customwidgets;

QList<quint8> Utility::computeRow(const QList<qreal> &input) {
    const QList<uint8_t> &output = QList<uint8_t>();
    return output;
}

void Utility::resample(const qreal *in, size_t in_ln, qreal *out, size_t out_ln) {
    qreal step = (qreal) in_ln / (qreal) out_ln;

    for (size_t i = 0; i < out_ln; i++) {
        qreal start = step * i;
        qreal end = step * (qreal) (i + 1);

        int first = (int) start;
        int last = (int) end;

        qreal v = 0;

        for (int j = first; j <= last; j++) {
            qreal coef = 1;

            if (j == first)
                coef = 1 - (start - first);
            else if (j == last)
                coef = (end - last);

            v += in[j] * coef;
        }

        v /= step;
        *(out + i) = v;
    }
}
