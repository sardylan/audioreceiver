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

#include <QtCore/QSettings>

#include "config.hpp"
#include "default.hpp"

using namespace audioreceiver;

Config::Config(QObject *parent) : QObject(parent) {
    audioInputDevice = CONFIG_AUDIO_INPUT_DEVICE_DEFAULT;
    audioInputChannels = CONFIG_AUDIO_INPUT_CHANNELS_DEFAULT;
    audioInputSampleRate = CONFIG_AUDIO_INPUT_SAMPLE_RATE_DEFAULT;
    audioInputSampleSize = CONFIG_AUDIO_INPUT_SAMPLE_SIZE_DEFAULT;
    audioInputSampleType = CONFIG_AUDIO_INPUT_SAMPLE_TYPE_DEFAULT;
    audioInputEndian = CONFIG_AUDIO_INPUT_ENDIAN_DEFAULT;
    audioInputCodec = CONFIG_AUDIO_INPUT_CODEC_DEFAULT;

    audioOutputDevice = CONFIG_AUDIO_OUTPUT_DEVICE_DEFAULT;
    audioOutputChannels = CONFIG_AUDIO_OUTPUT_CHANNELS_DEFAULT;
    audioOutputSampleRate = CONFIG_AUDIO_OUTPUT_SAMPLE_RATE_DEFAULT;
    audioOutputSampleSize = CONFIG_AUDIO_OUTPUT_SAMPLE_SIZE_DEFAULT;
    audioOutputSampleType = CONFIG_AUDIO_OUTPUT_SAMPLE_TYPE_DEFAULT;
    audioOutputEndian = CONFIG_AUDIO_OUTPUT_ENDIAN_DEFAULT;
    audioOutputCodec = CONFIG_AUDIO_OUTPUT_CODEC_DEFAULT;
}

Config::~Config() = default;

const QString &Config::getAudioInputDevice() const {
    return audioInputDevice;
}

void Config::setAudioInputDevice(const QString &newValue) {
    Config::audioInputDevice = newValue;
}

int Config::getAudioInputChannels() const {
    return audioInputChannels;
}

void Config::setAudioInputChannels(int newValue) {
    Config::audioInputChannels = newValue;
}

int Config::getAudioInputSampleRate() const {
    return audioInputSampleRate;
}

void Config::setAudioInputSampleRate(int newValue) {
    Config::audioInputSampleRate = newValue;
}

int Config::getAudioInputSampleSize() const {
    return audioInputSampleSize;
}

void Config::setAudioInputSampleSize(int newValue) {
    Config::audioInputSampleSize = newValue;
}

QAudioFormat::SampleType Config::getAudioInputSampleType() const {
    return audioInputSampleType;
}

void Config::setAudioInputSampleType(QAudioFormat::SampleType newValue) {
    Config::audioInputSampleType = newValue;
}

QAudioFormat::Endian Config::getAudioInputEndian() const {
    return audioInputEndian;
}

void Config::setAudioInputEndian(QAudioFormat::Endian newValue) {
    Config::audioInputEndian = newValue;
}

const QString &Config::getAudioInputCodec() const {
    return audioInputCodec;
}

void Config::setAudioInputCodec(const QString &newValue) {
    Config::audioInputCodec = newValue;
}

const QString &Config::getAudioOutputDevice() const {
    return audioOutputDevice;
}

void Config::setAudioOutputDevice(const QString &newValue) {
    Config::audioOutputDevice = newValue;
}

int Config::getAudioOutputChannels() const {
    return audioOutputChannels;
}

void Config::setAudioOutputChannels(int newValue) {
    Config::audioOutputChannels = newValue;
}

int Config::getAudioOutputSampleRate() const {
    return audioOutputSampleRate;
}

void Config::setAudioOutputSampleRate(int newValue) {
    Config::audioOutputSampleRate = newValue;
}

int Config::getAudioOutputSampleSize() const {
    return audioOutputSampleSize;
}

void Config::setAudioOutputSampleSize(int newValue) {
    Config::audioOutputSampleSize = newValue;
}

QAudioFormat::SampleType Config::getAudioOutputSampleType() const {
    return audioOutputSampleType;
}

void Config::setAudioOutputSampleType(QAudioFormat::SampleType newValue) {
    Config::audioOutputSampleType = newValue;
}

QAudioFormat::Endian Config::getAudioOutputEndian() const {
    return audioOutputEndian;
}

void Config::setAudioOutputEndian(QAudioFormat::Endian newValue) {
    Config::audioOutputEndian = newValue;
}

const QString &Config::getAudioOutputCodec() const {
    return audioOutputCodec;
}

void Config::setAudioOutputCodec(const QString &newValue) {
    Config::audioOutputCodec = newValue;
}

int Config::getDSPSettingsAudioChunkSize() const {
    return dspSettingsAudioChunkSize;
}

void Config::setDSPSettingsAudioChunkSize(int newValue) {
    Config::dspSettingsAudioChunkSize = newValue;
}

int Config::getDSPSettingsFFTSampleSize() const {
    return dspSettingsFFTSampleSize;
}

void Config::setDSPSettingsFFTSampleSize(int newValue) {
    dspSettingsFFTSampleSize = newValue;
}

