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


#ifndef __CUSTOMWIDGETS__VUMETER_H
#define __CUSTOMWIDGETS__VUMETER_H

#include <QtCore/QtGlobal>
#include <QtWidgets/QOpenGLWidget>

namespace customwidgets {

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
        int width;
        int height;

        qreal min;
        qreal max;
        qreal warning;
        qreal alert;

        qreal value;

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
