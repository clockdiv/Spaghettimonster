#include <esp_now.h>
#include <esp_wifi.h>
#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>

#include "averageFilter.h"

#define SENSOR_PIN_1 36
#define SENSOR_PIN_2 39
#define SENSOR_PIN_3 34
#define SENSOR_PIN_4 35
#define SENSOR_PIN_5 32
#define SENSOR_PIN_6 33
#define BUILTIN_LED 13

// Set your Board and Server ID
#define BOARD_ID 3
#define MAX_CHANNEL 13  // for North America // 13 in Europe
const unsigned long send_interval = 1000.0 / 60.0;

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
// sensor_data spaghettimonsterData, spaghettimonsterDataRounded, spaghettimonsterDataRoundedOld;
sensor_data spaghettimonsterData;

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

Adafruit_SSD1306 display(128, 64, &Wire, -1);


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
      display.setCursor(0, 1);
      display.print(("connected!      "));

#ifdef SAVE_CHANNEL
      lastChannel = pairingData.channel;
      EEPROM.write(0, pairingData.channel);
      EEPROM.commit();
#endif
      pairingStatus = PAIR_PAIRED;  // set the pairing status
      delay(1000);
      display.clearDisplay();
      display.setCursor(0, 0);
      display.print(spaghettimonsterData.id);
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

void set_min_max(float sample, value_range &vr) {
  if (sample > vr.max) vr.max = sample;
  if (sample < vr.min) vr.min = sample;
}

void printID() {
  display.setTextSize(1);
  if (BOARD_ID < 10) {
    display.setCursor(128 - 12 - 14, 7);
    display.print("ID");

    display.setTextSize(2);
    display.setCursor(128 - 12, 0);
    display.print(BOARD_ID);
    display.setTextSize(1);
  } else {
    display.setCursor(128 - 24 - 14, 7);
    display.print("ID");

    display.setTextSize(2);
    display.setCursor(128 - 24, 0);
    display.print(BOARD_ID);
    display.setTextSize(1);
  }
}

// char *sensorToString(float value) {
//   // formats the value to a string with 3 digits, without leading zero
//   // e.g. '.00', '.05', '.67', '1.0'
//   char* buffer = (char*)malloc(5);
//   char* dest_buffer = (char*)malloc(4);
//   if (value < 1) {
//     snprintf(buffer, 5, "%.2f", value);
//     for (int i = 0; i < 3; i++) {
//       dest_buffer[i] = buffer[i + 1];
//     }
//   } else {
//     char onedotzero[] = { '1', '.', '0' };
//     memcpy(dest_buffer, onedotzero, sizeof(onedotzero));
//   }
//   dest_buffer[3] = '\0';
//   return dest_buffer;
// }

String sensorToString(float value) {
  // formats the value to a string with 3 digits, without leading zero
  // e.g. '.00', '.05', '.67', '1.0'
  char buffer[5];
  char dest_buffer[4];
  if (value < 1) {
    snprintf(buffer, 5, "%.2f", value);
    for (int i = 0; i < 3; i++) {
      dest_buffer[i] = buffer[i + 1];
    }
  } else {
    char onedotzero[] = { '1', '.', '0' };
    memcpy(dest_buffer, onedotzero, sizeof(onedotzero));
  }

  dest_buffer[3] = '\0';
  return dest_buffer;
}

void printSensorValue(float value, uint8_t index) {
  const int width = 21;
  const int height = 30;
  const int padding = 6;
  const int x = 0;
  const int y = 56;
  display.setCursor(x + index * width, y);
  // char *d = sensorToString(spaghettimonsterData.s1);
  // display.print(d);
  // free(d);

  display.setTextSize(1);
  display.print(sensorToString(value));
  display.drawRect(x + index * width + padding - 2, y - 1 * height - 4, width - padding * 2, 1 * height, SSD1306_WHITE);
  display.fillRect(x + index * width + padding - 2, y - value * height - 4, width - padding * 2, value * height, SSD1306_WHITE);
}

void setup() {
  Serial.begin(115200);
  pinMode(BUILTIN_LED, OUTPUT);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;  // Don't proceed, loop forever
  }

  display.setTextWrap(false);
  display.clearDisplay();
  display.setTextSize(1);  // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);

  display.setCursor(0, 0);
  display.print("Spaghettimonster");

  printID();

  display.setCursor(0, 64 - 12);
  display.print("connecting...");
  display.display();

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
    if (millisCurrent - millisOld >= send_interval) {
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
      // spaghettimonsterDataRounded.s1 = ((int)(spaghettimonsterData.s1 * 100.0f)) / 100.0f;

      sample = multisample(SENSOR_PIN_2);
      set_min_max(sample, r2);
      spaghettimonsterData.s2 = constrain(mapfloat(sample, r2.min + 0.01, r2.max - 0.01, 0, 1), 0, 1);
      // spaghettimonsterDataRounded.s2 = ((int)(spaghettimonsterData.s2 * 100.0f)) / 100.0f;


      sample = multisample(SENSOR_PIN_3);
      set_min_max(sample, r3);
      spaghettimonsterData.s3 = constrain(mapfloat(sample, r3.min + 0.01, r3.max - 0.01, 0, 1), 0, 1);
      // spaghettimonsterDataRounded.s3 = ((int)(spaghettimonsterData.s3 * 100.0f)) / 100.0f;


      sample = multisample(SENSOR_PIN_4);
      set_min_max(sample, r4);
      spaghettimonsterData.s4 = constrain(mapfloat(sample, r4.min + 0.01, r4.max - 0.01, 0, 1), 0, 1);
      // spaghettimonsterDataRounded.s4 = ((int)(spaghettimonsterData.s4 * 100.0f)) / 100.0f;


      sample = multisample(SENSOR_PIN_5);
      set_min_max(sample, r5);
      spaghettimonsterData.s5 = constrain(mapfloat(sample, r5.min + 0.01, r5.max - 0.01, 0, 1), 0, 1);
      // spaghettimonsterDataRounded.s5 = ((int)(spaghettimonsterData.s5 * 100.0f)) / 100.0f;


      sample = multisample(SENSOR_PIN_6);
      set_min_max(sample, r6);
      spaghettimonsterData.s6 = constrain(mapfloat(sample, r6.min + 0.01, r6.max - 0.01, 0, 1), 0, 1);
      // spaghettimonsterDataRounded.s6 = ((int)(spaghettimonsterData.s6 * 100.0f)) / 100.0f;


      esp_err_t result = esp_now_send(serverAddress, (uint8_t *)&spaghettimonsterData, sizeof(sensor_data));
      if (result != ESP_OK) {
        Serial.println("Error sending the data");
      } else {
        display.clearDisplay();
        printID();
        printSensorValue(spaghettimonsterData.s1, 0);
        printSensorValue(spaghettimonsterData.s2, 1);
        printSensorValue(spaghettimonsterData.s3, 2);
        printSensorValue(spaghettimonsterData.s4, 3);
        printSensorValue(spaghettimonsterData.s5, 4);
        printSensorValue(spaghettimonsterData.s6, 5);
        display.display();
      }
    }
  }
}
