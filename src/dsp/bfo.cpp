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

#include "bfo.hpp"

using namespace audioreceiver::dsp;

BFO::BFO(const int &sampleRate, QObject *parent) : QObject(parent), sampleRate(sampleRate) {
    phase = 0;
    frequency = 100;
    enabled = false;
}

BFO::~BFO() = default;

int BFO::getSampleRate() const {
    return sampleRate;
}

qreal BFO::getPhase() const {
    return phase;
}

unsigned int BFO::getFrequency() const {
    return frequency;
}

void BFO::setFrequency(unsigned int value) {
    BFO::frequency = value;
}

bool BFO::isEnabled() const {
    return enabled;
}

void BFO::setEnabled(bool value) {
    BFO::enabled = value;
}

QList<qreal> BFO::mix(const QList<qreal> &values) {
    if (frequency == 0)
        return values;

    int len = values.length();

    const QList<qreal> &beats = generateSine(len);

    if (!enabled)
        return values;

    QList<qreal> output;

    for (int i = 0; i < values.length() && i < beats.length(); i++)
        output.append(values[i] * beats[i]);

    return output;
}


QList<qreal> BFO::generateSine(int len) {
    const qreal pulse = 2 * M_PI * frequency;

    QList<qreal> sine;

    for (int i = 0; i < len; i++) {
        qreal angle = pulse * ((qreal) i / sampleRate);
        qreal value = qSin(angle + phase);
        sine.append(value);
    }

    const qreal lastItem = sine.last();
    phase = qAsin(lastItem);

    return sine;
}
