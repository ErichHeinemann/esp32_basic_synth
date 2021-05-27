# esp32_basic_synth
ESP32 based simple synthesizer project

The project can be seen in my video https://www.youtube.com/watch?v=5XVK5MOKmZw

The project has been tested on the ESP32 DEVKIT - DOIT
An external audio DAC is recommended.

2021-05-26 E.Heinemann
Marcel did a better implementation of Pitchbend and MOD-Wheel. Therefore, I went back to his code.
Integration of AKWF-Waveforms is experimental and replaced the waveform "silence".
The implementation of the MIDI-Library is lost for now.
I added the Reverb and changed the Delay to Stereo. It is the same consumption of Memory as the version before.


2021-04-20 E.Heinemann
I tried to use the Arduino MIDI Library to create a small basic synth to extend an "Samplerbox.org" - Sampler based on Raspberry Pi.
This sampler usese 3.3 Volts for MIDI and the Serial RX-Pin of the PI.
I "glued" the ESP32 between the existing Optocoupler 6n139 to GPIO16 of the ESP32.
The TX-Pin GPIO17 is connected to the RX-Pin of the Raspberry PI (Samplerbox).

In the file midi_interface.ino, You could allow MIDI-Through which is needs to be activated for the used purpose with the ESP32.

I was able to power a Raspberry Pi3 with the standard powersupply with 2.5Amps and connect the followeing boards to its 5Volts:
- Standard Hitachi Display 16x2 with PCF8574 Backpack
- 2 PCM5102 Boards ( one for the raspi as "Hifiberry" and a second for the ESP32 )
- MIDI - In Board with 6N139

Pitchbend added on 2021-04-21, Pitchrange fixed to 1 Octave UP/Down
Modulation-Wheel to manipulte Pitch-Modulation added on 2021-04-22

ToDo / Missing functions:
Program-Management
Mapping of CCs to existing code for ADSR, Filter, Delay
Velocity - Mapping to a Modulation-"Matrix"??
Volume-Control
