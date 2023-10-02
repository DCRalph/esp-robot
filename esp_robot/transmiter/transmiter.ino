#include <ESP8266WiFi.h>
#include <espnow.h>

// change this to the MAC address of the receiver/robot
//  84:F3:EB:0A:8E:ED
uint8_t addr[] = {0x8c, 0xce, 0x4e, 0xc7, 0x1c, 0xe4};


struct SEND_DATA_STRUCTURE {
  int left;
  int right;
};

SEND_DATA_STRUCTURE TXX;

unsigned long ms = 0;

bool conn;
int fail_p = 0;
char buf[8];

String serial(String in, int arg, char wat = ',') {
  String out;
  int last = 0;
  int next = 0;

  if (arg == 1) {
    out = in.substring(0, in.indexOf(wat));
  } else if (arg > 1) {
    for (int i = 1; i < arg; i++) {
      last = in.indexOf(wat, last) + 1;
    }
    next = in.indexOf(wat, last);
    out = in.substring(last, next);
  }
  return out;
}

void OnDataSent(uint8_t *mac, uint8_t sendStatus) {
  if (sendStatus == 0) {
    Serial.println("Delivery success");
    conn = true;
    fail_p = 0;
  } else {
    if (conn) {
      fail_p += 1;
    }
  }
  if(fail_p > 5){
    conn = false;
  }
}

void setup() {
  Serial.begin(115200);
  Serial.setTimeout(10);

  WiFi.mode(WIFI_STA);

  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_register_send_cb(OnDataSent);
  esp_now_add_peer(addr, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
}

void loop() {
  if (millis() - ms > 50) {
    ms = millis();
    esp_now_send(addr, (uint8_t *) &TXX, sizeof(TXX));
  }


  if (Serial.available() > 0) {
    String in = Serial.readString();

    String left = serial(in, 1);
    String right = serial(in, 2);

    TXX.left =  left.toInt();
    TXX.right = right.toInt();


    sprintf(buf, "%d,", conn);
    Serial.println(buf);
  }
}
