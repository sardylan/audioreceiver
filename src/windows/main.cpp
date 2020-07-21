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

#include "main.hpp"
#include "ui_main.h"

using namespace audioreceiver::windows;

Main::Main(audioreceiver::Config *config, audioreceiver::Status *status, QWidget *parent) : QMainWindow(parent),
                                                                                            ui(new Ui::Main) {
    ui->setupUi(this);

    Main::config = config;
    Main::status = status;

    vuMeter = new widgets::VUMeter(this);

    signalConnect();
    initUi();
}

Main::~Main() {
    delete vuMeter;

    delete ui;
}

void Main::initUi() {
    ui->vuMeterStackedWidget->addWidget(vuMeter);
}

void Main::updateVuMeter(const qreal &value) {
    vuMeter->setValue(value);
}

void Main::signalConnect() {
    connect(ui->actionConfig, &QAction::triggered, this, &Main::openConfigWindow);
}
