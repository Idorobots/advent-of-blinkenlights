#include "hal/esp32.h"

#if defined(ARDUINO_ARCH_ESP32)

const char* ssid = ESP32_SSID;
const char* password = ESP32_PASS;

void initRTC(void) {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(500);
  }
  Serial.println("\nConnected!");

  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  configTime(0, 0, "time.nist.gov", "0.pool.ntp.org", "1.pool.ntp.org");
  Serial.println("NTP time configured.");
}

void toggleRTC(bool run) {
  // NOTE Nothing to do.
}

void setRTCTime(struct tm *time) {
  // NOTE Nothing to do, NTP handles time synchronization.
}

void getRTCTime(struct tm *time) {
  getLocalTime(time);
}


#endif
