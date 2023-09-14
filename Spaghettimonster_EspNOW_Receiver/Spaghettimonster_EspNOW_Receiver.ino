#include <esp_now.h>
#include <WiFi.h>

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
sensor_data spaghettimonsterData_01, spaghettimonsterData_02, spaghettimonsterData_03;

esp_now_peer_info_t slave;
int channel = 1;
unsigned long millisCurrent, millisOld;

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
  uint8_t type = incomingData[0];  // first message byte is the type of message
  switch (type) {
    case DATA:  // the message is data type
      sensor_data tmp_data;
      memcpy(&tmp_data, incomingData, sizeof(sensor_data));
      if (tmp_data.id == 1) {
        memcpy(&spaghettimonsterData_01, incomingData, sizeof(sensor_data));
      } else if (tmp_data.id == 2) {
        memcpy(&spaghettimonsterData_02, incomingData, sizeof(sensor_data));
      } else if (tmp_data.id == 3) {
        memcpy(&spaghettimonsterData_03, incomingData, sizeof(sensor_data));
      }
      // memcpy(&spaghettimonsterData_01, incomingData, sizeof(sensor_data));
      // Serial.printf("%d,%f,%f,%f,%f,%f,%f\n", spaghettimonsterData_01.id, spaghettimonsterData_01.s1, spaghettimonsterData_01.s2, spaghettimonsterData_01.s3, spaghettimonsterData_01.s4, spaghettimonsterData_01.s5, spaghettimonsterData_01.s6);
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
}

void loop() {
  unsigned long millisCurrent = millis();
  if (millisCurrent - millisOld >= 40) {
    // Serial.printf("%d,%f,%f,%f,%f,%f,%f,",
    //               spaghettimonsterData_01.id,
    //               spaghettimonsterData_01.s1,
    //               spaghettimonsterData_01.s2,
    //               spaghettimonsterData_01.s3,
    //               spaghettimonsterData_01.s4,
    //               spaghettimonsterData_01.s5,
    //               spaghettimonsterData_01.s6);

    // Serial.printf("%d,%f,%f,%f,%f,%f,%f,",
    //               spaghettimonsterData_02.id,
    //               spaghettimonsterData_02.s1,
    //               spaghettimonsterData_02.s2,
    //               spaghettimonsterData_02.s3,
    //               spaghettimonsterData_02.s4,
    //               spaghettimonsterData_02.s5,
    //               spaghettimonsterData_02.s6);

    // Serial.printf("%d,%f,%f,%f,%f,%f,%f",
    //               spaghettimonsterData_03.id,
    //               spaghettimonsterData_03.s1,
    //               spaghettimonsterData_03.s2,
    //               spaghettimonsterData_03.s3,
    //               spaghettimonsterData_03.s4,
    //               spaghettimonsterData_03.s5,
    //               spaghettimonsterData_03.s6);
    Serial.printf("%f,%f,%f,%f,%f,%f,",
                  spaghettimonsterData_01.s1,
                  spaghettimonsterData_01.s2,
                  spaghettimonsterData_01.s3,
                  spaghettimonsterData_01.s4,
                  spaghettimonsterData_01.s5,
                  spaghettimonsterData_01.s6);

    Serial.printf("%f,%f,%f,%f,%f,%f,",
                  spaghettimonsterData_02.s1,
                  spaghettimonsterData_02.s2,
                  spaghettimonsterData_02.s3,
                  spaghettimonsterData_02.s4,
                  spaghettimonsterData_02.s5,
                  spaghettimonsterData_02.s6);

    Serial.printf("%f,%f,%f,%f,%f,%f",
                  spaghettimonsterData_03.s1,
                  spaghettimonsterData_03.s2,
                  spaghettimonsterData_03.s3,
                  spaghettimonsterData_03.s4,
                  spaghettimonsterData_03.s5,
                  spaghettimonsterData_03.s6);

    Serial.printf("\n");
    
    millisOld = millisCurrent;
  }
}
