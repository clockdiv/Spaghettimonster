#include <esp_now.h>
#include <esp_wifi.h>
#include <WiFi.h>
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
sensor_data spaghettimonsterData;

uint8_t serverAddress[] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
#ifdef SAVE_CHANNEL
int lastChannel;
#endif
int channel = 1;

averageFilter f1, f2, f3, f4, f5, f6;
unsigned long millisCurrent, millisOld;

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
      Serial.print(pairingData.channel);  // channel used by the server
      addPeer(mac_addr, pairingData.channel);  // add the server  to the peer list
#ifdef SAVE_CHANNEL
      lastChannel = pairingData.channel;
      EEPROM.write(0, pairingData.channel);
      EEPROM.commit();
#endif
      pairingStatus = PAIR_PAIRED;  // set the pairing status
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

void setup() {
  Serial.begin(115200);
  pinMode(BUILTIN_LED, OUTPUT);

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

void loop() {
  if (autoPairing() == PAIR_PAIRED) {
    unsigned long millisCurrent = millis();
    if (millisCurrent - millisOld >= 40) {
      millisOld = millisCurrent;
      spaghettimonsterData.msgType = DATA;
      spaghettimonsterData.id = BOARD_ID;
      spaghettimonsterData.s1 = f1.filter(analogRead(SENSOR_PIN_1)) / 4095.0;
      spaghettimonsterData.s2 = f2.filter(analogRead(SENSOR_PIN_2)) / 4095.0;
      spaghettimonsterData.s3 = f3.filter(analogRead(SENSOR_PIN_3)) / 4095.0;
      spaghettimonsterData.s4 = f4.filter(analogRead(SENSOR_PIN_4)) / 4095.0;
      spaghettimonsterData.s5 = f5.filter(analogRead(SENSOR_PIN_5)) / 4095.0;
      spaghettimonsterData.s6 = f6.filter(analogRead(SENSOR_PIN_6)) / 4095.0;

      esp_err_t result = esp_now_send(serverAddress, (uint8_t *)&spaghettimonsterData, sizeof(sensor_data));
      if (result != ESP_OK) {
        Serial.println("Error sending the data");
      }
    }
  }
}
