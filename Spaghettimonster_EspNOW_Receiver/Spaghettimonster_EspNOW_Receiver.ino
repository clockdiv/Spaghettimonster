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
sensor_data spaghettimonsterData;

esp_now_peer_info_t slave;
int channel = 1;

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
      memcpy(&spaghettimonsterData, incomingData, sizeof(sensor_data));
      Serial.printf("%d,%f,%f,%f,%f,%f,%f\n", spaghettimonsterData.id, spaghettimonsterData.s1, spaghettimonsterData.s2, spaghettimonsterData.s3, spaghettimonsterData.s4, spaghettimonsterData.s5, spaghettimonsterData.s6);
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
  Serial.begin(115200);

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

void loop() {}
