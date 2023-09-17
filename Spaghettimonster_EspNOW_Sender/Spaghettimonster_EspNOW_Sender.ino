#include <esp_now.h>
#include <esp_wifi.h>
#include <WiFi.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#include "averageFilter.h"

#define SENSOR_PIN_1 36
#define SENSOR_PIN_2 39
#define SENSOR_PIN_3 34
#define SENSOR_PIN_4 35
#define SENSOR_PIN_5 32
#define SENSOR_PIN_6 33
#define BUILTIN_LED 13

// Set your Board and Server ID
#define BOARD_ID 1
#define MAX_CHANNEL 13  // for North America // 13 in Europe

enum PairingStatus { NOT_PAIRED,
                     PAIR_REQUEST,
                     PAIR_REQUESTED,
                     PAIR_PAIRED,
};
PairingStatus pairingStatus = NOT_PAIRED;

enum MessageType { PAIRING,
                   DATA,
};
MessageType messageType;

typedef struct struct_pairing {
  uint8_t msgType;
  uint8_t id;
  uint8_t channel;
} struct_pairing;
struct_pairing pairingData;

typedef struct sensor_data {
  uint8_t msgType;
  uint8_t id;
  float s1;
  float s2;
  float s3;
  float s4;
  float s5;
  float s6;
} sensor_data;
sensor_data spaghettimonsterData, spaghettimonsterDataRounded, spaghettimonsterDataRoundedOld;

typedef struct value_range {
  float max = 0;
  float min = 4095;
} value_range;
value_range r1, r2, r3, r4, r5, r6;

uint8_t serverAddress[] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
#ifdef SAVE_CHANNEL
int lastChannel;
#endif
int channel = 1;

averageFilter f1, f2, f3, f4, f5, f6;
unsigned long millisCurrent, millisOld;

LiquidCrystal_I2C lcd(0x27, 16, 2);

float mapfloat(float x, float in_min, float in_max, float out_min, float out_max) {
  return (float)(x - in_min) * (out_max - out_min) / (float)(in_max - in_min) + out_min;
}

void printMAC(const uint8_t *mac_addr) {
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.print(macStr);
}

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  // Serial.print("\r\nLast Packet Send to:\t");
  // printMAC(mac_addr);
  // Serial.print("\r\nLast Packet Send Status:\t");
  // Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");

  if (status != ESP_NOW_SEND_SUCCESS) Serial.println("OnDataSent() - Delivery Fail");
}

void OnDataRecv(const uint8_t *mac_addr, const uint8_t *incomingData, int len) {
  Serial.print("Packet received from: ");
  printMAC(mac_addr);
  Serial.println();
  Serial.print("data size = ");
  Serial.println(sizeof(incomingData));
  uint8_t type = incomingData[0];

  if (type == PAIRING) {
    memcpy(&pairingData, incomingData, sizeof(pairingData));
    if (pairingData.id == 0) {  // the message comes from server
      printMAC(mac_addr);
      Serial.print("Pairing done for ");
      printMAC(mac_addr);
      Serial.print(" on channel ");
      Serial.print(pairingData.channel);       // channel used by the server
      addPeer(mac_addr, pairingData.channel);  // add the server  to the peer list
      lcd.setCursor(0, 1);
      lcd.cursor_off();
      lcd.blink_off();
      lcd.print(("connected!      "));

#ifdef SAVE_CHANNEL
      lastChannel = pairingData.channel;
      EEPROM.write(0, pairingData.channel);
      EEPROM.commit();
#endif
      pairingStatus = PAIR_PAIRED;  // set the pairing status
      delay(1000);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(spaghettimonsterData.id);
    }
  }
}

