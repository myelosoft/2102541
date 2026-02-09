/*
 * Project: BMP280 Sensor Demo v1.0 in RTOS Multitasking
 * Hardware: ESP32-SS (Cucumber RS) with BMP280, SHT4x, MPU6050, NeoPixel
 * Remember, please select an "ESP32S2 Dev Module" in Arduino IDE before uploading your code.
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
SemaphoreHandle_t xSerialMutex; // Lock for Serial port

// Task Function Prototype
void TaskReadBMP280(void *pvParameters);

void setup() {
  Serial.begin(115200);
  Wire.begin(SDA_PIN, SCL_PIN);
  
  // Create Mutex for shared resources (Serial)
  xSerialMutex = xSemaphoreCreateMutex();

  // Initialize Sensor in Setup (Safest place for hardware init)
  if (!bmp.begin(BMP_ADDRESS)) {
    Serial.println(F("Could not find a valid BMP280 sensor!"));
    while (1) delay(10);
  }

  // Set BMP280 sampling settings
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,
                  Adafruit_BMP280::SAMPLING_X2,
                  Adafruit_BMP280::SAMPLING_X16,
                  Adafruit_BMP280::FILTER_X16,
                  Adafruit_BMP280::STANDBY_MS_500);

  Serial.println("Starting RTOS BMP280 Task...");

  // Create the Task
  xTaskCreate(
    TaskReadBMP280,   // Function to run
    "ReadBMP",        // Name for debugging
    2048,             // Stack size (bytes)
    NULL,             // Parameters
    1,                // Priority (1 is standard)
    NULL              // Task handle
  );
}

void loop() {
  // void loop() is free. (No code)
}

// Implement the Task
void TaskReadBMP280(void *pvParameters) {
  for (;;) { 
    
    // Perform Readings
    float temp = bmp.readTemperature();
    float pressure = bmp.readPressure();
    float altitude = bmp.readAltitude(1013.25); // Adjusted to standard sea level pressure

    // Safely Print to Serial (Take the lock)
    if (xSemaphoreTake(xSerialMutex, (TickType_t)100) == pdTRUE) {
      Serial.println(F("--- RTOS Task Output ---"));
      Serial.printf("Temperature = %.2f C\n", temp);
      Serial.printf("Pressure = %.2f Pa\n", pressure);
      Serial.printf("Approx altitude = %.2f m\n\n", altitude);
      
      // Release the lock so other tasks can print
      xSemaphoreGive(xSerialMutex);
    }
    vTaskDelay(2000 / portTICK_PERIOD_MS);
  }
}