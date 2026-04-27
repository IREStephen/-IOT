// ThingSpeak Channel page https://thingspeak.mathworks.com/channels

#include <Wire.h>           // I2C communication for BH1750 & INA219
#include <BH1750.h>         // Light intensity sensor
#include <Adafruit_INA219.h>// Voltage, current & power sensor
#include <DHT.h>            // DHT11
#include <WiFi.h>           // ESP32 WiFi
#include <WebServer.h>      // Hosts the local dashboard webpage
#include <ESPmDNS.h>        // Allows access via esp32.local
#include "homepage.h"       // HTML dashboard stored separately
#include <HTTPClient.h>     // Sends data to ThingSpeak cloud

// ThingSpeak Config
const char* thingSpeakApiKey = "HO9A8A2L142XPVU0"; // Write API key for our channel
const unsigned long thingSpeakInterval = 20000;     // Upload every 20 seconds
unsigned long lastThingSpeakUpdate = 0;             // Tracks time of last upload

// DHT11
#define DHTPIN 32       // GPIO pin connected to DHT11 data line
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// Sensor Objects
BH1750 lightMeter;       // Light sensor over I2C
Adafruit_INA219 ina219;  // Power monitor over I2C

// WiFi & Web Server
const char* ssid = "STEPHE";
const char* password = "zyqq4921";
WebServer server(80); // Standard HTTP port

// Sensor Read Helpers
// Each function wraps a single sensor reading for clean reuse
float getLight()        { return lightMeter.readLightLevel(); }   // Lux
float getBusVoltage()   { return ina219.getBusVoltage_V(); }      // Volts
float getShuntVoltage() { return ina219.getShuntVoltage_mV(); }   // Millivolts across shunt resistor
float getCurrent()      { return ina219.getCurrent_mA(); }        // Milliamps
float getPower()        { return ina219.getPower_mW(); }          // Milliwatts
float getTemperature()  { return dht.readTemperature(); }         // Celsius
float getHumidity()     { return dht.readHumidity(); }            // Percent RH
bool dhtOk()            { return !isnan(getTemperature()) && !isnan(getHumidity()); } // Validates DHT reading

// ThingSpeak Upload 
// Reads all 7 sensors and sends them as fields in a single HTTP GET request
void sendToThingSpeak() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    // Sensor values
    float light   = getLight();
    float busV    = getBusVoltage();
    float shuntV  = getShuntVoltage();
    float current = getCurrent();
    float power   = getPower();
    float temp    = getTemperature();
    float hum     = getHumidity();

    // Build URL
    String url = "http://api.thingspeak.com/update?api_key=" + String(thingSpeakApiKey);
    url += "&field1=" + String(light);    // Light (lx)
    url += "&field2=" + String(busV);     // Bus Voltage (V)
    url += "&field3=" + String(shuntV);   // Shunt Voltage (mV)
    url += "&field4=" + String(current);  // Current (mA)
    url += "&field5=" + String(power);    // Power (mW)
    url += "&field6=" + String(temp);     // Temperature (°C)
    url += "&field7=" + String(hum);      // Humidity (%)

    Serial.println("Sending to ThingSpeak...");

    http.begin(url);
    int httpCode = http.GET(); // ThingSpeak returns the entry ID on success

    if (httpCode > 0) {
      Serial.println("ThingSpeak update successful! HTTP code: " + String(httpCode));
    } else {
      Serial.println("ThingSpeak update failed! Error: " + String(http.errorToString(httpCode).c_str()));
    }
    http.end();
  }
}

// Dashboard HTML Builder
// Generates sensor cards injected into the webpage
String getSensorCards() {
  String html = "";
  html += "<div class=\"card\"><div>Light</div><div class=\"value\">"         + String(getLight())        + " lx</div></div>";
  html += "<div class=\"card\"><div>Bus Voltage</div><div class=\"value\">"   + String(getBusVoltage())   + " V</div></div>";
  html += "<div class=\"card\"><div>Shunt Voltage</div><div class=\"value\">" + String(getShuntVoltage()) + " mV</div></div>";
  html += "<div class=\"card\"><div>Current</div><div class=\"value\">"       + String(getCurrent())      + " mA</div></div>";
  html += "<div class=\"card\"><div>Power</div><div class=\"value\">"         + String(getPower())        + " mW</div></div>";

  if (dhtOk()) {
    html += "<div class=\"card\"><div>Temperature</div><div class=\"value\">" + String(getTemperature())  + " °C</div></div>";
    html += "<div class=\"card\"><div>Humidity</div><div class=\"value\">"    + String(getHumidity())     + " %</div></div>";
  } else {
    html += "<div class=\"card\"><div>DHT11 ERROR</div></div>"; // Shown if sensor returns NaN
  }
  return html;
}

// HTTP Route Handlers 
void handleRoot() {
  // Combines static HTML with live sensor cards
  String message = homePagePart1 + getSensorCards() + homePagePart2;
  server.send(200, "text/html", message);
}

void handleSensors() {
  // Called by the JS fetch() every 2s — returns only the updated cards (no full page reload)
  String message = getSensorCards();
  server.send(200, "text/html", message);
}

void handleNotFound() {
  server.send(404, "text/plain", "File Not Found");
}

//  etup
void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22); // SDA on GPIO21, SCL on GPIO22

  // Initialise all three sensors
  if (!lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE)) Serial.println("BH1750 error");
  if (!ina219.begin()) Serial.println("INA219 error");
  dht.begin();

  // Connect to WiFi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) { delay(500); Serial.print("."); }
  Serial.println("\nConnected! IP: " + WiFi.localIP().toString());

  if (MDNS.begin("esp32")) Serial.println("MDNS responder started"); // Enables esp32.local

  // Register URL routes
  server.on("/", handleRoot);
  server.on("/sensors", handleSensors); // Lightweight endpoint for JS polling
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");
  Serial.println("ThingSpeak integration ready!");
}

void loop() {
  server.handleClient(); // Handles any incoming web requests

  // ThingSpeak timer — avoids delay() so the server stays responsive
  unsigned long currentMillis = millis();
  if (currentMillis - lastThingSpeakUpdate >= thingSpeakInterval) {
    sendToThingSpeak();
    lastThingSpeakUpdate = currentMillis;
  }
}