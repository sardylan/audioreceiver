/*
 * AudioVisual
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

#include <src/utility.hpp>
#include "utilitytest.hpp"

QTEST_MAIN(CustomWidgetsUtilityTest)

void CustomWidgetsUtilityTest::testComputeRow() {
    QList<double> input = QList<double>();
    input.append(65.3);
    input.append(65.3);

    QList<quint8> expected = QList<quint8>();

    QList<quint8> actual = CustomWidgetsUtility::computeRow(input);

    QCOMPARE(actual, expected);
}

void CustomWidgetsUtilityTest::testInterpolationEqual() {
    const double in[]{1, 2, 3, 4, 5, 6, 7, 8};
    size_t in_ln = 8;

    double expected[]{1, 2, 3, 4, 5, 6, 7, 8};

    size_t actual_ln = 8;
    double actual[8];

    CustomWidgetsUtility::resample(in, in_ln, actual, actual_ln);

    for (int i = 0; i < actual_ln; i++)
        QCOMPARE(actual[i], expected[i]);
}

void CustomWidgetsUtilityTest::testInterpolationHalfSize() {
    const double in[]{1, 2, 3, 4, 5, 6, 7, 8};
    size_t in_ln = 8;

    double expected[]{1.5, 3.5, 5.5, 7.5};

    size_t actual_ln = 4;
    double actual[4];

    CustomWidgetsUtility::resample(in, in_ln, actual, actual_ln);

    for (int i = 0; i < actual_ln; i++)
        QCOMPARE(actual[i], expected[i]);
}

void CustomWidgetsUtilityTest::testInterpolationQuarterSize() {
    const double in[]{1, 2, 3, 4, 5, 6, 7, 8};
    size_t in_ln = 8;

    double expected[]{2.5, 6.5};

    size_t actual_ln = 2;
    double actual[2];

    CustomWidgetsUtility::resample(in, in_ln, actual, actual_ln);

    for (int i = 0; i < actual_ln; i++)
        QCOMPARE(actual[i], expected[i]);
}

void CustomWidgetsUtilityTest::testInterpolationOddSizeSmaller() {
    const double in[]{1, 2, 3, 4, 5, 6, 7, 8};
    size_t in_ln = 8;

    double expected[]{1.375, 2.875, 4.5, 6.125, 7.625};

    size_t actual_ln = 5;
    double actual[5];

    CustomWidgetsUtility::resample(in, in_ln, actual, actual_ln);

    for (int i = 0; i < actual_ln; i++)
        QCOMPARE(actual[i], expected[i]);
}

void CustomWidgetsUtilityTest::testInterpolationOddSizeGreater() {
    const double in[]{1, 2, 3, 4, 5, 6, 7, 8};
    size_t in_ln = 8;

    double expected[]{1.25, 1.75, 2.5, 3.25, 4, 6.25, 5.75, 6.5, 7.25, 8};

    size_t actual_ln = 10;
    double actual[10];

    CustomWidgetsUtility::resample(in, in_ln, actual, actual_ln);

    for (int i = 0; i < actual_ln; i++)
        QCOMPARE(actual[i], expected[i]);
}
