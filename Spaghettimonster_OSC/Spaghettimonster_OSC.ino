#include <WiFi.h>
#include <WiFiUdp.h>
#include <OSCBundle.h>

#include "averageFilter.h"

#define SENSOR_PIN_1 36
#define SENSOR_PIN_2 39
#define SENSOR_PIN_3 34
#define SENSOR_PIN_4 35
#define SENSOR_PIN_5 32
#define SENSOR_PIN_6 33

// OSC-Stuff ----------------------------------------------------------
IPAddress outIp = {192, 168, 236, 41};  //
WiFiUDP Udp;
const unsigned int localPort = 9000;
const unsigned int destPort = 6448; // Wekinator Port
const char *ssid = "xxxxxxxx";
const char *password = "xxxxxxxx";  // min. 8 characters

unsigned long millisCurrent, millisOld;

averageFilter f1, f2, f3, f4, f5, f6;
float s1, s2, s3, s4, s5, s6;
float s1Old, s2Old, s3Old, s4Old, s5Old, s6Old;

void setup() {
  Serial.begin(115200);
  analogReadResolution(10);

  WiFi.setHostname("Spaghettimonster");
  //WiFi.config(staticIP, subnet, gateway, dns);
  WiFi.mode(WIFI_STA); //WiFi mode station (connect to wifi router only)
  WiFi.begin(ssid, password);

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("local IP address: ");
  Serial.println(WiFi.localIP());

  Udp.begin(localPort);
  Serial.print("Listening at port: ");
  Serial.println(localPort);

  //  outIp = WiFi.localIP();
  //  outIp[3] = 255;         // broadcast on subnet

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
    Serial.printf("%f,%f,%f,%f,%f,%f\n", s1, s2, s3, s4, s5, s6); // open Serial Plotter to see a sensor-graph

    if (WiFi.status() == WL_CONNECTED)
    {
      OSCMessage msg("/wek/inputs");
      msg.add(s1);
      msg.add(s2);
      msg.add(s3);
      msg.add(s4);
      msg.add(s5);
      msg.add(s6);
      /*
        if (s1 != s1Old) {
        s1Old = s1;
        }
        if (s2 != s2Old) {
        s2Old = s2;
        }
        if (s3 != s3Old) {
        s3Old = s3;
        }
        if (s4 != s4Old) {
        s4Old = s4;
        }
        if (s5 != s5Old) {
        s5Old = s5;
        }
        if (s6 != s6Old) {
        s6Old = s6;
        }
      */
      Udp.beginPacket(outIp, destPort);
      msg.send(Udp);
      Udp.endPacket();
      msg.empty();

    }
    else {
      Serial.print("WiFi.status != WL_CONNECTED - ");
      Serial.println(WiFi.status());
    }

    millisOld = millisCurrent;
  }
}
