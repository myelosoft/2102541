/*
 * Project: HTS221 or SHT4x Sensor Demo v1.0 in Sequence C Programing
 * Hardware: ESP32-S3 (Cucumber RS) with BMP280, SHT4x, MPU6050, NeoPixel
 * 2102541 IOT Fundamentals, Electrical Engineering, Chulalongkorn University
 * Rewrite code by Dr.Natthakorn Kasamsumran (NKR) February 3, 2026
 */

#include <Wire.h>
#include <Adafruit_HTS221.h>
#include <Adafruit_SHT4x.h>

// --- Configuration ---
#define SDA_PIN 41
#define SCL_PIN 40

// Create objects for both potential sensors
Adafruit_HTS221 hts;
Adafruit_SHT4x sht4;

// Enum to track which sensor is active
enum SensorType { NONE, SENSOR_HTS221, SENSOR_SHT4X };
SensorType activeSensor = NONE;

void setup() {
  Serial.begin(115200);
  Wire.begin(SDA_PIN, SCL_PIN);

  Serial.println("Temperature & Humidity Sensor Test");
  Serial.println("Checking for sensors...");

  // Try to find HTS221
  if (hts.begin_I2C()) {
    activeSensor = SENSOR_HTS221;
    Serial.println("Success: Found HTS221!");
    hts.setDataRate(HTS221_RATE_1_HZ);
  } 
  // If not found, try to find SHT4x
  else if (sht4.begin()) {
    activeSensor = SENSOR_SHT4X;
    Serial.println("Success: Found SHT4x!");
    sht4.setPrecision(SHT4X_HIGH_PRECISION);
    sht4.setHeater(SHT4X_NO_HEATER);
  } 
  else {
    Serial.println("Error: No valid humidity sensor found (Check wiring!)");
    while (1) delay(10); // Halt
  }
}

void loop() {
  sensors_event_t humidity, temp;
  if (activeSensor == SENSOR_HTS221) {
    hts.getEvent(&humidity, &temp);
  } else if (activeSensor == SENSOR_SHT4X) {
    sht4.getEvent(&humidity, &temp);
  }

  Serial.print("Temperature: "); 
  Serial.print(temp.temperature); 
  Serial.println(" degrees C");
  
  Serial.print("Humidity: "); 
  Serial.print(humidity.relative_humidity); 
  Serial.println("% rH");
  
  Serial.println("-----------------------");
  delay(2000);
}