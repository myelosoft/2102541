/*
 * Project: MPU6050 Sensor Demo v1.0 in Sequence C Programing
 * Hardware: ESP32-SS (Cucumber RS) with BMP280, SHT4x, MPU6050, NeoPixel
 * Remember, please select an "ESP32S2 Dev Module" in Arduino IDE before uploading your code.
 * 2102541 IOT Fundamentals, Electrical Engineering, Chulalongkorn University
 * Rewrite code by Dr.Natthakorn Kasamsumran (NKR) February 3, 2026
 */

#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

// --- Configuration ---
#define SDA_PIN 41
#define SCL_PIN 40

Adafruit_MPU6050 mpu;

void setup() {
  Serial.begin(115200);
  Wire.begin(SDA_PIN, SCL_PIN);
  Serial.println("MPU6050 Sequence C Programing Test...");

  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

  // Setup sensor range and bandwidth
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
}

void loop() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  Serial.print("Acceleration X: "); Serial.print(a.acceleration.x);
  Serial.print(", Y: "); Serial.print(a.acceleration.y);
  Serial.print(", Z: "); Serial.print(a.acceleration.z);
  Serial.println(" m/s^2");

  Serial.print("Rotation X: "); Serial.print(g.gyro.x);
  Serial.print(", Y: "); Serial.print(g.gyro.y);
  Serial.print(", Z: "); Serial.print(g.gyro.z);
  Serial.println(" rad/s");

  Serial.print("Temperature: "); Serial.print(temp.temperature);
  Serial.println(" degC");

  Serial.println("");
  delay(500);
}