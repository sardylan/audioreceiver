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

#include "QtCore/QDebug"

#include "vumeter.hpp"

using namespace audioreceiver::widgets;

VUMeter::VUMeter(QWidget *parent) {
    min = 0;
    max = 1;
    warning = .5;
    alert = .75;

    value = 0;

    width = 0;
    height = 0;

    interval = 0;
    halfInterval = 0;
    valueThreshold = 0;
    warningThreshold = 0;
    alertThreshold = 0;

    updateInterval();
}

VUMeter::~VUMeter() = default;

qreal VUMeter::getMin() const {
    return min;
}

void VUMeter::setMin(qreal newValue) {
    VUMeter::min = newValue;

    updateInterval();
    updateAlertThreshold();
    update();
}

qreal VUMeter::getMax() const {
    return max;
}

void VUMeter::setMax(qreal newValue) {
    VUMeter::max = newValue;

    updateInterval();
    updateAlertThreshold();
    update();
}

qreal VUMeter::getWarning() const {
    return warning;
}

void VUMeter::setWarning(qreal newValue) {
    VUMeter::warning = newValue;

    updateAlertThreshold();
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
    qDebug() << "Resize" << w << "x" << h;

    width = w;
    height = h;
}

void VUMeter::paintGL() {
    qDebug() << "paintGL" << valueThreshold;

    if (width == 0 || height == 0)
        return;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBegin(GL_QUAD_STRIP);

    if (valueThreshold < warningThreshold) {
        glColor3d(0, 1, 0);
        glVertex2d(-1, -1);
        glVertex2d(-1, 1);
        glVertex2d(valueThreshold, -1);
        glVertex2d(valueThreshold, 1);
    } else if (valueThreshold < alertThreshold) {
        glColor3d(0, 1, 0);
        glVertex2d(-1, -1);
        glVertex2d(-1, 1);
        glVertex2d(warningThreshold, -1);
        glVertex2d(warningThreshold, 1);
        glColor3d(1, 1, 0);
        glVertex2d(valueThreshold, -1);
        glVertex2d(valueThreshold, 1);
    } else {
        glColor3d(0, 1, 0);
        glVertex2d(-1, -1);
        glVertex2d(-1, 1);
        glVertex2d(warningThreshold, -1);
        glVertex2d(warningThreshold, 1);
        glColor3d(1, 1, 0);
        glVertex2d(alertThreshold, -1);
        glVertex2d(alertThreshold, 1);
        glColor3d(1, 0, 0);
        glVertex2d(valueThreshold, -1);
        glVertex2d(valueThreshold, 1);
    }

    glEnd();
}
