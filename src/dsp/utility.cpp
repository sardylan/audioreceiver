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

#include <QtCore/QDebug>
#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QtMath>
#include <QtCore/QtEndian>
#include <QtCore/QDataStream>

#include "utility.hpp"

using namespace audioreceiver::dsp;

QList<qreal> Utility::bytesToValues(const QByteArray &data, const QAudioFormat &audioFormat) {
    int dataLength = data.length();
    int sampleSize = audioFormat.sampleSize() / 8;
    qint32 frames = audioFormat.framesForBytes(dataLength);
    int channels = audioFormat.channelCount();
    QAudioFormat::Endian endian = audioFormat.byteOrder();
    QAudioFormat::SampleType sampleType = audioFormat.sampleType();
    int frameSize = sampleSize * channels;

    qreal max = qPow(256, sampleSize);
    if (sampleType == QAudioFormat::SignedInt)
        max /= 2;

    QList<qreal> valueList;

    for (qint32 frame = 0; frame < frames; frame++) {
        qreal channelSum = 0;

        for (int channel = 0; channel < channels; channel++) {
            int offset = (frameSize * frame) + (channel * sampleSize);
            QByteArray sampleData = data.mid(offset, sampleSize);

            qreal value = 0;

            if (endian == QAudioFormat::BigEndian) {
                switch (sampleType) {
                    case QAudioFormat::UnSignedInt:
                        if (sampleSize == 1)
                            value = qFromBigEndian<quint8>(sampleData);
                        else if (sampleSize == 2)
                            value = qFromBigEndian<quint16>(sampleData);
                        else if (sampleSize == 4)
                            value = qFromBigEndian<quint32>(sampleData);
                        break;
                    case QAudioFormat::SignedInt:
                        if (sampleSize == 1)
                            value = qFromBigEndian<qint8>(sampleData);
                        else if (sampleSize == 2)
                            value = qFromBigEndian<qint16>(sampleData);
                        else if (sampleSize == 4)
                            value = qFromBigEndian<qint32>(sampleData);
                        break;
                    case QAudioFormat::Float:
                        value = qFromBigEndian<qreal>(sampleData);
                        break;
                    default:
                        break;
                }
            } else {
                switch (sampleType) {
                    case QAudioFormat::UnSignedInt:
                        if (sampleSize == 1)
                            value = qFromLittleEndian<quint8>(sampleData);
                        else if (sampleSize == 2)
                            value = qFromLittleEndian<quint16>(sampleData);
                        else if (sampleSize == 4)
                            value = qFromLittleEndian<quint32>(sampleData);
                        break;
                    case QAudioFormat::SignedInt:
                        if (sampleSize == 1)
                            value = qFromLittleEndian<qint8>(sampleData);
                        else if (sampleSize == 2)
                            value = qFromLittleEndian<qint16>(sampleData);
                        else if (sampleSize == 4)
                            value = qFromLittleEndian<qint32>(sampleData);
                        break;
                    case QAudioFormat::Float:
                        value = qFromLittleEndian<qreal>(sampleData);
                        break;
                    default:
                        break;
                }
            }

            channelSum += value;
        }

        qreal value = (channelSum / channels) / max;

        valueList.append(value);
    }

    return valueList;
}

QByteArray Utility::valuesToBytes(const QList<qreal> &values, const QAudioFormat &audioFormat) {
    int sampleSize = audioFormat.sampleSize() / 8;
    int channels = audioFormat.channelCount();
    QAudioFormat::Endian endian = audioFormat.byteOrder();
    QAudioFormat::SampleType sampleType = audioFormat.sampleType();

    qreal max = qPow(256, sampleSize);
    if (sampleType == QAudioFormat::SignedInt)
        max /= 2;

    QByteArray data;
    QDataStream dataStream(&data, QIODevice::ReadWrite);

    for (qreal value: values) {
        for (int channel = 0; channel < channels; channel++) {

            if (endian == QAudioFormat::BigEndian)
                dataStream.setByteOrder(QDataStream::BigEndian);
            else
                dataStream.setByteOrder(QDataStream::LittleEndian);

            qreal singleValue;

            switch (sampleType) {
                case QAudioFormat::UnSignedInt:
                    singleValue = value * max;

                    if (sampleSize == 1)
                        dataStream << (quint8) singleValue;
                    else if (sampleSize == 2)
                        dataStream << (quint16) singleValue;
                    else if (sampleSize == 4)
                        dataStream << (quint32) singleValue;
                    break;
                case QAudioFormat::SignedInt:
                    singleValue = value * max;

                    if (sampleSize == 1)
                        dataStream << (qint8) singleValue;
                    else if (sampleSize == 2)
                        dataStream << (qint16) singleValue;
                    else if (sampleSize == 4)
                        dataStream << (qint32) singleValue;
                    break;
                case QAudioFormat::Float:
                    dataStream << value;
                    break;
                default:
                    break;
            }
        }
    }

    return data;
}

qreal Utility::rms(const QList<qreal> &data) {
//    qDebug() << "RMS start";

    qreal sum = 0;

    for (qreal item: data)
        sum += item * item;

    if (sum == 0)
        return 0;

    qreal mean = sum / data.length();
    qreal root = qSqrt(mean);

//    qDebug() << "RMS end";

    return root;
}

qreal Utility::logScale(qreal value) {
    if (value < 0)
        return 0;
    if (value > 1)
        return 1;

    qreal v = 9 * value;
    qreal ln = log10(v + 1);
    return ln;
}
