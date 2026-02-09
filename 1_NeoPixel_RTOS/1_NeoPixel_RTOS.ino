/*
 * Project: RGB NeoPixel Demo v1.0 in RTOS Multitasking
 * Hardware: ESP32-SS (Cucumber RS) with BMP280, SHT4x, MPU6050, NeoPixel
 * Remember, please select an "ESP32S2 Dev Module" in Arduino IDE before uploading your code.
 * 2102541 IOT Fundamentals, Electrical Engineering, Chulalongkorn University
 * Rewrite code by Dr.Natthakorn Kasamsumran (NKR) February 3, 2026
 */

#include <Adafruit_NeoPixel.h>

// --- Configuration ---
#define RGB_PIN       18  // The Cucumber RS LED pinout
#define NUMPIXELS     1   // Number of LEDs

Adafruit_NeoPixel pixels(NUMPIXELS, RGB_PIN, NEO_GRB + NEO_KHZ800);

// Define the Task Function Prototype
void TaskBlinkRGB(void *pvParameters);

void setup() {
  Serial.begin(115200);
  Serial.println("Starting RTOS Multitasking LED Blink...");
  pixels.begin();
  pixels.clear();

  // Create the Task
  xTaskCreate(
    TaskBlinkRGB,    // Function to run
    "Blink RGB",     // Name for debugging
    2048,            // Stack size (bytes)
    NULL,            // Parameters
    1,               // Priority (1 is standard)
    NULL             // Task handle
  );
}

void loop() {
  // void loop() is free. (No code)
}

// Implement the Task Logic
void TaskBlinkRGB(void *pvParameters) {
  // Similar to the void setup(), this is a one-time run.
  int R[] = { 150, 0, 0 };
  int G[] = { 0, 150, 0 };
  int B[] = { 0, 0, 150 };
  
  // Similar to the void loop(), this is a forever loop.
  for (;;) { 
    for (int i = 0; i < 3; i++) {
      pixels.setPixelColor(0, pixels.Color(R[i], G[i], B[i]));
      pixels.show();
      Serial.println("The void loop is not running.\nJust TaskBlinkRGB is running.");
      vTaskDelay(500 / portTICK_PERIOD_MS); 
    }
  }
}