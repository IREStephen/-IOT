#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>

// DHT11 settings
#define DHTPIN 32
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

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

// Setup
void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22); // SDA, SCL

  // Initialize sensors
  if (!lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE)) Serial.println("BH1750 error");
  if (!ina219.begin()) Serial.println("INA219 error");
  dht.begin();

//WiFi 
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected! IP: " + WiFi.localIP().toString());

  if (MDNS.begin("esp32")) Serial.println("MDNS responder started");

  // Server routes
  server.on("/", handleRoot);
  server.on("/sensors", handleSensors);
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");
}

// The Loop
void loop() {
  server.handleClient();
}
