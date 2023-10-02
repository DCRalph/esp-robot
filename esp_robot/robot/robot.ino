#include <ESP8266WiFi.h>
#include <espnow.h>
#include <FastLED.h>

//mac of the receiver
//8c:ce:4e:c7:1c:e4

#define L1 D5
#define L2 12
#define R1 D8
#define R2 D7

struct RECEIVE_DATA_STRUCTURE {
  int left;
  int right;
};

RECEIVE_DATA_STRUCTURE RXX;

unsigned long ms = -9999999;
bool conn;

CRGB leds[7];

void OnDataRecv(uint8_t *mac, uint8_t *incomingData, uint8_t len) {
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  memcpy(&RXX, incomingData, sizeof(RXX));

  ms = millis();
}

void setup() {
  Serial.begin(115200);

  pinMode(L1, OUTPUT);
  pinMode(L2, OUTPUT);
  pinMode(R1, OUTPUT);
  pinMode(R2, OUTPUT);

  FastLED.addLeds<NEOPIXEL, 2>(leds, 7);


  WiFi.mode(WIFI_STA);

  if (esp_now_init() != 0) {
    while (1)
      ;
  }

  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
  conn = (millis() - ms < 200);

  for (auto &led : leds) {
    led = conn ? CRGB::Green : CRGB::Red;
  }
  FastLED.show();

  if (conn) {

    Serial.print(RXX.left);
    Serial.print(" - ");
    Serial.print(RXX.right);

    Serial.print(" - L1 ");
    Serial.print(RXX.left > 0 ? RXX.left : 0);

    Serial.print("  L2 ");
    Serial.print(RXX.left < 0 ? RXX.left * -1 : 0);

    Serial.print("  R1 ");
    Serial.print(RXX.right > 0 ? RXX.right : 0);

    Serial.print("  R2 ");
    Serial.println(RXX.right < 0 ? RXX.right * -1 : 0);


    analogWrite(L1, RXX.left > 0 ? RXX.left : 0);
    analogWrite(L2, RXX.left < 0 ? RXX.left * -1 : 0);

    analogWrite(R1, RXX.right > 0 ? RXX.right : 0);
    analogWrite(R2, RXX.right < 0 ? RXX.right * -1 : 0);


  } else {
    digitalWrite(L1, LOW);
    digitalWrite(L2, LOW);
    digitalWrite(R1, LOW);
    digitalWrite(R2, LOW);
  }
}
