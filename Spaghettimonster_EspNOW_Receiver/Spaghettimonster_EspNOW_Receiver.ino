#include "ESP32_NOW.h"
#include "WiFi.h"

#include <esp_mac.h>  // For the MAC2STR and MACSTR macros

#include <vector>

#include <Bounce2.h>

#define ESPNOW_WIFI_CHANNEL 6
#define SPAGHETTIMONSTER_COUNT 1
#define BTN_PIN 19

const unsigned long fps = 10;
const unsigned long send_interval = 1000 / fps;

unsigned long millisCurrent, millisOld;


Bounce2::Button btn = Bounce2::Button();



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
sensor_data spaghettimonsterData[SPAGHETTIMONSTER_COUNT];


class ESP_NOW_Peer_Class : public ESP_NOW_Peer {
public:
  // Constructor of the class
  ESP_NOW_Peer_Class(const uint8_t *mac_addr, uint8_t channel, wifi_interface_t iface, const uint8_t *lmk)
    : ESP_NOW_Peer(mac_addr, channel, iface, lmk) {}

  // Destructor of the class
  ~ESP_NOW_Peer_Class() {}

  // Function to register the master peer
  bool add_peer() {
    if (!add()) {
      log_e("Failed to register the broadcast peer");
      return false;
    }
    return true;
  }

  // Function to print the received messages from the master
  void onReceive(const uint8_t *data, size_t len, bool broadcast) {
    // Safety check: make sure the received data is large enough
    if (len >= sizeof(sensor_data)) {
      memcpy(&spaghettimonsterData[0], data, sizeof(sensor_data));
      sendSensorData();
      // Serial.println("Data successfully copied into spaghettimonsterData[0]");
      // Print the contents of spaghettimonsterData[0]
      // Serial.println("sensor_data contents:");
      // Serial.printf("  msgType: %u\n", spaghettimonsterData[0].msgType);
      // Serial.printf("  id     : %u\n", spaghettimonsterData[0].id);
      // Serial.printf("  s0     : %.2f\n", spaghettimonsterData[0].s0);
      // Serial.printf("  s1     : %.2f\n", spaghettimonsterData[0].s1);
      // Serial.printf("  s2     : %.2f\n", spaghettimonsterData[0].s2);
      // Serial.printf("  s3     : %.2f\n", spaghettimonsterData[0].s3);
      // Serial.printf("  s4     : %.2f\n", spaghettimonsterData[0].s4);
      // Serial.printf("  s5     : %.2f\n", spaghettimonsterData[0].s5);
    } else {
      Serial.println("Received data is too small to fit into sensor_data");
    }
    // Serial.printf("Received a message from master " MACSTR " (%s)\n", MAC2STR(addr()), broadcast ? "broadcast" : "unicast");
    // Serial.printf("  Message: %s\n", (char *)data);
  }

  void sendSensorData() {
    sensor_data *d = &spaghettimonsterData[0];
    Serial.print(d->msgType);
    Serial.print(",");
    Serial.print(d->id);
    Serial.print(",");
    Serial.print(d->s0, 2);
    Serial.print(",");
    Serial.print(d->s1, 2);
    Serial.print(",");
    Serial.print(d->s2, 2);
    Serial.print(",");
    Serial.print(d->s3, 2);
    Serial.print(",");
    Serial.print(d->s4, 2);
    Serial.print(",");
    Serial.print(d->s5, 2);
    Serial.println();  // newline to indicate end of packet
  }
};

// List of all the masters. It will be populated when a new master is registered
std::vector<ESP_NOW_Peer_Class> masters;


// Callback called when an unknown peer sends a message
void register_new_master(const esp_now_recv_info_t *info, const uint8_t *data, int len, void *arg) {
  if (memcmp(info->des_addr, ESP_NOW.BROADCAST_ADDR, 6) == 0) {
    Serial.printf("Unknown peer " MACSTR " sent a broadcast message\n", MAC2STR(info->src_addr));
    Serial.println("Registering the peer as a master");

    ESP_NOW_Peer_Class new_master(info->src_addr, ESPNOW_WIFI_CHANNEL, WIFI_IF_STA, NULL);

    masters.push_back(new_master);
    if (!masters.back().add_peer()) {
      Serial.println("Failed to register the new master");
      return;
    }
  } else {
    // The slave will only receive broadcast messages
    log_v("Received a unicast message from " MACSTR, MAC2STR(info->src_addr));
    log_v("Igorning the message");
  }
}



