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

#ifndef __AUDIORECEVIER__DSP__RMS_H
#define __AUDIORECEVIER__DSP__RMS_H

#include "../utilities/async_compute.hpp"

using namespace audioreceiver;

namespace audioreceiver::dsp {

    class RMS : public utilities::AsyncCompute {
    Q_OBJECT

    public:

        explicit RMS(QObject *parent = nullptr);

        ~RMS() override;

    public slots:

        void execute(const QList<qreal> &data) override;

    signals:

        void newComputedValues(const qreal &data) override;

    };

}

#endif
