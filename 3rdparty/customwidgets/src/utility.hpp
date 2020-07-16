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


#ifndef __CUSTOMWIDGETS__UTILITY_H
#define __CUSTOMWIDGETS__UTILITY_H

#include <QtCore/QList>

namespace customwidgets {

    class Utility {

    public:

        static QList<quint8> computeRow(const QList<qreal>& input);

        static void resample(const qreal *in, size_t in_ln, qreal *out, size_t out_ln);

    };

}

#endif
