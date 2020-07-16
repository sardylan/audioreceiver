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


#ifndef __CUSTOMWIDGETS__TEST__UTILITYTEST_H
#define __CUSTOMWIDGETS__TEST__UTILITYTEST_H

#include <QtTest>

class CustomWidgetsUtilityTest : public QObject {
Q_OBJECT

private slots:

    void testComputeRow();

    void testInterpolationEqual();

    void testInterpolationHalfSize();

    void testInterpolationQuarterSize();

    void testInterpolationOddSizeSmaller();

    void testInterpolationOddSizeGreater();

};

#endif
