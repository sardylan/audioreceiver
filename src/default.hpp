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

#ifndef __AUDIORECEIVER__DEFAULT_H
#define __AUDIORECEIVER__DEFAULT_H

#include <QtMultimedia/QAudioFormat>

#define CONFIG_AUDIO_INPUT_DEVICE_DEFAULT ""
#define CONFIG_AUDIO_INPUT_CHANNELS_DEFAULT 1
#define CONFIG_AUDIO_INPUT_SAMPLE_RATE_DEFAULT 48000
#define CONFIG_AUDIO_INPUT_SAMPLE_SIZE_DEFAULT 16
#define CONFIG_AUDIO_INPUT_SAMPLE_TYPE_DEFAULT QAudioFormat::SignedInt
#define CONFIG_AUDIO_INPUT_ENDIAN_DEFAULT QAudioFormat::BigEndian
#define CONFIG_AUDIO_INPUT_CODEC_DEFAULT "audio/pcm"

#define CONFIG_AUDIO_OUTPUT_DEVICE_DEFAULT ""
#define CONFIG_AUDIO_OUTPUT_CHANNELS_DEFAULT 1
#define CONFIG_AUDIO_OUTPUT_SAMPLE_RATE_DEFAULT 48000
#define CONFIG_AUDIO_OUTPUT_SAMPLE_SIZE_DEFAULT 16
#define CONFIG_AUDIO_OUTPUT_SAMPLE_TYPE_DEFAULT QAudioFormat::SignedInt
#define CONFIG_AUDIO_OUTPUT_ENDIAN_DEFAULT QAudioFormat::BigEndian
#define CONFIG_AUDIO_OUTPUT_CODEC_DEFAULT "audio/pcm"

#endif
