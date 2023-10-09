#include <esp_now.h>
#include <esp_wifi.h>
#include <WiFi.h>
#include <Wire.h>
#include <EEPROM.h>
#include <Adafruit_SSD1306.h>
#include <Bounce2.h>

#include "averageFilter.h"

#define SENSOR_PIN_0 36
#define SENSOR_PIN_1 39
#define SENSOR_PIN_2 34
#define SENSOR_PIN_3 35
#define SENSOR_PIN_4 32
#define SENSOR_PIN_5 33
#define BTN_PIN 19
#define BUILTIN_LED 13

// Set your Board and Server ID
#define BOARD_ID 0
#define MAX_CHANNEL 13
#define SAVE_CHANNEL


const uint16_t EEPROM_WIFI_CHANNEL_ADDR = 0;
const uint16_t EEPROM_WIFI_CHANNEL_LENGTH = 0;

const unsigned long send_interval = 1000 / 120;

uint8_t serverAddress[] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };

int channel = 1, lastChannel = 1;

String printed_status = "";

averageFilter f1, f2, f3, f4, f5, f6;
unsigned long millisCurrent, millisOld;
unsigned long millisCurrentPairing, millisOldPairing;

Adafruit_SSD1306 display(128, 64, &Wire, -1);

Bounce2::Button btn = Bounce2::Button();



enum PairingStatus { NOT_PAIRED,
                     PAIR_REQUEST,
                     PAIR_REQUESTED,
                     PAIR_PAIRED,
};
PairingStatus pairingStatus = NOT_PAIRED;

enum SpaghettiStatus {
  PAIRINGMONSTERS,
  CALIBRATING,
  STORING_TO_EEPROM,
  SENDING
};
SpaghettiStatus spaghettiStatus = PAIRINGMONSTERS;

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
  float s0;
  float s1;
  float s2;
  float s3;
  float s4;
  float s5;
} sensor_data;
sensor_data spaghettimonsterData, dataToSend;

typedef struct value_range {
  float max = 0;
  float min = 4095;
} value_range;
value_range r0, r1, r2, r3, r4, r5;

struct eeprom_data {
  uint8_t lastChannel;
  value_range r0;
  value_range r1;
  value_range r2;
  value_range r3;
  value_range r4;
  value_range r5;
};
eeprom_data eepromData;





