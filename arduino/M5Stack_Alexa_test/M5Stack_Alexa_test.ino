#include <M5Stack.h>
#include "M5StackUpdater.h"
#include <WiFi.h>
#include "fauxmoESP.h"

#define SERIAL_BAUDRATE 115200

#define WIFI_SSID "BeBoX_Home"
#define WIFI_PASS "8201000501"

#define DEVICE_1 "M5"

fauxmoESP fauxmo;
bool current_state;
bool change_flag;
int current_value;

void wifiSetup() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(100);
  }
  Serial.println();
  Serial.printf("WIFI coneccted! IP: %s\n", WiFi.localIP().toString().c_str());
}

void setup() {
  Serial.begin(SERIAL_BAUDRATE);
  Serial.println();
  M5.begin();
  checkSDUpdater();
  M5.Power.begin();

  wifiSetup();

  current_state = false;
  current_value = 0;
  change_flag = false;
  
  fauxmo.createServer(true);
  fauxmo.setPort(80); 
  fauxmo.enable(true);
  fauxmo.addDevice(DEVICE_1);

  fauxmo.onSetState([](unsigned char device_id, const char * device_name, bool state, unsigned char value) { 
    Serial.printf("Device #%d (%s) state: %s value: %d\n", device_id, device_name, state ? "ON" : "OFF", value);
    current_value = value * 100 / 254;
    current_state = state;
    change_flag = true;
  });
}

void loop() {
  fauxmo.handle();

  static unsigned long last = millis();
  if (millis() - last > 5000) {
    last = millis();
    Serial.printf("[MAIN] Free heap: %d bytes\n", ESP.getFreeHeap());
  }
  if (change_flag) {
    M5.Lcd.fillScreen(current_state ? GREEN : RED);
    M5.Lcd.setCursor(100, 100);
    M5.Lcd.setTextColor(WHITE);
    M5.Lcd.setTextSize(6);
    M5.Lcd.print(current_state ? current_value : 0);
    change_flag = false;
  }
}
