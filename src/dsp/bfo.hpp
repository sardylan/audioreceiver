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

#ifndef __AUDIORECEIVER__DSP__BFO_H
#define __AUDIORECEIVER__DSP__BFO_H

#include <QtCore/QtGlobal>
#include <QtCore/QObject>
#include <QtCore/QList>

namespace audioreceiver::dsp {

    class BFO : public QObject {
    Q_OBJECT

    public:

        explicit BFO(const int &sampleRate, QObject *parent = nullptr);

        ~BFO() override;

        [[nodiscard]] int getSampleRate() const;

        [[nodiscard]] qreal getPhase() const;

        [[nodiscard]] bool isEnabled() const;

        void setEnabled(bool value);

        [[nodiscard]] unsigned int getFrequency() const;

        void setFrequency(unsigned int value);

    public slots:

        Q_INVOKABLE QList<qreal> compute(const QList<qreal> &values);

    private:

        const int sampleRate;

        qreal phase;

        unsigned int frequency;
        bool enabled;

        QList<qreal> generateSine(int len);

    };

}

#endif
