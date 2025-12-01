#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>

const char* ssid = "STEPHE";
const char* password = "zyqq4921";
WebServer server(80);

void setup() {
 Serial.begin(115200);

 //WiFi 
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected! IP: " + WiFi.localIP().toString());
}

if (MDNS.begin("esp32")) Serial.println("MDNS responder started");

void loop() {
  server.handleClient();
}
