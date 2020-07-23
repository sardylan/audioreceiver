#!/usr/bin/python3

# audioreceiver
# Copyright (C) 2020  Luca Cireddu IS0GVH (sardylan@gmail.com, is0gvh@gmail.com)
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.


FILE_HEADER = """
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

#ifndef __AUDIORECEIVER__WIDGETS__WATERFALL_LOOKUP_COLOR_H
#define __AUDIORECEIVER__WIDGETS__WATERFALL_LOOKUP_COLOR_H

#include <QtGui/QRgba64>

static const QRgba64 waterfallColor[1024] = {
"""

FILE_FOOTER = """
};

#endif
"""

FILE_NAME = "waterfall_color.hpp"

VALUE_LINE = "        QRgba64::fromRgba(%d, %d, %d, 0)"


class WaterfallColorGenerator:
    def __init__(self):
        self._fd = None

    def run(self):
        self._file_open()
        self._write_header()
        self._generate_values()
        self._write_footer()
        self._file_close()

    def _file_open(self):
        self._fd = open(FILE_NAME, "w")

    def _write_header(self):
        self._fd.write(FILE_HEADER)

    def _generate_values(self):
        rows = []

        for i in range(0, 1024):
            if i < 256:
                r = 0
                g = 0
                b = i
            elif i < 512:
                k = i - 256
                r = k
                g = k
                b = 255 - k
            elif i < 768:
                k = i - 512
                r = 255
                g = 255 - k
                b = 0
            else:
                k = i - 768
                r = 255
                g = k
                b = k

            line = VALUE_LINE % (r, g, b)
            rows.append(line)

        self._fd.write(",\n".join(rows))

    def _write_footer(self):
        self._fd.write(FILE_FOOTER)

    def _file_close(self):
        self._fd.close()


if __name__ == "__main__":
    waterfall_color_generator = WaterfallColorGenerator()
    waterfall_color_generator.run()
