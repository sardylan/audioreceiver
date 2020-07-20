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

#include "frame.hpp"

using namespace audioreceiver::model;

Frame::Frame() : number(0),
                 dateTime(QDateTime::currentDateTimeUtc()),
                 values() {

}

Frame::Frame(const quint64 number, QList<qreal> values, QDateTime dateTime) :
        number(number),
        dateTime(qMove(dateTime)),
        values(qMove(values)) {

}

Frame::Frame(Frame &other) = default;

Frame::Frame(const Frame &other) = default;

Frame::~Frame() = default;

quint64 Frame::getNumber() const {
    return number;
}

const QDateTime &Frame::getDateTime() const {
    return dateTime;
}

const QList<qreal> &Frame::getValues() const {
    return values;
}

QDebug operator<<(QDebug dbg, const audioreceiver::model::Frame &frame) {
    dbg << frame.getDateTime();
    return dbg.maybeSpace();
}
