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
IPAddress outIp = { 192, 168, 8, 53 };  // IP Adress of Receiver PC
WiFiUDP Udp;
const unsigned int localPort = 9000;  // not needed
const unsigned int destPort = 6448;   // Wekinator Port: 6448
const char *ssid = "LABOR";
const char *password = "D1git8l.LAB";  // min. 8 characters

unsigned long millisCurrent, millisOld;

averageFilter f1, f2, f3, f4, f5, f6;
float s1, s2, s3, s4, s5, s6;
float s1Old, s2Old, s3Old, s4Old, s5Old, s6Old;

void setup() {
  Serial.begin(115200);
  //analogReadResolution(10);

  WiFi.setHostname("Spaghettimonster1");
  //WiFi.config(staticIP, subnet, gateway, dns);
  WiFi.mode(WIFI_STA);  //WiFi mode station (connect to wifi router only)
  WiFi.begin(ssid, password);

  // Wait for connection
  int timeOut = 0;
  while ((WiFi.status() != WL_CONNECTED) && (timeOut < 10)) {
    delay(500);
    timeOut++;
    Serial.println(timeOut);
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

  s1 = f1.filter(analogRead(SENSOR_PIN_1)) / 4095.0;
  s2 = f2.filter(analogRead(SENSOR_PIN_2)) / 4095.0;
  s3 = f3.filter(analogRead(SENSOR_PIN_3)) / 4095.0;
  s4 = f4.filter(analogRead(SENSOR_PIN_4)) / 4095.0;
  s5 = f5.filter(analogRead(SENSOR_PIN_5)) / 4095.0;
  s6 = f6.filter(analogRead(SENSOR_PIN_6)) / 4095.0;

  if (millisCurrent >= millisOld + 40) {
    // Serial.printf("%f,%f,%f,%f,%f,%f\n", s1, s2, s3, s4, s5, s6);  // open Serial Plotter to see a sensor-graph
    /*
    Serial.write(uint8_t(s1*255.0));
    Serial.write(uint8_t(s2*255.0));
    Serial.write(uint8_t(s3*255.0));
    Serial.write(uint8_t(s4*255.0));
    Serial.write(uint8_t(s5*255.0));
    Serial.write(uint8_t(s6*255.0));
    Serial.println();
    */
    if (WiFi.status() == WL_CONNECTED) {
      Udp.beginPacket(outIp, destPort);

      if (s1 != s1Old) {
        OSCMessage msg("/spaghetti/1");
        msg.add(s1);
        msg.send(Udp);
        Serial.printf("Sending s1: %f\n", s1);
        msg.empty();
        s1Old = s1;
      }
      if (s2 != s2Old) {
        OSCMessage msg("/spaghetti/2");
        msg.add(s2);
        msg.send(Udp);
        Serial.printf("Sending s2: %f\n", s2);
        msg.empty();
        s2Old = s2;
      }
      if (s3 != s3Old) {
        OSCMessage msg("/spaghetti/3");
        msg.add(s3);
        msg.send(Udp);
        Serial.printf("Sending s3: %f\n", s3);
        msg.empty();
        s3Old = s3;
      }
      if (s4 != s4Old) {
        OSCMessage msg("/spaghetti/4");
        msg.add(s4);
        msg.send(Udp);
        Serial.printf("Sending s4: %f\n", s4);
        msg.empty();
        s4Old = s4;
      }
      if (s5 != s5Old) {
        OSCMessage msg("/spaghetti/5");
        msg.add(s5);
        msg.send(Udp);
        Serial.printf("Sending s5: %f\n", s5);
        msg.empty();
        s5Old = s5;
      }
      if (s6 != s6Old) {
        OSCMessage msg("/spaghetti/6");
        msg.add(s6);
        msg.send(Udp);
        Serial.printf("Sending s6: %f\n", s6);
        msg.empty();
        s6Old = s6;
      }
      Udp.endPacket();
    }
    /*
    else {
      Serial.print("WiFi.status != WL_CONNECTED - ");
      Serial.println(WiFi.status());
    }
*/
    millisOld = millisCurrent;
  }
}
