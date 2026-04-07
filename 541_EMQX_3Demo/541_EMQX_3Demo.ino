/*
 * Project: MQTT EMQX Demo v1.1 in Sequence C Programing
 * Hardware: ESP32-SS (Cucumber RS) with BMP280, SHT4x, MPU6050, NeoPixel
 * Remember, please select an "ESP32S2 Dev Module" in Arduino IDE before uploading your code.
 * 2102541 IOT Fundamentals, Electrical Engineering, Chulalongkorn University
 * Rewrite code by Dr.Natthakorn Kasamsumran (NKR) April 7, 2026
 */

#include <Wire.h>
#include <WiFi.h>
#include <Adafruit_BMP280.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "credentials.h"

const char* mqtt_server = "iotfun.iotcloudserve.net";
const int mqtt_port = 18XXX;

const char* topic_publish = "IoTFun/GroupXXX";

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;

// I2C Pinout configuration for ESP32 Cucumber RS/MIS
#define SDA_PIN 41
#define SCL_PIN 40
#define BMP_ADDRESS 0x76

Adafruit_BMP280 bmp(&Wire);

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = "ESP32_SensorNode_" + String(random(0xffff), HEX);
    if (client.connect(clientId.c_str(), mqtt_user, mqtt_pass)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  Wire.begin(SDA_PIN, SCL_PIN);
  Serial.println(F("BMP280 Sequence C Programing Test..."));
  
  if (!bmp.begin(BMP_ADDRESS)) {
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring!"));
    while (1) delay(10);
  }

  // Optional: Set sampling parameters for higher accuracy
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  unsigned long now = millis();
  if (now - lastMsg > 5000) { // Sent payload every 5000 milliseconds
    lastMsg = now;

    // BMP280 absolute barometric pressure sensor reading value via I2C
    float temp = bmp.readTemperature();
    float pressure = bmp.readPressure();
    float altitude = bmp.readAltitude(1013.25); // Adjusted to standard sea level pressure

    // Create JSON payload size 512 bytes as doc
    StaticJsonDocument<512> doc;

    // Add your gateway name
    doc["Gateway"] = "";

    // Read the actual values ​​from the sensors (temperature,pressure, and altitute) as Node 01.
    JsonObject n1 = doc.createNestedObject("Node_01");
    n1["temp"] = temp;
    n1["pres"] = pressure;
    n1["alti"] = altitude;

    // Simulate the sensor values read (temperature,pressure, and altitute) from Node 02.
    JsonObject n2 = doc.createNestedObject("Node_02");
    n2["temp"] = temp+float(1.5);
    n2["pres"] = pressure;
    n2["alti"] = altitude+float(5.5);;

    // Simulate the sensor values read (temperature,pressure, and altitute) from Node 03.
    JsonObject n3 = doc.createNestedObject("Node_03");
    n3["temp"] = temp-float(1.5);;
    n3["pres"] = pressure;
    n3["alti"] = altitude-float(5.5);;

    // Create buffer payload size 512 bytes
    char buffer[512];
    serializeJson(doc, buffer);

    // Publish payload to MQTT broker
    Serial.print("Sending Data: ");
    Serial.println(buffer);
    client.publish(topic_publish, buffer);
  }
}