void addPeer(const uint8_t *mac_addr, uint8_t chan) {
  esp_now_peer_info_t peer;
  ESP_ERROR_CHECK(esp_wifi_set_channel(chan, WIFI_SECOND_CHAN_NONE));
  esp_now_del_peer(mac_addr);
  memset(&peer, 0, sizeof(esp_now_peer_info_t));
  peer.channel = chan;
  peer.encrypt = false;
  memcpy(peer.peer_addr, mac_addr, sizeof(uint8_t[6]));
  if (esp_now_add_peer(&peer) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
  memcpy(serverAddress, mac_addr, sizeof(uint8_t[6]));
}

PairingStatus autoPairing() {
  switch (pairingStatus) {
    case PAIR_REQUEST:
      Serial.print("Pairing request on channel ");
      Serial.println(channel);

      // set WiFi channel
      ESP_ERROR_CHECK(esp_wifi_set_channel(channel, WIFI_SECOND_CHAN_NONE));
      if (esp_now_init() != ESP_OK) {
        Serial.println("Error initializing ESP-NOW");
      }

      // set callback routines
      esp_now_register_send_cb(OnDataSent);
      esp_now_register_recv_cb(OnDataRecv);

      // set pairing data to send to the server
      pairingData.msgType = PAIRING;
      pairingData.id = BOARD_ID;
      pairingData.channel = channel;

      // add peer and send request
      addPeer(serverAddress, channel);
      esp_now_send(serverAddress, (uint8_t *)&pairingData, sizeof(pairingData));
      millisOld = millis();
      pairingStatus = PAIR_REQUESTED;
      break;

    case PAIR_REQUESTED:
      // time out to allow receiving response from server
      millisCurrent = millis();
      if (millisCurrent - millisOld > 250) {
        millisOld = millisCurrent;
        // time out expired,  try next channel
        channel++;
        if (channel > MAX_CHANNEL) {
          channel = 1;
        }
        pairingStatus = PAIR_REQUEST;
      }
      break;

    case PAIR_PAIRED:
      // nothing to do here
      break;
  }
  return pairingStatus;
}

float multisample(uint8_t analogPin, const uint8_t sampleCount = 32) {
  float sum = 0;
  float highest = 0;
  float lowest = 4095;
  for (int i = 0; i < sampleCount; i++) {
    float sample = analogRead(analogPin);
    sum += sample;
    if (sample > highest) highest = sample;
    if (sample < lowest) lowest = sample;
  }
  sum -= highest;
  sum -= lowest;
  sum /= (sampleCount - 2);
  sum /= 4095.0;
  return sum;
}

void setup() {
  Serial.begin(115200);
  pinMode(BUILTIN_LED, OUTPUT);

  lcd.begin();
  lcd.clear();
  lcd.backlight();
  lcd.print("Spaghettimonster");
  lcd.setCursor(0, 1);
  lcd.print("connecting...");
  lcd.cursor_on();
  lcd.blink_on();


  Serial.println();
  Serial.print("Client Board MAC Address:  ");
  Serial.println(WiFi.macAddress());

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

#ifdef SAVE_CHANNEL
  EEPROM.begin(10);
  lastChannel = EEPROM.read(0);
  Serial.println(lastChannel);
  if (lastChannel >= 1 && lastChannel <= MAX_CHANNEL) {
    channel = lastChannel;
  }
  Serial.println(channel);
#endif

  pairingStatus = PAIR_REQUEST;

  millisOld = millis();
}

void set_min_max(float sample, value_range &vr) {
  if (sample > vr.max) vr.max = sample;
  if (sample < vr.min) vr.min = sample;
}

void loop() {
  if (autoPairing() == PAIR_PAIRED) {
    unsigned long millisCurrent = millis();
    if (millisCurrent - millisOld >= 40) {
      millisOld = millisCurrent;
      spaghettimonsterData.msgType = DATA;
      spaghettimonsterData.id = BOARD_ID;
      // spaghettimonsterData.s1 = f1.filter(analogRead(SENSOR_PIN_1)) / 4095.0;
      // spaghettimonsterData.s2 = f2.filter(analogRead(SENSOR_PIN_2)) / 4095.0;
      // spaghettimonsterData.s3 = f3.filter(analogRead(SENSOR_PIN_3)) / 4095.0;
      // spaghettimonsterData.s4 = f4.filter(analogRead(SENSOR_PIN_4)) / 4095.0;
      // spaghettimonsterData.s5 = f5.filter(analogRead(SENSOR_PIN_5)) / 4095.0;
      // spaghettimonsterData.s6 = f6.filter(analogRead(SENSOR_PIN_6)) / 4095.0;

      float sample = multisample(SENSOR_PIN_1);
      set_min_max(sample, r1);
      spaghettimonsterData.s1 = constrain(mapfloat(sample, r1.min + 0.01, r1.max - 0.01, 0, 1), 0, 1);
      spaghettimonsterDataRounded.s1 = ((int)(spaghettimonsterData.s1 * 100.0f)) / 100.0f;

      sample = multisample(SENSOR_PIN_2);
      set_min_max(sample, r2);
      spaghettimonsterData.s2 = constrain(mapfloat(sample, r2.min + 0.01, r2.max - 0.01, 0, 1), 0, 1);
      spaghettimonsterDataRounded.s2 = ((int)(spaghettimonsterData.s2 * 100.0f)) / 100.0f;


      sample = multisample(SENSOR_PIN_3);
      set_min_max(sample, r3);
      spaghettimonsterData.s3 = constrain(mapfloat(sample, r3.min + 0.01, r3.max - 0.01, 0, 1), 0, 1);
      spaghettimonsterDataRounded.s3 = ((int)(spaghettimonsterData.s3 * 100.0f)) / 100.0f;


      sample = multisample(SENSOR_PIN_4);
      set_min_max(sample, r4);
      spaghettimonsterData.s4 = constrain(mapfloat(sample, r4.min + 0.01, r4.max - 0.01, 0, 1), 0, 1);
      spaghettimonsterDataRounded.s4 = ((int)(spaghettimonsterData.s4 * 100.0f)) / 100.0f;


      sample = multisample(SENSOR_PIN_5);
      set_min_max(sample, r5);
      spaghettimonsterData.s5 = constrain(mapfloat(sample, r5.min + 0.01, r5.max - 0.01, 0, 1), 0, 1);
      spaghettimonsterDataRounded.s5 = ((int)(spaghettimonsterData.s5 * 100.0f)) / 100.0f;


      sample = multisample(SENSOR_PIN_6);
      set_min_max(sample, r6);
      spaghettimonsterData.s6 = constrain(mapfloat(sample, r6.min + 0.01, r6.max - 0.01, 0, 1), 0, 1);
      spaghettimonsterDataRounded.s6 = ((int)(spaghettimonsterData.s6 * 100.0f)) / 100.0f;


      esp_err_t result = esp_now_send(serverAddress, (uint8_t *)&spaghettimonsterData, sizeof(sensor_data));
      if (result != ESP_OK) {
        Serial.println("Error sending the data");
      } else {
        if (spaghettimonsterDataRounded.s1 != spaghettimonsterDataRoundedOld.s1) {
          lcd.setCursor(4, 0);
          lcd.printf("%0.2f", spaghettimonsterDataRounded.s1);
          spaghettimonsterDataRoundedOld.s1 = spaghettimonsterDataRounded.s1;
        }
        if (spaghettimonsterDataRounded.s2 != spaghettimonsterDataRoundedOld.s2) {
          lcd.setCursor(8, 0);
          lcd.printf("%.2f", spaghettimonsterDataRounded.s2);
          spaghettimonsterDataRoundedOld.s2 = spaghettimonsterDataRounded.s2;
        }
        if (spaghettimonsterDataRounded.s3 != spaghettimonsterDataRoundedOld.s3) {
          lcd.setCursor(12, 0);
          lcd.printf("%.2f", spaghettimonsterDataRounded.s3);
          spaghettimonsterDataRoundedOld.s3 = spaghettimonsterDataRounded.s3;
        }
        if (spaghettimonsterDataRounded.s4 != spaghettimonsterDataRoundedOld.s4) {
          lcd.setCursor(4, 1);
          lcd.printf("%.2f", spaghettimonsterDataRounded.s4);
          spaghettimonsterDataRoundedOld.s4 = spaghettimonsterDataRounded.s4;
        }
        if (spaghettimonsterDataRounded.s5 != spaghettimonsterDataRoundedOld.s5) {
          lcd.setCursor(8, 1);
          lcd.printf("%.2f", spaghettimonsterDataRounded.s5);
          spaghettimonsterDataRoundedOld.s5 = spaghettimonsterDataRounded.s5;
        }
        if (spaghettimonsterDataRounded.s6 != spaghettimonsterDataRoundedOld.s6) {
          lcd.setCursor(12, 1);
          lcd.printf("%.2f", spaghettimonsterDataRounded.s6);
          spaghettimonsterDataRoundedOld.s6 = spaghettimonsterDataRounded.s6;
        }
      }
    }
  }
}
