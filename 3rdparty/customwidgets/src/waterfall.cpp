/*
 * Custom QT5 Widgets
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

#include "waterfall.hpp"
#include "utility.hpp"
#include "waterfall_color_table.hpp"

#include <GL/gl.h>
#include <GL/glu.h>

#include <QDebug>
#include <QPainter>
#include <QPaintEvent>
#include <QRgb>

using namespace customwidgets;

Waterfall::Waterfall(QWidget *parent) : QOpenGLWidget(parent) {
    setMouseTracking(true);

    width = 0;
    height = 0;

    showMousePos = false;
    mousePosX = 0;
    maxFrequency = 0;

    bfoEnabled = false;
    bfoFrequency = 0;
}

Waterfall::~Waterfall() = default;

void Waterfall::initializeGL() {
    glClearColor(0, 0, 0, 1);
    glShadeModel(GL_FLAT);
}

void Waterfall::resizeGL(int w, int h) {
    width = w;
    height = h;
}

void Waterfall::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 1, height, 0);

    if (width == 0 || height == 0)
        return;

    for (int y = 0; y < height && y < dataList.size(); y++) {
        QList<qreal> lineList = dataList.at((dataList.size() - 1) - y);

        glBegin(GL_LINE_STRIP);

        glVertex2i(0, y);

        for (int listX = 0; listX < lineList.size(); listX++) {
            qreal value = lineList.at(listX);
            QColor qColor = computeRgbValue(value);
            glColor3f(qColor.redF(), qColor.greenF(), qColor.blueF());

            float x = (float) listX / (float) lineList.size();
            glVertex2f(x, y);
        }

        glEnd();
    }

    unsigned int interval = 0;
    unsigned int intervalBig = 0;

    if (maxFrequency == 8000) {
        interval = 500;
        intervalBig = 1000;
    } else if (maxFrequency == 11025) {
        interval = 500;
        intervalBig = 1000;
    } else if (maxFrequency == 22050) {
        interval = 1000;
        intervalBig = 2000;
    } else if (maxFrequency == 44100) {
        interval = 2000;
        intervalBig = 4000;
    } else if (maxFrequency == 48000) {
        interval = 2000;
        intervalBig = 4000;
    } else if (maxFrequency == 96000) {
        interval = 4000;
        intervalBig = 8000;
    }

    if (interval > 0) {
        for (unsigned int iterX = 0; iterX < maxFrequency; iterX += interval) {
            glBegin(GL_LINES);

            if (iterX % intervalBig == 0)
                glColor4ub(64, 64, 64, 128);
            else
                glColor4ub(32, 32, 32, 128);

            float x = (float) iterX / maxFrequency;
            glVertex2f(x, 0);
            glVertex2f(x, height);

            glEnd();
        }
    }

    if (bfoEnabled && bfoFrequency >= 0 && bfoFrequency <= maxFrequency) {
        float x = (float) bfoFrequency / maxFrequency;
        glBegin(GL_LINES);
        glColor4ub(255, 0, 0, 64);
        glVertex2f(x, 0);
        glVertex2f(x, height);
        glEnd();
    }

    if (showMousePos) {
        float x = (float) mousePosX / width;
        glBegin(GL_LINES);
        glColor4ub(255, 255, 255, 64);
        glVertex2f(x, 0);
        glVertex2f(x, height);
        glEnd();
    }
}

void Waterfall::mouseMoveEvent(QMouseEvent *event) {
    mousePosX = event->pos().x();
    update();

    if (event->button() == Qt::LeftButton) {
        auto frequency = (unsigned int) (((qreal) mousePosX / width) * maxFrequency);
        emit newClickFrequency(frequency);
    }
}

void Waterfall::enterEvent(QEvent *event) {
    if (event->type() == QEvent::Enter) {
        showMousePos = true;
        update();
    }
}

void Waterfall::leaveEvent(QEvent *event) {
    if (event->type() == QEvent::Leave) {
        showMousePos = false;
        update();
    }
}

unsigned int Waterfall::getMaxFrequency() const {
    return maxFrequency;
}

void Waterfall::setMaxFrequency(unsigned int value) {
    Waterfall::maxFrequency = value;
}

bool Waterfall::isBfoEnabled() const {
    return bfoEnabled;
}

void Waterfall::setBfoEnabled(bool value) {
    Waterfall::bfoEnabled = value;
}

unsigned int Waterfall::getBfoFrequency() const {
    return bfoFrequency;
}

void Waterfall::setBfoFrequency(unsigned int value) {
    Waterfall::bfoFrequency = value;
}

void Waterfall::addData(const QList<qreal> &value) {
    dataList.append(value);
    cleanDataList();
    update();
}

void Waterfall::cleanDataList() {
    while (dataList.length() > height)
        dataList.removeFirst();
}

QColor Waterfall::computeRgbValue(int value) {
    int v = value;
    if (v < 0)
        v = 0;
    if (v > CUSTOMWIDGETS_WATERFALL_DATA_MAX)
        v = CUSTOMWIDGETS_WATERFALL_DATA_MAX;

    int r = waterfall_color_table[v][0];
    int g = waterfall_color_table[v][1];
    int b = waterfall_color_table[v][2];

    return QColor::fromRgb(r, g, b);
}

QColor Waterfall::computeRgbValue(qreal value) {
    return computeRgbValue((int) value);
}
