/* 
 * simple teensy drum machine
 *
 * listens on MIDI channel 10, plays a few drum samples and a few simpledrum sounds.
 * uses AudioOutputMQS on Teensy4 pin 12 for output.
 * if expecting line-level output then put a resistor or a pot on the output otherwise it will be really really hot.
 *
 */
#define DEBUG
#ifdef DEBUG
	#define DEBUG_PRINT(x) Serial.println(x)
#else
	#define DEBUG_PRINT(x)
#endif
 
#include <MIDI.h>

MIDI_CREATE_INSTANCE(HardwareSerial, Serial2, MIDI);

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

#include <synth_simple_drum.h>

#include "AudioSampleKickwav.h"
#include "AudioSampleSnarewav.h"
#include "AudioSampleHatwav.h"

AudioPlayMemory          playMem1;       //xy=195.4166603088379,199.1666603088379
AudioPlayMemory          playMem2;       //xy=195.4166603088379,199.1666603088379
AudioPlayMemory          playMem3;       //xy=195.4166603088379,199.1666603088379

AudioSynthSimpleDrum		 drum1;
AudioSynthSimpleDrum		 drum2;

AudioMixer4              mixer1;         //xy=421.6666603088379,204.1666603088379
AudioMixer4              mixer2;         //xy=421.6666603088379,204.1666603088379
AudioMixer4              mixer3;         //xy=421.6666603088379,204.1666603088379

AudioOutputMQS					 mqs1;

AudioConnection          patchCord10(drum1, 0, mixer1, 0);
AudioConnection          patchCord11(drum2, 0, mixer1, 1);

AudioConnection          patchCord20(playMem1, 0, mixer2, 0);
AudioConnection          patchCord21(playMem2, 0, mixer2, 1);
AudioConnection          patchCord22(playMem3, 0, mixer2, 2);

AudioConnection          patchCord31(mixer1, 0, mixer3, 0);

AudioConnection          patchCord32(mixer2, 0, mixer3, 1);

AudioConnection          patchCord33(mixer3, 0, mqs1, 0);

void setup() {
  MIDI.begin(MIDI_CHANNEL_OMNI);
#ifdef DEBUG
  Serial.begin(57600);
#endif
  DEBUG_PRINT("program started");
	AudioMemory(15);
}

unsigned long t=0;
float thisgain;

void loop() {
  int type, note, velocity, channel, d1, d2;
  if (MIDI.read()) {
    byte type = MIDI.getType();
    switch (type) {
      case midi::NoteOn:
        note = MIDI.getData1();
        velocity = MIDI.getData2();
        channel = MIDI.getChannel();
        if (velocity > 0) {
          DEBUG_PRINT(String("Note On:  ch=") + channel + ", note=" + note + ", velocity=" + velocity);
					if (channel==10){
						thisgain=velocity/127.0;
						DEBUG_PRINT(String("gain:  ") + thisgain);
						switch (note) {
							case 36:
								//mixer2.gain(0, thisgain);
								//playMem1.play(AudioSampleKickwav);

								mixer1.gain(0, thisgain);
								drum1.frequency(30);
								drum1.length(200);
								drum1.noteOn();
								break;

							case 37:
								mixer2.gain(1, thisgain);
								playMem2.play(AudioSampleSnarewav);
								break;

							case 38:
								mixer2.gain(2, thisgain);
								playMem3.play(AudioSampleHatwav);
								break;

							case 39:
								mixer1.gain(0, thisgain);
								drum1.frequency(500);
								drum1.length(200);
								drum1.noteOn();
								break;

							case 40:
								mixer1.gain(1, thisgain);
								drum2.frequency(1000);
								drum2.length(200);
								drum2.noteOn();
								break;

							default:
								break;
						}
					}
        } else {
          DEBUG_PRINT(String("Note Off: ch=") + channel + ", note=" + note);
        }
        break;
      case midi::NoteOff:
        note = MIDI.getData1();
        velocity = MIDI.getData2();
        channel = MIDI.getChannel();
        DEBUG_PRINT(String("Note Off: ch=") + channel + ", note=" + note + ", velocity=" + velocity);
        break;

			case 248:
				// clock pulse, suppress
				break;

			case 254:
				// sensing message, ignore
				break;

			case 252:
        DEBUG_PRINT(String("clock stop"));
				break;

			case 251:
        DEBUG_PRINT(String("clock continue"));
				break;

			case 250:
        DEBUG_PRINT(String("clock start"));
				break;

      default:
        d1 = MIDI.getData1();
        d2 = MIDI.getData2();
				
        DEBUG_PRINT(String("Message, type=") + type + ", data = " + d1 + " " + d2);
				break;
    }
    t = millis();
  }
  if (millis() - t > 10000) {
    t += 10000;
    DEBUG_PRINT("(no MIDI activity)");
  }
}
