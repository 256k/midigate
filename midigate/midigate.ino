
/*
 * midigate
 *
 * convert incoming usb midi notes into discrete gate output signals
 *
 *
 */ 

#include "MIDIUSB.h"
int C_GATE = 7;
int D_GATE = 8;
int E_GATE = 9;
// list of all root notes in order of midi note number (0-11)
char *notes[] = {"c", "c#", "d", "d#", "e", "f", "f#", "g", "g#", "a", "a#", "b"};
unsigned long timestamp;
unsigned long oldtimestamp;


// First parameter is the event type (0x09 = note on, 0x08 = note off).
// Second parameter is note-on/note-off, combined with the channel.
// Channel can be anything between 0-15. Typically reported to the user as 1-16.
// Third parameter is the note number (48 = middle C).
// Fourth parameter is the velocity (64 = normal, 127 = fastest).

void noteOn(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOn = {0x09, 0x90 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOn);
}

void noteOff(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOff = {0x08, 0x80 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOff);
}

void setup() {
  Serial.begin(115200);
  pinMode(C_GATE, OUTPUT);
  pinMode(D_GATE, OUTPUT);
  pinMode(E_GATE, OUTPUT);
}

// First parameter is the event type (0x0B = control change).
// Second parameter is the event type, combined with the channel.
// Third parameter is the control number number (0-119).
// Fourth parameter is the control value (0-127).

void controlChange(byte channel, byte control, byte value) {
  midiEventPacket_t event = {0x0B, 0xB0 | channel, control, value};
  MidiUSB.sendMIDI(event);
}

void loop() {
  midiEventPacket_t rx;
  do {
    rx = MidiUSB.read();
    // Serial.println(rx.header);
    if (rx.header == 9) {
      digitalWrite(E_GATE, HIGH);
      oldtimestamp = millis();
      // timestamp = millis();
      // if (timestamp - oldtimestamp > 500) {
      //   digitalWrite(E_GATE, LOW);
      //   Serial.print(timestamp);
      // } {
      //   timestamp = millis();
      // }
      
    }
    if (rx.header != 0) {
      Serial.print("header : ");
      Serial.println(rx.header);

      if (notes[(rx.byte2 % 12)] == "c" && rx.byte1 == 144) { // byte1 = 128 means noteOn 144 // note off?
        digitalWrite(C_GATE, HIGH);
      } else {
        digitalWrite(C_GATE, LOW);
      }
      if (notes[(rx.byte2 % 12)] == "d" && rx.byte1 == 144) {
        digitalWrite(D_GATE, HIGH);
      } else {
        digitalWrite(D_GATE, LOW);
      }
      if (notes[(rx.byte2 % 12)] == "e" && rx.byte1 == 144) {
        digitalWrite(E_GATE, HIGH);
      } else {
        digitalWrite(E_GATE, LOW);
      }
      Serial.print("byte1: ");
      Serial.println(rx.byte1);
      Serial.println(notes[(rx.byte2 % 12)]);
      Serial.print("Received: ");
      Serial.print(rx.header, HEX);
      Serial.print("-");
      Serial.print(rx.byte1, HEX);
      Serial.print("-");
      // Serial.print(rx.byte2, HEX);
      Serial.print(rx.byte2);
      Serial.print("-");
      Serial.println(rx.byte3, HEX);
    }

    
  } while (rx.header != 0);
}