// ------------------------
//  Data to Serial Functions
// ------------------------

void print_data_as_csv() {
  for (uint8_t i = 0; i < SPAGHETTIMONSTER_COUNT; i++) {
    if (spaghettimonsterData[i].id < 255) {
      Serial.printf("%d,%.6f,%.6f,%.6f,%.6f,%.6f,%.6f,",
                    spaghettimonsterData[i].id,
                    spaghettimonsterData[i].s0,
                    spaghettimonsterData[i].s1,
                    spaghettimonsterData[i].s2,
                    spaghettimonsterData[i].s3,
                    spaghettimonsterData[i].s4,
                    spaghettimonsterData[i].s5);
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
                    spaghettimonsterData[i].s0,
                    spaghettimonsterData[i].s1,
                    spaghettimonsterData[i].s2,
                    spaghettimonsterData[i].s3,
                    spaghettimonsterData[i].s4,
                    spaghettimonsterData[i].s5);
      if (i < SPAGHETTIMONSTER_COUNT - 1) {
        if (spaghettimonsterData[i + 1].id < 255) {
          Serial.printf(",");
        }
      }
    }
  }
  Serial.printf("}\n");
}

void print_debug_data() {
  // Serial.printf("{ '1' : '(1.000000,0.329251,0.000000,0.303578,0.134032,0.462309)', '2' : '(0.513508,0.508389,0.500000,0.530522,0.511721,0.794337)', '3' : '(1.000000,0.472681,0.474965,0.337906,0.550442,0.000000)'}");
  // Serial.printf("{\"1\":\"(0.503225,0.521750,0.214153,0.522978,0.133823,0.000000)\",\"2\":\"(0.535541,0.536594,0.500000,0.538680,0.543106,0.748236)\",\"3\":\"(0.533928,0.523421,0.524241,0.496456,0.532858,0.000000)\"}");
  // Serial.printf("{\"1\":[0.503225,0.521750,0.214153,0.522978,0.133823,0.000000],\"2\":[0.535541,0.536594,0.500000,0.538680,0.543106,0.748236],\"3\":[0.533928,0.523421,0.524241,0.496456,0.532858,0.000000]}");
  // Serial.printf("%.6f,%.6f\n", spaghettimonsterData[1].s3, (sin(millis()/100.0)+1.0)/2.0);
  // Serial.printf("%.6f\n", spaghettimonsterData[1].s3);
  // Serial.printf("%.6f,%d,%d\n", spaghettimonsterData[1].s3, counter1, counter2);
  // Serial.printf("%d\n", millis());

  Serial.printf("%d,%.6f,%.6f,%.6f,%.6f,%.6f,%.6f\n",
                spaghettimonsterData[1].id,
                spaghettimonsterData[1].s0,
                spaghettimonsterData[1].s1,
                spaghettimonsterData[1].s2,
                spaghettimonsterData[1].s3,
                spaghettimonsterData[1].s4,
                spaghettimonsterData[1].s5);
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



// ------------------------
//  Setup + Loop
// ------------------------

void setup() {
  Serial.begin(115200);

  btn.attach(BTN_PIN, INPUT_PULLUP);
  btn.interval(25);
  btn.setPressedState(LOW);


  // Initialize the Wi-Fi module
  WiFi.mode(WIFI_STA);
  WiFi.setChannel(ESPNOW_WIFI_CHANNEL);
  while (!WiFi.STA.started()) {
    delay(100);
  }

  Serial.println("ESP-NOW Example - Broadcast Slave");
  Serial.println("Wi-Fi parameters:");
  Serial.println("  Mode: STA");
  Serial.println("  MAC Address: " + WiFi.macAddress());
  Serial.printf("  Channel: %d\n", ESPNOW_WIFI_CHANNEL);

  // Initialize the ESP-NOW protocol
  if (!ESP_NOW.begin()) {
    Serial.println("Failed to initialize ESP-NOW");
    Serial.println("Reeboting in 5 seconds...");
    delay(5000);
    ESP.restart();
  }

  // Register the new peer callback
  ESP_NOW.onNewPeer(register_new_master, NULL);

  Serial.println("Setup complete. Waiting for a master to broadcast a message...");
}

void loop() {
  millisCurrent = millis();

  btn.update();

  if (millisCurrent - millisOld >= send_interval) {
    millisOld = millisCurrent;
  }
}
