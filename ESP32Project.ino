#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>

//Sensors
BH1750 lightMeter;
Adafruit_INA219 ina219;

//WiFi
const char* ssid = "STEPHE";
const char* password = "zyqq4921";
WebServer server(80);

// Sensor functions
float getLight() { return lightMeter.readLightLevel(); }
float getBusVoltage() { return ina219.getBusVoltage_V(); }
float getShuntVoltage() { return ina219.getShuntVoltage_mV(); }
float getCurrent() { return ina219.getCurrent_mA(); }
float getPower() { return ina219.getPower_mW(); }
float getTemperature() { return dht.readTemperature(); }
float getHumidity() { return dht.readHumidity(); }
bool dhtOk() { return !isnan(getTemperature()) && !isnan(getHumidity()); }

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
