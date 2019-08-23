# teensy_4_drum_machine_test
Simple drum machine for Teensy 4.0 using MQS for audio

Connect pin 12 and ground to a small speaker, preferably with a capacitor inline.
Or, for line level, put a potentiometer between pin 12 and the audio+, perhaps also with a capacitor inline.

This uses the simpledrum for a few sounds, and also has a few wav sounds that have been converted for use on Teensy via wav2sketch. Look at the note numbers to see which notes to send in via MIDI.

For the MIDI, I used a basic MIDI breakout board that's around $12 on Amazon: https://www.amazon.com/gp/product/B00YDLVLVO/

Build that breakout board, connect as follows:
v+ to Teensy 3.3v pin
v= to ground
tx to pin 8 (TX2)
rx to pin 7 (RX2)

Simple as that. Audio and MIDI on Teensy 4.0!
