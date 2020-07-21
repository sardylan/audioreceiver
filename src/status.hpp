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

#ifndef __AUDIORECEIVER__STATUS_H
#define __AUDIORECEIVER__STATUS_H

#include <QtCore/QObject>

namespace audioreceiver {

    class Status : public QObject {
    Q_OBJECT

    public:

        explicit Status(QObject *parent = nullptr);

        ~Status() override;

        [[nodiscard]] bool isRunning() const;

    private:

        bool running;

    public slots:

        void setRunning(bool newValue);

    signals:

        void updateRunning(bool value);

    };

}

#endif