// ------------------------
//  ESPNOW Functions
// ------------------------

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
    if (pairingData.id == 255) {  // the message comes from server
      printMAC(mac_addr);
      Serial.print("Pairing done for ");
      printMAC(mac_addr);
      Serial.print(" on channel ");
      Serial.print(pairingData.channel);       // channel used by the server
      addPeer(mac_addr, pairingData.channel);  // add the server  to the peer list
      display.setCursor(0, 1);
      display.print(("connected!      "));

#ifdef SAVE_CHANNEL
      // lastChannel = pairingData.channel;
      // EEPROM.write(0, pairingData.channel);
      eepromData.lastChannel = pairingData.channel;
      EEPROM.put(0, eepromData);
      EEPROM.commit();
#endif
      pairingStatus = PAIR_PAIRED;  // set the pairing status
      delay(1000);

      display.clearDisplay();
      printLogo();
      printID();
      printStatus("Connected on ch." + String(channel));
      display.display();
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

      display.clearDisplay();
      printLogo();
      printID();
      printStatus("Connecting to ch. " + String(channel));
      display.display();


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
      millisOldPairing = millis();
      pairingStatus = PAIR_REQUESTED;
      break;

    case PAIR_REQUESTED:
      // time out to allow receiving response from server
      millisCurrentPairing = millis();
      if (millisCurrentPairing - millisOldPairing >= 250) {

        millisOldPairing = millisCurrentPairing;
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



// ------------------------
//  Sensor Read Functions
// ------------------------

float mapfloat(float x, float in_min, float in_max, float out_min, float out_max) {
  return (float)(x - in_min) * (out_max - out_min) / (float)(in_max - in_min) + out_min;
}

float multisample(uint8_t analogPin, const uint8_t sampleCount = 32) {
  // returns the average from sampleCount number of reads and
  // removes the highest and lowest value
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

void clearMaxMin() {
  r0.min = 4095;
  r0.max = 0;
  r1.min = 4095;
  r1.max = 0;
  r2.min = 4095;
  r2.max = 0;
  r3.min = 4095;
  r3.max = 0;
  r4.min = 4095;
  r4.max = 0;
  r5.min = 4095;
  r5.max = 0;
}

/* char *sensorToString(float value) {
  // formats the value to a string with 3 digits, without leading zero
  // e.g. '.00', '.05', '.67', '1.0'
  char* buffer = (char*)malloc(5);
  char* dest_buffer = (char*)malloc(4);
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
} */

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

void load_eeprom() {
  EEPROM.get(0, eepromData);
  lastChannel = eepromData.lastChannel;
  r0.min = eepromData.r0.min;
  r0.max = eepromData.r0.max;
  r1.min = eepromData.r1.min;
  r1.max = eepromData.r1.max;
  r2.min = eepromData.r2.min;
  r2.max = eepromData.r2.max;
  r3.min = eepromData.r3.min;
  r3.max = eepromData.r3.max;
  r4.min = eepromData.r4.min;
  r4.max = eepromData.r4.max;
  r5.min = eepromData.r5.min;
  r5.max = eepromData.r5.max;

  display.clearDisplay();
  printLogo();
  printID();
  printStatus("Loading EEPROM...");
  display.display();

  delay(1000);
}



// ------------------------
//  Data to Serial Functions
// ------------------------

void print_eeprom_data() {
  Serial.println("last channel: " + String(eepromData.lastChannel));
  Serial.println(eepromData.r0.min);
  Serial.println(eepromData.r0.max);
  Serial.println(eepromData.r1.min);
  Serial.println(eepromData.r1.max);
  Serial.println(eepromData.r2.min);
  Serial.println(eepromData.r2.max);
  Serial.println(eepromData.r3.min);
  Serial.println(eepromData.r3.max);
  Serial.println(eepromData.r4.min);
  Serial.println(eepromData.r4.max);
  Serial.println(eepromData.r5.min);
  Serial.println(eepromData.r5.max);
}


// ------------------------
//  Display Functions
// ------------------------

void printSensorValue(float value, uint8_t index) {
  const int width = 21;
  const int height = 30;
  const int padding = 6;
  const int x = 0;
  const int y = 56;
  display.setCursor(x + index * width, y);
  // char *d = sensorToString(spaghettimonsterData.s0);
  // display.print(d);
  // free(d);

  display.setTextSize(1);
  display.print(sensorToString(value));
  display.drawRect(x + index * width + padding - 2, y - 1 * height - 4, width - padding * 2, 1 * height, SSD1306_WHITE);
  display.fillRect(x + index * width + padding - 2, y - value * height - 4, width - padding * 2, value * height, SSD1306_WHITE);
}

void printStatus(String status) {
  printed_status = status;
  display.setCursor(0, 64 - 12);
  display.setTextSize(1);
  display.print(printed_status);
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

void printLogo() {
  display.setTextWrap(false);
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  display.setCursor(0, 0);
  display.print("Spaghetti");
  display.setCursor(9, 7);
  display.print("-monster");

  display.setCursor(54, 0);
  display.setTextSize(2);
  display.print("TX");
}



// ------------------------
//  Setup + Loop
// ------------------------

void setup() {
  Serial.begin(500000);
  pinMode(BUILTIN_LED, OUTPUT);

  btn.attach(BTN_PIN, INPUT_PULLUP);
  btn.interval(25);
  btn.setPressedState(LOW);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;  // Don't proceed, loop forever
  }

  Serial.println();
  Serial.print("Client Board MAC Address:  ");
  Serial.println(WiFi.macAddress());

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();



  EEPROM.begin(sizeof(eeprom_data));
  btn.update();
  if (btn.isPressed()) {
    display.clearDisplay();
    printLogo();
    printID();
    printStatus("Skipping EEPROM!");
    display.display();
    delay(2000);

  } else {
    load_eeprom();
  }
  print_eeprom_data();

  Serial.println(lastChannel);
  if (lastChannel >= 1 && lastChannel <= MAX_CHANNEL) {
    channel = lastChannel;
  }
  Serial.println(channel);

  pairingStatus = PAIR_REQUEST;
  millisOld = millis();
}

void loop() {
  millisCurrent = millis();

  btn.update();

  spaghettimonsterData.s0 = f1.filter(analogRead(SENSOR_PIN_0)) / 4095.0;
  spaghettimonsterData.s1 = f2.filter(analogRead(SENSOR_PIN_1)) / 4095.0;
  spaghettimonsterData.s2 = f3.filter(analogRead(SENSOR_PIN_2)) / 4095.0;
  spaghettimonsterData.s3 = f4.filter(analogRead(SENSOR_PIN_3)) / 4095.0;
  spaghettimonsterData.s4 = f5.filter(analogRead(SENSOR_PIN_4)) / 4095.0;
  spaghettimonsterData.s5 = f6.filter(analogRead(SENSOR_PIN_5)) / 4095.0;

  switch (spaghettiStatus) {
    case PAIRINGMONSTERS:
      if (autoPairing() == PAIR_PAIRED) {
        spaghettiStatus = SENDING;
      }
      break;

    case CALIBRATING:
      if (btn.fell()) {
        display.clearDisplay();
        printLogo();
        printID();
        printStatus("Storing to EEPROM");
        display.display();
        spaghettiStatus = STORING_TO_EEPROM;
        break;
      }
      if (millisCurrent - millisOld >= send_interval) {
        millisOld = millisCurrent;

        display.clearDisplay();
        printLogo();
        printID();
        printSensorValue(spaghettimonsterData.s0, 0);
        printSensorValue(spaghettimonsterData.s1, 1);
        printSensorValue(spaghettimonsterData.s2, 2);
        printSensorValue(spaghettimonsterData.s3, 3);
        printSensorValue(spaghettimonsterData.s4, 4);
        printSensorValue(spaghettimonsterData.s5, 5);
        printStatus("Calibrating");
        display.display();

        set_min_max(spaghettimonsterData.s0, r0);
        set_min_max(spaghettimonsterData.s1, r1);
        set_min_max(spaghettimonsterData.s2, r2);
        set_min_max(spaghettimonsterData.s3, r3);
        set_min_max(spaghettimonsterData.s4, r4);
        set_min_max(spaghettimonsterData.s5, r5);
      }
      break;

    case STORING_TO_EEPROM:
      Serial.println("done calibrating, storing to eeprom...");
      eepromData.r0.min = r0.min;
      eepromData.r0.max = r0.max;
      eepromData.r1.min = r1.min;
      eepromData.r1.max = r1.max;
      eepromData.r2.min = r2.min;
      eepromData.r2.max = r2.max;
      eepromData.r3.min = r3.min;
      eepromData.r3.max = r3.max;
      eepromData.r4.min = r4.min;
      eepromData.r4.max = r4.max;
      eepromData.r5.min = r5.min;
      eepromData.r5.max = r5.max;

      EEPROM.put(0, eepromData);
      EEPROM.commit();
      Serial.println("data initialized and written to EEPROM.");
      delay(1000);

      load_eeprom();
      print_eeprom_data();

      display.clearDisplay();
      printLogo();
      printID();
      printStatus("Sending on ch. " + String(channel));
      display.display();
      spaghettiStatus = SENDING;
      break;

    case SENDING:
      if (btn.fell()) {
        display.clearDisplay();
        printLogo();
        printID();
        printStatus("Calibrating");
        display.display();

        clearMaxMin();
        spaghettiStatus = CALIBRATING;
        break;
      }
      if (millisCurrent - millisOld >= send_interval) {
        millisOld = millisCurrent;
        // spaghettimonsterData.s0 = multisample(SENSOR_PIN_0);
        // spaghettimonsterData.s1 = multisample(SENSOR_PIN_1);
        // spaghettimonsterData.s2 = multisample(SENSOR_PIN_2);
        // spaghettimonsterData.s3 = multisample(SENSOR_PIN_3);
        // spaghettimonsterData.s4 = multisample(SENSOR_PIN_4);
        // spaghettimonsterData.s5 = multisample(SENSOR_PIN_5);

        dataToSend.msgType = DATA;
        dataToSend.id = BOARD_ID;
        dataToSend.s0 = constrain(mapfloat(spaghettimonsterData.s0, r0.min + 0.01, r0.max - 0.01, 0, 1), 0, 1);
        dataToSend.s1 = constrain(mapfloat(spaghettimonsterData.s1, r1.min + 0.01, r1.max - 0.01, 0, 1), 0, 1);
        dataToSend.s2 = constrain(mapfloat(spaghettimonsterData.s2, r2.min + 0.01, r2.max - 0.01, 0, 1), 0, 1);
        dataToSend.s3 = constrain(mapfloat(spaghettimonsterData.s3, r3.min + 0.01, r3.max - 0.01, 0, 1), 0, 1);
        dataToSend.s4 = constrain(mapfloat(spaghettimonsterData.s4, r4.min + 0.01, r4.max - 0.01, 0, 1), 0, 1);
        dataToSend.s5 = constrain(mapfloat(spaghettimonsterData.s5, r5.min + 0.01, r5.max - 0.01, 0, 1), 0, 1);

        esp_err_t result = esp_now_send(serverAddress, (uint8_t *)&dataToSend, sizeof(sensor_data));
        if (result != ESP_OK) {
          Serial.println("Error sending the data");
        } else {
          // Serial.printf("%d\n", millis());
          Serial.printf("%.6f,%.6f,%.6f,%.6f,%.6f,%.6f\n", dataToSend.s0, dataToSend.s1, dataToSend.s2, dataToSend.s3, dataToSend.s4, dataToSend.s5);
          // Serial.printf("%.6f,%.6f,%.6f,%.6f,%.6f,%.6f\n", spaghettimonsterData.s0, spaghettimonsterData.s1, spaghettimonsterData.s2, spaghettimonsterData.s3, spaghettimonsterData.s4, spaghettimonsterData.s5);

          // display.clearDisplay();
          // printID();
          // printSensorValue(spaghettimonsterData.s0, 0);
          // printSensorValue(spaghettimonsterData.s1, 1);
          // printSensorValue(spaghettimonsterData.s2, 2);
          // printSensorValue(spaghettimonsterData.s3, 3);
          // printSensorValue(spaghettimonsterData.s4, 4);
          // printSensorValue(spaghettimonsterData.s5, 5);
          // display.display();
        }
      }
      break;
    default:
      break;
  }
}
