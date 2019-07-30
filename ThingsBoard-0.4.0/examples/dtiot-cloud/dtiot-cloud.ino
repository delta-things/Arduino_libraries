#include "ThingsBoard.h"

#include <ESP8266WiFi.h>


#define WIFI_AP             "BR-2G"
#define WIFI_PASSWORD       ""
#define TOKEN               "9XjaYHS5hjbjj6DxniLL"
#define DTIOT_SERVER  "dtiot.ddns.net"
#define DTIOT_PORT 30003

// Baud rate for debug serial
#define SERIAL_DEBUG_BAUD   115200

// Initialize ThingsBoard client
WiFiClient espClient;
// Initialize ThingsBoard instance
ThingsBoard tb(espClient);
// the Wifi radio's status
int status = WL_IDLE_STATUS;

void setup() {
  // initialize serial for debugging
  Serial.begin(SERIAL_DEBUG_BAUD);
  WiFi.begin(WIFI_AP, WIFI_PASSWORD);
  InitWiFi();
}

void loop() {
  delay(1000);

  if (WiFi.status() != WL_CONNECTED) {
    reconnect();
  }

  if (!tb.connected()) {
    // Connect to the Delta Things IoT server
    Serial.print("Connecting to: ");
    Serial.print(DTIOT_SERVER);
    Serial.print(" with token ");
    Serial.println(TOKEN);
    if (!tb.connect(DTIOT_SERVER, TOKEN,DTIOT_PORT)) {
      Serial.println("Failed to connect");
      return;
    }
    else
    {
      tb.sendAttributeString("Serial-No","DT-ESP-00001");
      tb.sendAttributeString("SW-Ver","1.00");
      tb.sendAttributeString("HW-Type","ESP8266");
    }
  }

  Serial.println("Sending data...");

  tb.sendTelemetryInt("temperature", 22);
  tb.sendTelemetryFloat("humidity", 42.5);

  tb.loop();
}

void InitWiFi()
{
  Serial.println("Connecting to AP ...");
  // attempt to connect to WiFi network

  WiFi.begin(WIFI_AP, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to AP");
}

void reconnect() {
  // Loop until we're reconnected
  status = WiFi.status();
  if ( status != WL_CONNECTED) {
    WiFi.begin(WIFI_AP, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("Connected to AP");
  }
}
