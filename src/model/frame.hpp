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

#ifndef __AUDIORECEIVER__MODEL__FRAME_H
#define __AUDIORECEIVER__MODEL__FRAME_H

#include <QtCore/QtGlobal>
#include <QtCore/QtDebug>
#include <QtCore/QList>
#include <QtCore/QDateTime>

namespace audioreceiver::model {

    class Frame {

    public:

        Frame();

        ~Frame();

        Frame(const Frame &frame);

        Frame(QList<qreal> values, QDateTime dateTime = QDateTime::currentDateTimeUtc());

        [[nodiscard]] const QDateTime &getDateTime() const;

        [[nodiscard]] const QList<qreal> &getValues() const;

    private:

        const QDateTime dateTime;
        const QList<qreal> values;

    };

}

Q_DECLARE_METATYPE(audioreceiver::model::Frame)

QDebug operator<<(QDebug dbg, const audioreceiver::model::Frame &frame);

#endif
