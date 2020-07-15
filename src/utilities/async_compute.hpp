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

#ifndef __AUDIORECEVIER__UTILITIES__ASYNC_COMPUTE_H
#define __AUDIORECEVIER__UTILITIES__ASYNC_COMPUTE_H

#include "service.hpp"

using namespace audioreceiver;

namespace audioreceiver::utilities {

    class AsyncCompute : public utilities::Service {
    Q_OBJECT

    public:

        explicit AsyncCompute(QObject *parent = nullptr);

        ~AsyncCompute() override;

        void start() override;

        void stop() override;

        virtual void execute(const QList<qreal> &data) = 0;

    signals:

        virtual void newComputedValues(const QList<qreal> &data) = 0;

    };

}

#endif
