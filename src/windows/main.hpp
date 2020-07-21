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

#ifndef __AUDIORECEIVER__WINDOWS__MAIN_H
#define __AUDIORECEIVER__WINDOWS__MAIN_H

#include <QtCore/QtGlobal>
#include <QtCore/QtDebug>
#include <QtCore/QList>
#include <QtCore/QDateTime>

#include <QtWidgets/QMainWindow>

#include "../config.hpp"
#include "../status.hpp"

#include "../widgets/vumeter.hpp"

namespace Ui {
    class Main;
}

namespace audioreceiver::windows {

    class Main : public QMainWindow {
    Q_OBJECT

    public:

        explicit Main(audioreceiver::Config *config, audioreceiver::Status *status, QWidget *parent = nullptr);

        ~Main() override;

    public slots:

        void updateVuMeter(const qreal &value);

    private:

        Ui::Main *ui;

        audioreceiver::Config *config;
        audioreceiver::Status *status;

        widgets::VUMeter *vuMeter;

        void signalConnect();

        void initUi();

    signals:

        void openConfigWindow();

    };

}

#endif