void Config::load() {
    QSettings settings;

    settings.beginGroup(CONFIG_NAME_AUDIO_INPUT);
    setAudioInputDevice(
            settings.value(CONFIG_NAME_AUDIO_DEVICE, CONFIG_AUDIO_INPUT_DEVICE_DEFAULT).toString());
    setAudioInputChannels(
            settings.value(CONFIG_NAME_AUDIO_CHANNELS, CONFIG_AUDIO_INPUT_CHANNELS_DEFAULT).toInt());
    setAudioInputSampleRate(
            settings.value(CONFIG_NAME_AUDIO_SAMPLE_RATE, CONFIG_AUDIO_INPUT_SAMPLE_RATE_DEFAULT).toInt());
    setAudioInputSampleSize(
            settings.value(CONFIG_NAME_AUDIO_SAMPLE_SIZE, CONFIG_AUDIO_INPUT_SAMPLE_SIZE_DEFAULT).toInt());
    setAudioInputSampleType(
            settings.value(CONFIG_NAME_AUDIO_SAMPLE_TYPE,
                           CONFIG_AUDIO_INPUT_SAMPLE_TYPE_DEFAULT).value<QAudioFormat::SampleType>());
    setAudioInputEndian(
            settings.value(CONFIG_NAME_AUDIO_ENDIAN, CONFIG_AUDIO_INPUT_ENDIAN_DEFAULT).value<QAudioFormat::Endian>());
    setAudioInputCodec(
            settings.value(CONFIG_NAME_AUDIO_CODEC, CONFIG_AUDIO_INPUT_CODEC_DEFAULT).value<QString>());
    settings.endGroup();

    settings.beginGroup(CONFIG_NAME_AUDIO_OUTPUT);
    setAudioOutputDevice(
            settings.value(CONFIG_NAME_AUDIO_DEVICE, CONFIG_AUDIO_OUTPUT_DEVICE_DEFAULT).toString());
    setAudioOutputChannels(
            settings.value(CONFIG_NAME_AUDIO_CHANNELS, CONFIG_AUDIO_OUTPUT_CHANNELS_DEFAULT).toInt());
    setAudioOutputSampleRate(
            settings.value(CONFIG_NAME_AUDIO_SAMPLE_RATE, CONFIG_AUDIO_OUTPUT_SAMPLE_RATE_DEFAULT).toInt());
    setAudioOutputSampleSize(
            settings.value(CONFIG_NAME_AUDIO_SAMPLE_SIZE, CONFIG_AUDIO_OUTPUT_SAMPLE_SIZE_DEFAULT).toInt());
    setAudioOutputSampleType(
            settings.value(CONFIG_NAME_AUDIO_SAMPLE_TYPE,
                           CONFIG_AUDIO_OUTPUT_SAMPLE_TYPE_DEFAULT).value<QAudioFormat::SampleType>());
    setAudioOutputEndian(
            settings.value(CONFIG_NAME_AUDIO_ENDIAN, CONFIG_AUDIO_OUTPUT_ENDIAN_DEFAULT).value<QAudioFormat::Endian>());
    setAudioInputCodec(
            settings.value(CONFIG_NAME_AUDIO_CODEC, CONFIG_AUDIO_OUTPUT_CODEC_DEFAULT).value<QString>());
    settings.endGroup();

    settings.beginGroup(CONFIG_NAME_DSP_SETTINGS);
    setDSPSettingsAudioChunkSize(
            settings.value(CONFIG_NAME_AUDIO_CHUNK_SIZE, CONFIG_GENERAL_AUDIO_CHUNK_SIZE_DEFAULT).toInt());
    setDSPSettingsFFTSampleSize(
            settings.value(CONFIG_NAME_FFT_SAMPLE_SIZE, CONFIG_GENERAL_FFT_SAMPLE_SIZE_DEFAULT).toInt());
    settings.endGroup();
}

void Config::save() const {
    QSettings settings;

    settings.beginGroup(CONFIG_NAME_AUDIO_INPUT);
    settings.setValue(CONFIG_NAME_AUDIO_DEVICE, getAudioInputDevice());
    settings.setValue(CONFIG_NAME_AUDIO_CHANNELS, getAudioInputChannels());
    settings.setValue(CONFIG_NAME_AUDIO_SAMPLE_RATE, getAudioInputSampleRate());
    settings.setValue(CONFIG_NAME_AUDIO_SAMPLE_SIZE, getAudioInputSampleSize());
    settings.setValue(CONFIG_NAME_AUDIO_SAMPLE_TYPE, getAudioInputSampleType());
    settings.setValue(CONFIG_NAME_AUDIO_ENDIAN, getAudioInputEndian());
    settings.setValue(CONFIG_NAME_AUDIO_CODEC, getAudioInputCodec());
    settings.endGroup();

    settings.beginGroup(CONFIG_NAME_AUDIO_OUTPUT);
    settings.setValue(CONFIG_NAME_AUDIO_DEVICE, getAudioOutputDevice());
    settings.setValue(CONFIG_NAME_AUDIO_CHANNELS, getAudioOutputChannels());
    settings.setValue(CONFIG_NAME_AUDIO_SAMPLE_RATE, getAudioOutputSampleRate());
    settings.setValue(CONFIG_NAME_AUDIO_SAMPLE_SIZE, getAudioOutputSampleSize());
    settings.setValue(CONFIG_NAME_AUDIO_SAMPLE_TYPE, getAudioOutputSampleType());
    settings.setValue(CONFIG_NAME_AUDIO_ENDIAN, getAudioOutputEndian());
    settings.setValue(CONFIG_NAME_AUDIO_CODEC, getAudioOutputCodec());
    settings.endGroup();

    settings.beginGroup(CONFIG_NAME_DSP_SETTINGS);
    settings.setValue(CONFIG_NAME_AUDIO_CHUNK_SIZE, getDSPSettingsAudioChunkSize());
    settings.setValue(CONFIG_NAME_FFT_SAMPLE_SIZE, getDSPSettingsFFTSampleSize());
    settings.endGroup();

    settings.sync();
}
