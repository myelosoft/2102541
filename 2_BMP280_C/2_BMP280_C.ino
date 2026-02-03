/*
 * Project: BMP280 Sensor Demo v1.0 in Sequence C Programing
 * Hardware: ESP32-S3 (Cucumber RS) with BMP280, SHT4x, MPU6050, NeoPixel
 * 2102541 IOT Fundamentals, Electrical Engineering, Chulalongkorn University
 * Rewrite code by Dr.Natthakorn Kasamsumran (NKR) February 3, 2026
 */

#include <Wire.h>
#include <Adafruit_BMP280.h>

// --- Configuration ---
#define SDA_PIN 41
#define SCL_PIN 40
#define BMP_ADDRESS 0x76

Adafruit_BMP280 bmp(&Wire);

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
}

void loop() {
  float temp = bmp.readTemperature();
  float pressure = bmp.readPressure();
  float altitude = bmp.readAltitude(1013.25); // Adjusted to standard sea level pressure

  Serial.print(F("Temperature = "));
  Serial.print(temp);
  Serial.println(" *C");

  Serial.print(F("Pressure = "));
  Serial.print(pressure);
  Serial.println(" Pa");

  Serial.print(F("Approx altitude = "));
  Serial.print(altitude);
  Serial.println(" m");

  Serial.println();
  delay(2000);
}