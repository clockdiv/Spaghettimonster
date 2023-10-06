#include <esp_now.h>
#include <WiFi.h>

#include <Wire.h>
#include <Adafruit_SSD1306.h>

const uint8_t SPAGHETTIMONSTER_COUNT = 6;
const unsigned long send_interval = 1000 / 60;

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
// sensor_data spaghettimonsterData_01, spaghettimonsterData_02, spaghettimonsterData_03;
// volatile sensor_data spaghettimonsterData[SPAGHETTIMONSTER_COUNT];
sensor_data spaghettimonsterData[SPAGHETTIMONSTER_COUNT];


esp_now_peer_info_t slave;
int channel = 1;
unsigned long millisCurrent, millisOld;

unsigned long stopwatch = 0, stopwatchOld = 0;

Adafruit_SSD1306 display(128, 64, &Wire, -1);

// int counter1 = 0, counter2 = 0;

void printMAC(const uint8_t *mac_addr) {
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.print(macStr);
}

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  // Serial.print("Last Packet Send Status: ");
  // Serial.print(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success to " : "Delivery Fail to ");
  // printMAC(mac_addr);
  // Serial.println();

  if (status != ESP_NOW_SEND_SUCCESS) Serial.println("OnDataSent() - Delivery Fail");
}

void OnDataRecv(const uint8_t *mac_addr, const uint8_t *incomingData, int len) {
  // Serial.print(len);
  // Serial.print(" bytes of data received from : ");
  // printMAC(mac_addr);
  // Serial.println();
  // incomingDataGlobal = (sensor_data*)incomingData;
  uint8_t type = incomingData[0];  // first message byte is the type of message

  switch (type) {
    case DATA:  // the message is data type
      sensor_data tmp_data;
      memcpy(&tmp_data, incomingData, sizeof(sensor_data));
      if (tmp_data.id > 0) {
        memcpy(&spaghettimonsterData[tmp_data.id - 1], incomingData, sizeof(sensor_data));
      }
      // counter1++;

      break;

    case PAIRING:  // the message is a pairing request
      memcpy(&pairingData, incomingData, sizeof(pairingData));
      Serial.println(pairingData.msgType);
      Serial.println(pairingData.id);
      Serial.print("Pairing request from: ");
      printMAC(mac_addr);
      Serial.println();
      Serial.println(pairingData.channel);
      if (pairingData.id > 0) {  // do not replay to server itself
        if (pairingData.msgType == PAIRING) {
          pairingData.id = 0;  // 0 is server
          pairingData.channel = channel;
          Serial.println("send response");
          esp_err_t result = esp_now_send(mac_addr, (uint8_t *)&pairingData, sizeof(pairingData));
          addPeer(mac_addr);
        }
      }
      break;
  }
}

bool addPeer(const uint8_t *peer_addr) {  // add pairing
  memset(&slave, 0, sizeof(slave));
  const esp_now_peer_info_t *peer = &slave;
  memcpy(slave.peer_addr, peer_addr, 6);

  slave.channel = channel;  // pick a channel
  slave.encrypt = 0;        // no encryption
  // check if the peer exists
  bool exists = esp_now_is_peer_exist(slave.peer_addr);
  if (exists) {
    // Slave already paired.
    Serial.println("Already Paired");
    return true;
  } else {
    esp_err_t addStatus = esp_now_add_peer(peer);
    if (addStatus == ESP_OK) {
      Serial.println("Pair success");
      return true;
    } else {
      Serial.println("Pair failed");
      return false;
    }
  }
}

