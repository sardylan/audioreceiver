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

#ifndef __AUDIORECEVIER__WIDGETS__VUMETER_H
#define __AUDIORECEVIER__WIDGETS__VUMETER_H

#include <QtCore/QtGlobal>
#include <QtCore/QtDebug>
#include <QtCore/QList>
#include <QtCore/QDateTime>

#include <QtWidgets/QOpenGLWidget>

namespace audioreceiver::widgets {

    class VUMeter : public QOpenGLWidget {
    Q_OBJECT

    public:

        explicit VUMeter(QWidget *parent = nullptr);

        ~VUMeter() override;

        [[nodiscard]] qreal getMin() const;

        [[nodiscard]] qreal getMax() const;

        [[nodiscard]] qreal getWarning() const;

        [[nodiscard]] qreal getAlert() const;

        [[nodiscard]] qreal getValue() const;

    public slots:

        void setMin(qreal newValue);

        void setMax(qreal newValue);

        void setWarning(qreal newValue);

        void setAlert(qreal newValue);

        void setValue(qreal newValue);

    protected:

        void initializeGL() override;

        void resizeGL(int w, int h) override;

        void paintGL() override;

    private:

        qreal min;
        qreal max;
        qreal warning;
        qreal alert;

        qreal value;

        int width;
        int height;

        qreal interval;
        qreal halfInterval;
        qreal valueThreshold;
        qreal warningThreshold;
        qreal alertThreshold;

        void updateInterval();

        void updateValueThreshold();

        void updateWarningThreshold();

        void updateAlertThreshold();

    };

}

#endif
