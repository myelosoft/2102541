/*
 * Project: RGB NeoPixel Demo v1.0 in Sequence C Programing
 * Hardware: ESP32-S3 (Cucumber RS) with BMP280, SHT4x, MPU6050, NeoPixel
 * 2102541 IOT Fundamentals, Electrical Engineering, Chulalongkorn University
 * Rewrite code by Dr.Natthakorn Kasamsumran (NKR) February 3, 2026
 */

#include <Adafruit_NeoPixel.h>

// --- Configuration ---
#define RGB_PIN       18    // The Cucumber RS LED pinout
#define NUMPIXELS     1     // Number of LEDs
#define DELAY_MS      500   // Speed of blink in milisecond

Adafruit_NeoPixel pixels(NUMPIXELS, RGB_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(115200);
  pixels.begin();
  pixels.clear();
  Serial.println("Starting Sequence C Programing LED Blink...");
}

void loop() {
  // Red
  Serial.println("Color: RED");
  pixels.setPixelColor(0, pixels.Color(150, 0, 0));
  pixels.show();
  delay(DELAY_MS); // The CPU is "stuck" here for 500ms

  // Green
  Serial.println("Color: GREEN");
  pixels.setPixelColor(0, pixels.Color(0, 150, 0));
  pixels.show();
  delay(DELAY_MS);

  // Blue
  Serial.println("Color: BLUE");
  pixels.setPixelColor(0, pixels.Color(0, 0, 150));
  pixels.show();
  delay(DELAY_MS);
}