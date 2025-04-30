#include "ESP32_NOW.h"
#include "WiFi.h"
#include <esp_mac.h>  // For the MAC2STR and MACSTR macros
#include <EEPROM.h>
#include <Bounce2.h>
#include "averageFilter.h"

#define ESPNOW_WIFI_CHANNEL 6


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

const unsigned long fps = 10;
const unsigned long send_interval = 1000 / fps;

uint8_t serverAddress[] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };

int channel = 1, lastChannel = 1;

String printed_status = "";

averageFilter f1, f2, f3, f4, f5, f6;
unsigned long millisCurrent, millisOld;
unsigned long millisCurrentPairing, millisOldPairing;

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
SpaghettiStatus spaghettiStatus = SENDING;

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



class ESP_NOW_Broadcast_Peer : public ESP_NOW_Peer {
public:
  // Constructor of the class using the broadcast address
  ESP_NOW_Broadcast_Peer(uint8_t channel, wifi_interface_t iface, const uint8_t *lmk)
    : ESP_NOW_Peer(ESP_NOW.BROADCAST_ADDR, channel, iface, lmk) {}

  // Destructor of the class
  ~ESP_NOW_Broadcast_Peer() {
    remove();
  }

  // Function to properly initialize the ESP-NOW and register the broadcast peer
  bool begin() {
    if (!ESP_NOW.begin() || !add()) {
      log_e("Failed to initialize ESP-NOW or register the broadcast peer");
      return false;
    }
    return true;
  }

  // Function to send a message to all devices within the network
  bool send_message(const uint8_t *data, size_t len) {
    if (!send(data, len)) {
      log_e("Failed to broadcast message");
      return false;
    }
    return true;
  }
};

ESP_NOW_Broadcast_Peer broadcast_peer(ESPNOW_WIFI_CHANNEL, WIFI_IF_STA, NULL);




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

  // display.clearDisplay();
  // printLogo();
  // printID();
  // printStatus("Loading EEPROM...");
  // display.display();

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
//  Setup + Loop
// ------------------------

void setup() {
  Serial.begin(115200);
  pinMode(BUILTIN_LED, OUTPUT);

  btn.attach(BTN_PIN, INPUT_PULLUP);
  btn.interval(25);
  btn.setPressedState(LOW);





  EEPROM.begin(sizeof(eeprom_data));
  btn.update();
  if (btn.isPressed()) {
    // display.clearDisplay();
    // printLogo();
    // printID();
    // printStatus("Skipping EEPROM!");
    // display.display();
    // delay(2000);

  } else {
    //    load_eeprom();
  }
  // print_eeprom_data();

  // Serial.println(lastChannel);
  // if (lastChannel >= 1 && lastChannel <= MAX_CHANNEL) {
  //   channel = lastChannel;
  // }
  // Serial.println(channel);

  // pairingStatus = PAIR_REQUEST;

  WiFi.mode(WIFI_STA);
  WiFi.setChannel(ESPNOW_WIFI_CHANNEL);
  while (!WiFi.STA.started()) {
    delay(100);
  }

  Serial.println("ESP-NOW Example - Broadcast Master");
  Serial.println("Wi-Fi parameters:");
  Serial.println("  Mode: STA");
  Serial.println("  MAC Address: " + WiFi.macAddress());
  Serial.printf("  Channel: %d\n", ESPNOW_WIFI_CHANNEL);

  // Register the broadcast peer
  if (!broadcast_peer.begin()) {
    Serial.println("Failed to initialize broadcast peer");
    Serial.println("Reebooting in 5 seconds...");
    delay(5000);
    ESP.restart();
  }

  Serial.println("Setup complete. Broadcasting messages every 5 seconds.");
  Serial.print("Size of data: ");
  Serial.println(sizeof(dataToSend));
  clearMaxMin();

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
      // if (autoPairing() == PAIR_PAIRED) {
      //   spaghettiStatus = SENDING;
      // }
      break;

    case CALIBRATING:
      if (btn.fell()) {
        // display.clearDisplay();
        // printLogo();
        // printID();
        // printStatus("Storing to EEPROM");
        // display.display();
        spaghettiStatus = STORING_TO_EEPROM;
        break;
      }
      if (millisCurrent - millisOld >= send_interval) {
        millisOld = millisCurrent;

        // display.clearDisplay();
        // printLogo();
        // printID();
        // printSensorValue(spaghettimonsterData.s0, 0);
        // printSensorValue(spaghettimonsterData.s1, 1);
        // printSensorValue(spaghettimonsterData.s2, 2);
        // printSensorValue(spaghettimonsterData.s3, 3);
        // printSensorValue(spaghettimonsterData.s4, 4);
        // printSensorValue(spaghettimonsterData.s5, 5);
        // printStatus("Calibrating");
        // display.display();

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

      // display.clearDisplay();
      // printLogo();
      // printID();
      // printStatus("Sending on ch. " + String(channel));
      // display.display();
      spaghettiStatus = SENDING;
      break;

    case SENDING:
      if (btn.fell()) {
        // display.clearDisplay();
        // printLogo();
        // printID();
        // printStatus("Calibrating");
        // display.display();

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
        // dataToSend.s0 = constrain(mapfloat(spaghettimonsterData.s0, r0.min + 0.01, r0.max - 0.01, 0, 1), 0, 1);
        // dataToSend.s1 = constrain(mapfloat(spaghettimonsterData.s1, r1.min + 0.01, r1.max - 0.01, 0, 1), 0, 1);
        // dataToSend.s2 = constrain(mapfloat(spaghettimonsterData.s2, r2.min + 0.01, r2.max - 0.01, 0, 1), 0, 1);
        // dataToSend.s3 = constrain(mapfloat(spaghettimonsterData.s3, r3.min + 0.01, r3.max - 0.01, 0, 1), 0, 1);
        // dataToSend.s4 = constrain(mapfloat(spaghettimonsterData.s4, r4.min + 0.01, r4.max - 0.01, 0, 1), 0, 1);
        // dataToSend.s5 = constrain(mapfloat(spaghettimonsterData.s5, r5.min + 0.01, r5.max - 0.01, 0, 1), 0, 1);
        dataToSend.s0 = constrain(spaghettimonsterData.s0, 0, 1);
        dataToSend.s1 = constrain(spaghettimonsterData.s1, 0, 1);
        dataToSend.s2 = constrain(spaghettimonsterData.s2, 0, 1);
        dataToSend.s3 = constrain(spaghettimonsterData.s3, 0, 1);
        dataToSend.s4 = constrain(spaghettimonsterData.s4, 0, 1);
        dataToSend.s5 = constrain(spaghettimonsterData.s5, 0, 1);


        // Broadcast a message to all devices within the network
        // char data[28];
        // snprintf(data, sizeof(data), dataToSend);

        // Serial.printf("Broadcasting message: %s\n", data);

        if (!broadcast_peer.send_message((uint8_t *)&dataToSend, sizeof(dataToSend))) {
          Serial.println("Failed to broadcast message");
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
