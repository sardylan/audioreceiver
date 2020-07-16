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

#include "vumeter.hpp"

#include <QtCore/QtMath>

using namespace customwidgets;

VUMeter::VUMeter(QWidget *parent) : QOpenGLWidget(parent) {
    setAutoFillBackground(false);

    height = 0;
    width = 0;

    min = 0;
    max = 0;
    warning = 0;
    alert = 0;

    interval = 0;
    value = 0;
}

VUMeter::~VUMeter() = default;

qreal VUMeter::getMin() const {
    return min;
}

void VUMeter::setMin(qreal newValue) {
    VUMeter::min = newValue;
    updateInterval();
    update();
}

qreal VUMeter::getMax() const {
    return max;
}

void VUMeter::setMax(qreal newValue) {
    VUMeter::max = newValue;
    updateInterval();
    update();
}

qreal VUMeter::getWarning() const {
    return warning;
}

void VUMeter::setWarning(qreal newValue) {
    VUMeter::warning = newValue;
    updateWarningThreshold();
    update();
}

qreal VUMeter::getAlert() const {
    return alert;
}

void VUMeter::setAlert(qreal newValue) {
    VUMeter::alert = newValue;
    updateAlertThreshold();
    update();
}

qreal VUMeter::getValue() const {
    return value;
}

void VUMeter::setValue(qreal newValue) {
    VUMeter::value = newValue;
    updateValueThreshold();
    update();
}

void VUMeter::updateInterval() {
    interval = max - min;
    halfInterval = interval / 2;

    updateValueThreshold();
    updateWarningThreshold();
    updateAlertThreshold();
}

void VUMeter::updateAlertThreshold() {
    alertThreshold = (alert / halfInterval) - 1;
}

void VUMeter::updateWarningThreshold() {
    warningThreshold = (warning / halfInterval) - 1;
}

void VUMeter::updateValueThreshold() {
    valueThreshold = (value / halfInterval) - 1;
}

void VUMeter::initializeGL() {
    glClearColor(1, 1, 1, 1);
    glShadeModel(GL_FLAT);
}

void VUMeter::resizeGL(int w, int h) {
    width = w;
    height = h;
}

void VUMeter::paintGL() {
    if (width == 0 || height == 0)
        return;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBegin(GL_QUAD_STRIP);

    if (valueThreshold < warningThreshold) {
        glColor3f(0, 1, 0);
        glVertex2f(-1, -1);
        glVertex2f(-1, 1);
        glVertex2f(valueThreshold, -1);
        glVertex2f(valueThreshold, 1);
    } else if (valueThreshold < alertThreshold) {
        glColor3f(0, 1, 0);
        glVertex2f(-1, -1);
        glVertex2f(-1, 1);
        glVertex2f(warningThreshold, -1);
        glVertex2f(warningThreshold, 1);
        glColor3f(1, 1, 0);
        glVertex2f(valueThreshold, -1);
        glVertex2f(valueThreshold, 1);
    } else {
        glColor3f(0, 1, 0);
        glVertex2f(-1, -1);
        glVertex2f(-1, 1);
        glVertex2f(warningThreshold, -1);
        glVertex2f(warningThreshold, 1);
        glColor3f(1, 1, 0);
        glVertex2f(alertThreshold, -1);
        glVertex2f(alertThreshold, 1);
        glColor3f(1, 0, 0);
        glVertex2f(valueThreshold, -1);
        glVertex2f(valueThreshold, 1);
    }

    glEnd();
}