void setup() {
  Serial.begin(500000);

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
  display.print("Spaghettimonster RX");
  display.display();

  WiFi.mode(WIFI_MODE_STA);
  Serial.print("Receiver ESP Board MAC Address:  ");
  Serial.println(WiFi.macAddress());

  channel = WiFi.channel();
  Serial.print("Station IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.print("Wi-Fi Channel: ");
  Serial.println(WiFi.channel());

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_send_cb(OnDataSent);
  esp_now_register_recv_cb(OnDataRecv);

  for (uint8_t i = 0; i < SPAGHETTIMONSTER_COUNT; i++) {
    spaghettimonsterData[i].id = 255;
  }

}

void displayData(uint8_t index) {
  const int width = 21;
  const int height = 30;
  const int padding = 6;
  const int x = 0;
  const int y = 52;
  display.setCursor(x + index * width + 4, y + 4);
  display.setTextSize(1);

  if (spaghettimonsterData[index].id < 255) {
    display.print(spaghettimonsterData[index].id);

    display.fillRect(x + index * width + 0 * 2, y - spaghettimonsterData[index].s1 * height, 1, spaghettimonsterData[index].s1 * height, SSD1306_WHITE);
    display.fillRect(x + index * width + 1 * 2, y - spaghettimonsterData[index].s2 * height, 1, spaghettimonsterData[index].s2 * height, SSD1306_WHITE);
    display.fillRect(x + index * width + 2 * 2, y - spaghettimonsterData[index].s3 * height, 1, spaghettimonsterData[index].s3 * height, SSD1306_WHITE);
    display.fillRect(x + index * width + 3 * 2, y - spaghettimonsterData[index].s4 * height, 1, spaghettimonsterData[index].s4 * height, SSD1306_WHITE);
    display.fillRect(x + index * width + 4 * 2, y - spaghettimonsterData[index].s5 * height, 1, spaghettimonsterData[index].s5 * height, SSD1306_WHITE);
    display.fillRect(x + index * width + 5 * 2, y - spaghettimonsterData[index].s6 * height, 1, spaghettimonsterData[index].s6 * height, SSD1306_WHITE);

  } else {
    display.print('-');
  }
}

void print_data_as_csv() {
  for (uint8_t i = 0; i < SPAGHETTIMONSTER_COUNT; i++) {
    if (spaghettimonsterData[i].id < 255) {
      Serial.printf("%d,%.6f,%.6f,%.6f,%.6f,%.6f,%.6f,",
                    spaghettimonsterData[i].id,
                    spaghettimonsterData[i].s1,
                    spaghettimonsterData[i].s2,
                    spaghettimonsterData[i].s3,
                    spaghettimonsterData[i].s4,
                    spaghettimonsterData[i].s5,
                    spaghettimonsterData[i].s6);
    }
  }
  Serial.printf("\n");
}

void print_data_as_json() {
  Serial.printf("{");
  for (uint8_t i = 0; i < SPAGHETTIMONSTER_COUNT; i++) {
    if (spaghettimonsterData[i].id < 255) {
      Serial.printf("\"%d\":[%.6f,%.6f,%.6f,%.6f,%.6f,%.6f]",
                    spaghettimonsterData[i].id,
                    spaghettimonsterData[i].s1,
                    spaghettimonsterData[i].s2,
                    spaghettimonsterData[i].s3,
                    spaghettimonsterData[i].s4,
                    spaghettimonsterData[i].s5,
                    spaghettimonsterData[i].s6);
    }

    if (i < SPAGHETTIMONSTER_COUNT - 1) {
      if (spaghettimonsterData[i + 1].id < 255) {
        Serial.printf(",");
      }
    }
  }
  Serial.printf("}\n");
}

void print_debug_data() {
  // Serial.printf("{ '1' : '(1.000000,0.329251,0.000000,0.303578,0.134032,0.462309)', '2' : '(0.513508,0.508389,0.500000,0.530522,0.511721,0.794337)', '3' : '(1.000000,0.472681,0.474965,0.337906,0.550442,0.000000)'}");
  // Serial.printf("{\"1\":\"(0.503225,0.521750,0.214153,0.522978,0.133823,0.000000)\",\"2\":\"(0.535541,0.536594,0.500000,0.538680,0.543106,0.748236)\",\"3\":\"(0.533928,0.523421,0.524241,0.496456,0.532858,0.000000)\"}");
  // Serial.printf("{\"1\":[0.503225,0.521750,0.214153,0.522978,0.133823,0.000000],\"2\":[0.535541,0.536594,0.500000,0.538680,0.543106,0.748236],\"3\":[0.533928,0.523421,0.524241,0.496456,0.532858,0.000000]}");
  // Serial.printf("%.6f,%.6f\n", spaghettimonsterData[1].s4, (sin(millis()/100.0)+1.0)/2.0);
  // Serial.printf("%.6f\n", spaghettimonsterData[1].s4);
  // Serial.printf("%.6f,%d,%d\n", spaghettimonsterData[1].s4, counter1, counter2);
  // Serial.printf("%d\n", millis());

  Serial.printf("%d,%.6f,%.6f,%.6f,%.6f,%.6f,%.6f\n",
                spaghettimonsterData[1].id,
                spaghettimonsterData[1].s1,
                spaghettimonsterData[1].s2,
                spaghettimonsterData[1].s3,
                spaghettimonsterData[1].s4,
                spaghettimonsterData[1].s5,
                spaghettimonsterData[1].s6);
}

void print_debug_sine_data() {
  // Serial.printf("{ '1' : '(1.000000,0.329251,0.000000,0.303578,0.134032,0.462309)', '2' : '(0.513508,0.508389,0.500000,0.530522,0.511721,0.794337)', '3' : '(1.000000,0.472681,0.474965,0.337906,0.550442,0.000000)'}");
  // Serial.printf("{\"1\":\"(0.503225,0.521750,0.214153,0.522978,0.133823,0.000000)\",\"2\":\"(0.535541,0.536594,0.500000,0.538680,0.543106,0.748236)\",\"3\":\"(0.533928,0.523421,0.524241,0.496456,0.532858,0.000000)\"}");
  float offset = sin(millis() / 100.0);
  // Serial.printf("{\"1\":[0.503225,0.521750,0.214153,0.522978,0.133823,0.000000],\"2\":[0.535541,0.536594,0.500000,0.538680,0.543106,0.748236],\"3\":[0.533928,0.523421,0.524241,0.496456,0.532858,0.000000]}");
  Serial.printf("{\"%d\":[%.6f, %.6f, %.6f, %.6f, %.6f, %.6f],\"%d\":[%.6f, %.6f, %.6f, %.6f, %.6f, %.6f],\"%d\":[%.6f, %.6f, %.6f, %.6f, %.6f, %.6f]}",
                0,
                offset + 0.0,
                offset + 1.0,
                offset + 2.0,
                offset + 3.0,
                offset + 4.0,
                offset + 5.0,
                1,
                offset + 6.0,
                offset + 7.0,
                offset + 8.0,
                offset + 9.0,
                offset + 10.0,
                offset + 11.0,
                2,
                offset + 12.0,
                offset + 13.0,
                offset + 14.0,
                offset + 15.0,
                offset + 16.0,
                offset + 17.0);


  Serial.printf("\n");
}

void print_data_to_display() {
  display.clearDisplay();

  for (uint8_t i = 0; i < SPAGHETTIMONSTER_COUNT; i++) {
    displayData(i);
  }
  display.setCursor(0, 0);
  display.print("Spaghettimonster RX");
  display.display();
}

void loop() {
  millisCurrent = millis();
  if (millisCurrent - millisOld >= send_interval) {

    // print_data_as_csv();
    // print_debug_data();
    // print_debug_sine_data();
    print_data_as_json();

    // print_data_to_display();

    millisOld = millisCurrent;
  }
}
