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

#ifndef __AUDIORECEIVER__WINDOWS__CONFIG_H
#define __AUDIORECEIVER__WINDOWS__CONFIG_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QDialog>

#include "../config.hpp"
#include "../status.hpp"

namespace Ui {
    class Config;
}

namespace audioreceiver::windows {

    class Config : public QDialog {
    Q_OBJECT

    public:

        explicit Config(QWidget *parent = nullptr);

        ~Config() override;

    private:

        audioreceiver::Config *config;
        audioreceiver::Status *status;

        Ui::Config *ui;

        void signalConnect();

        void initUi();

        void load();

        void save();

        void checkStatus();

    private slots:

        void handleOK();

        void handleApply();

        void handleClose();

        void handleReset();

        void updateInputDeviceOptions();

        void updateOutputDeviceOptions();

    };

}

#endif
