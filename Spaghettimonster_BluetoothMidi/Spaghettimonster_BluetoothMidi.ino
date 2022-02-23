#include <BLEMidi.h>  // https://github.com/max22-/ESP32-BLE-MIDI
#include "averageFilter.h"

#define SENSOR_PIN_1 36
#define SENSOR_PIN_2 39
#define SENSOR_PIN_3 34
#define SENSOR_PIN_4 35
#define SENSOR_PIN_5 32
#define SENSOR_PIN_6 33

unsigned long millisCurrent, millisOld;

averageFilter f1, f2, f3, f4, f5, f6;
float s1, s2, s3, s4, s5, s6;
float s1Old, s2Old, s3Old, s4Old, s5Old, s6Old;

void setup() {
  Serial.begin(115200);
  analogReadResolution(10);
  BLEMidiServer.begin("Spaghettimonster");
  millisOld = millis();
}


void loop() {
  millisCurrent = millis();
  s1 = map(f1.filter(analogRead(SENSOR_PIN_1)), 0, 1023, 0, 126);
  s2 = map(f2.filter(analogRead(SENSOR_PIN_2)), 0, 1023, 0, 126);
  s3 = map(f3.filter(analogRead(SENSOR_PIN_3)), 0, 1023, 0, 126);
  s4 = map(f4.filter(analogRead(SENSOR_PIN_4)), 0, 1023, 0, 126);
  s5 = map(f5.filter(analogRead(SENSOR_PIN_5)), 0, 1023, 0, 126);
  s6 = map(f6.filter(analogRead(SENSOR_PIN_6)), 0, 1023, 0, 126);

  if (millisCurrent >= millisOld + 40) {
    Serial.printf("%f,%f,%f,%f,%f,%f\n", s1, s2, s3, s4, s5, s6);

    if (BLEMidiServer.isConnected())
    {
      if (s1 != s1Old) {
        BLEMidiServer.controlChange(0, 1, s1);
        s1Old = s1;
      }
      if (s2 != s2Old) {
        BLEMidiServer.controlChange(0, 2, s2);
        s2Old = s2;
      }
      if (s3 != s3Old) {
        BLEMidiServer.controlChange(0, 3, s3);
        s3Old = s3;
      }
      if (s4 != s4Old) {
        BLEMidiServer.noteOff(0, s4Old, 127);
        BLEMidiServer.noteOn(0, s4, 127);
        s4Old = s4;
      }
      if (s5 != s5Old) {
        BLEMidiServer.noteOff(1, s5Old, 127);
        BLEMidiServer.noteOn(1, s5, 127);
        s5Old = s5;
      }
      if (s6 != s6Old) {
        BLEMidiServer.noteOff(2, s6Old, 127);
        BLEMidiServer.noteOn(2, s6, 127);
        s6Old = s6;
      }
    }

    millisOld = millisCurrent;
  }
}
