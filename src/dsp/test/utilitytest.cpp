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

#include "utilitytest.hpp"

#include "../utility.hpp"

using namespace audioreceiver::dsp;

QTEST_MAIN(UtilityTest)

void UtilityTest::testLogScaleZero() {
    qreal input = 0;
    qreal expected = 0;
    qreal actual = Utility::logScale(input);

    Q_ASSERT(qFuzzyCompare(actual, expected));
}

void UtilityTest::testLogScaleOne() {
    qreal input = 1;
    qreal expected = 1;
    qreal actual = Utility::logScale(input);

    Q_ASSERT(qFuzzyCompare(actual, expected));
}

void UtilityTest::testLogScaleHalf() {
    qreal input = .5;
    qreal expected = .5;
    qreal actual = Utility::logScale(input);

    Q_ASSERT(actual > expected);
}

void UtilityTest::testLogScaleNegative() {
    qreal input = -123;
    qreal expected = 0;
    qreal actual = Utility::logScale(input);

    Q_ASSERT(qFuzzyCompare(actual, expected));
}

void UtilityTest::testLogScaleTooHigh() {
    qreal input = 1.1;
    qreal expected = 1;
    qreal actual = Utility::logScale(input);

    Q_ASSERT(qFuzzyCompare(actual, expected));
}
