#include <esp_now.h>
#include <WiFi.h>


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
//callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&spaghettimonsterData, incomingData, sizeof(sensor_data));
  Serial.printf("%f,%f,%f,%f,%f,%f\n", spaghettimonsterData.s1, spaghettimonsterData.s2, spaghettimonsterData.s3, spaghettimonsterData.s4, spaghettimonsterData.s5, spaghettimonsterData.s6);
  //  Serial.print("Bytes received: ");
  //  Serial.println(len);
  //  Serial.print("s1: ");
  //  Serial.println(spaghettimonsterData.s1);
  //  Serial.print("s2: ");
  //  Serial.println(spaghettimonsterData.s2);
  //  Serial.print("s3: ");
  //  Serial.println(spaghettimonsterData.s3);
  //  Serial.print("s4: ");
  //  Serial.println(spaghettimonsterData.s4);
  //  Serial.print("s5: ");
  //  Serial.println(spaghettimonsterData.s5);
  //  Serial.print("s6: ");
  //  Serial.println(spaghettimonsterData.s6);

}

void setup() {

  Serial.begin(115200);

  WiFi.mode(WIFI_MODE_STA);
  Serial.print("ESP Board MAC Address:  ");
  Serial.println(WiFi.macAddress());

  //Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_recv_cb(OnDataRecv);
}


void loop() {}
