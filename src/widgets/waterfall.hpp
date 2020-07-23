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

#ifndef __AUDIORECEIVER__WIDGETS__WATERFALL_H
#define __AUDIORECEIVER__WIDGETS__WATERFALL_H

#include <QtCore/QtGlobal>
#include <QtCore/QtDebug>
#include <QtCore/QList>
#include <QtCore/QDateTime>

#include <QtWidgets/QOpenGLWidget>

namespace audioreceiver::widgets {

    class Waterfall : public QOpenGLWidget {
    Q_OBJECT

    public:

        explicit Waterfall(QWidget *parent = nullptr);

        ~Waterfall() override;

        [[nodiscard]] unsigned int getFrequency() const;


        [[nodiscard]] bool isBfoEnabled() const;


        [[nodiscard]] unsigned int getBfoFrequency() const;

    public slots:

        void setFrequency(unsigned int newValue);

        void setBfoEnabled(bool newValue);

        void setBfoFrequency(unsigned int newValue);

        void addData(const QList<qreal> &values);

    protected:

        void initializeGL() override;

        void resizeGL(int w, int h) override;

        void paintGL() override;

    private:

        int width;
        int height;

        unsigned int frequency;
        QList<QList<qreal>> dataList;

        bool bfoEnabled;
        unsigned int bfoFrequency;

        void cleanDataList();

    };

}

#endif
