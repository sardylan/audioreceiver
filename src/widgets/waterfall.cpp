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

    mousePosX = 0;
    showMousePos = false;
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

    if (bfoEnabled && bfoFrequency >= 0 && bfoFrequency <= frequency) {
        qreal x = (qreal) bfoFrequency / frequency;
        glBegin(GL_LINES);
        glColor4ub(255, 0, 0, 64);
        glVertex2d(x, 0);
        glVertex2d(x, height);
        glEnd();
    }

    if (bfoEnabled && showMousePos) {
        qreal x = (qreal) mousePosX / width;

        glBegin(GL_LINES);
        glColor4ub(128, 128, 128, 64);
        glVertex2f(x, 0);
        glVertex2f(x, height);
        glEnd();
    }
}

void Waterfall::mouseMoveEvent(QMouseEvent *event) {
    mousePosX = event->pos().x();

    if (event->button() == Qt::LeftButton) {
        auto mouseFrequency = (unsigned int) (((double) mousePosX / width) * frequency);
        QMetaObject::invokeMethod(this, "newClickFrequency", Qt::QueuedConnection, Q_ARG(unsigned int, mouseFrequency));
    }
}

void Waterfall::enterEvent(QEvent *event) {
    if (event->type() == QEvent::Enter)
        showMousePos = true;
}

void Waterfall::leaveEvent(QEvent *event) {
    if (event->type() == QEvent::Leave)
        showMousePos = false;
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