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

#include <QtCore/QDebug>

#include <GL/gl.h>

#include "waterfall.hpp"
#include "waterfall_color.hpp"

using namespace audioreceiver::widgets;

Waterfall::Waterfall(QWidget *parent) {
    setMouseTracking(true);

    width = 0;
    height = 0;

    frequency = 0;
    bfoEnabled = false;
    bfoFrequency = 0;
}

Waterfall::~Waterfall() = default;

unsigned int Waterfall::getFrequency() const {
    return frequency;
}

void Waterfall::setFrequency(unsigned int newValue) {
    Waterfall::frequency = newValue;
}

bool Waterfall::isBfoEnabled() const {
    return bfoEnabled;
}

void Waterfall::setBfoEnabled(bool newValue) {
    Waterfall::bfoEnabled = newValue;
}

unsigned int Waterfall::getBfoFrequency() const {
    return bfoFrequency;
}

void Waterfall::setBfoFrequency(unsigned int newValue) {
    Waterfall::bfoFrequency = newValue;
}

void Waterfall::initializeGL() {
    glClearColor(0, 0, 0, 1);
    glShadeModel(GL_FLAT);
}

void Waterfall::resizeGL(int w, int h) {
    width = w;
    height = h;
}

void Waterfall::paintGL() {
    if (width == 0 || height == 0)
        return;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, 1, height, 0, -1, 1);

    for (int y = 0; y < height && y < dataList.size(); y++) {
        QList<qreal> lineList = dataList.at((dataList.size() - 1) - y);

        glBegin(GL_LINE_STRIP);

        glVertex2i(0, y);

        for (int listX = 0; listX < lineList.size(); listX++) {
            qreal value = lineList.at(listX) * WIDGETS_WATERFALL_COLORS;
            int colorValue = (int) value;
            QRgba64 color = waterfallColor[colorValue];
            glColor4ub(color.red8(), color.green8(), color.blue8(), color.alpha8());

            qreal x = (qreal) listX / lineList.size();
            glVertex2d(x, y);
        }

        glEnd();
    }

//    unsigned int interval = 0;
//    unsigned int intervalBig = 0;
//
//    if (maxFrequency == 8000) {
//        interval = 500;
//        intervalBig = 1000;
//    } else if (maxFrequency == 11025) {
//        interval = 500;
//        intervalBig = 1000;
//    } else if (maxFrequency == 22050) {
//        interval = 1000;
//        intervalBig = 2000;
//    } else if (maxFrequency == 44100) {
//        interval = 2000;
//        intervalBig = 4000;
//    } else if (maxFrequency == 48000) {
//        interval = 2000;
//        intervalBig = 4000;
//    } else if (maxFrequency == 96000) {
//        interval = 4000;
//        intervalBig = 8000;
//    }
//
//    if (interval > 0) {
//        for (int iterX = 0; iterX < maxFrequency; iterX += interval) {
//            glBegin(GL_LINES);
//
//            if (iterX % intervalBig == 0)
//                glColor4ub(64, 64, 64, 128);
//            else
//                glColor4ub(32, 32, 32, 128);
//
//            float x = (float) iterX / maxFrequency;
//            glVertex2f(x, 0);
//            glVertex2f(x, height);
//
//            glEnd();
//        }
//    }

    if (bfoEnabled && bfoFrequency >= 0 && bfoFrequency <= frequency) {
        qreal x = (qreal) bfoFrequency / frequency;
        glBegin(GL_LINES);
        glColor4ub(255, 0, 0, 64);
        glVertex2d(x, 0);
        glVertex2d(x, height);
        glEnd();
    }

//    if (showMousePos) {
//        float x = (float) mousePosX / width;
//        glBegin(GL_LINES);
//        glColor4ub(255, 255, 255, 64);
//        glVertex2f(x, 0);
//        glVertex2f(x, height);
//        glEnd();
//    }
}

void Waterfall::addData(const QList<qreal> &values) {
    dataList.append(values);
    cleanDataList();
    update();
}

void Waterfall::cleanDataList() {
    while (dataList.length() > height)
        dataList.removeFirst();
}