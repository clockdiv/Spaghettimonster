#include <esp_now.h>
#include <WiFi.h>

#include "averageFilter.h"

#define SENSOR_PIN_1 36
#define SENSOR_PIN_2 39
#define SENSOR_PIN_3 34
#define SENSOR_PIN_4 35
#define SENSOR_PIN_5 32
#define SENSOR_PIN_6 33


uint8_t broadcastAddress[] = {0x40, 0x22, 0xD8, 0x5F, 0x33, 0x10};

unsigned long millisCurrent, millisOld;

averageFilter f1, f2, f3, f4, f5, f6;
//float s1, s2, s3, s4, s5, s6;
//float s1Old, s2Old, s3Old, s4Old, s5Old, s6Old;

esp_now_peer_info_t peerInfo;

typedef struct sensor_data {
  float s1;
  float s2;
  float s3;
  float s4;
  float s5;
  float s6;
} sensor_data;

sensor_data spaghettimonsterData;

// ------------------------------------------------
// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  //char macStr[18];
  //Serial.print("Packet to: ");

  // Copies the sender mac address to a string
  // snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x", mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  // Serial.print(macStr);
  // Serial.print(" send status:\t");
  //Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");

  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");

}

void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_send_cb(OnDataSent);


  // register  peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  // Add peer
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }

  millisOld = millis();
}


void loop() {
  millisCurrent = millis();

  spaghettimonsterData.s1 = f1.filter(analogRead(SENSOR_PIN_1)) / 4095.0;
  spaghettimonsterData.s2 = f2.filter(analogRead(SENSOR_PIN_2)) / 4095.0;
  spaghettimonsterData.s3 = f3.filter(analogRead(SENSOR_PIN_3)) / 4095.0;
  spaghettimonsterData.s4 = f4.filter(analogRead(SENSOR_PIN_4)) / 4095.0;
  spaghettimonsterData.s5 = f5.filter(analogRead(SENSOR_PIN_5)) / 4095.0;
  spaghettimonsterData.s6 = f6.filter(analogRead(SENSOR_PIN_6)) / 4095.0;

  if (millisCurrent >= millisOld + 40) {
    //Serial.printf("%f,%f,%f,%f,%f,%f\n", s1, s2, s3, s4, s5, s6); // open Serial Plotter to see a sensor-graph
    /*
      Serial.write(uint8_t(s1*255.0));
      Serial.write(uint8_t(s2*255.0));
      Serial.write(uint8_t(s3*255.0));
      Serial.write(uint8_t(s4*255.0));
      Serial.write(uint8_t(s5*255.0));
      Serial.write(uint8_t(s6*255.0));
      Serial.println();
    */

    esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &spaghettimonsterData, sizeof(sensor_data));
    if (result != ESP_OK) {
      Serial.println("Error sending the data");
    }
    millisOld = millisCurrent;
  }
}
