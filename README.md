# audioreceiver

> Simple VLF receiver using PC soundcard with direct sampling.

![Audio Receiver](https://raw.githubusercontent.com/sardylan/audioreceiver/master/doc/img/audioreceiver-win.png)

This software is a simple VLF (Very Low Frequency, from 3 kHz to 30 kHz) double sideband receiver, entirely software based.
It works using the PC soundcard for direct sampling the antenna signal and applies some maths in order to obtain the desidered signal.

Since it uses the soundcard, tunable frequency interval directly depends on the sampling rate of the sound card itself.
With modern PC, sound cards embedded on motherboards are capable to sample up to 192 kHz, so you are able to tune up to 96 kHz.

Obviously, it requires a [proper antenna](https://www.google.com/search?q=indoor+VLF+antenna+design) to be connected to sound card input connector.

[Download](https://github.com/sardylan/audioreceiver/releases)

