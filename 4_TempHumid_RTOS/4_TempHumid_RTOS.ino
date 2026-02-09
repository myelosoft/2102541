/*
 * Project: HTS221 or SHT4x Sensor Demo v1.0 in RTOS Multitasking
 * Hardware: ESP32-SS (Cucumber RS) with BMP280, SHT4x, MPU6050, NeoPixel
 * Remember, please select an "ESP32S2 Dev Module" in Arduino IDE before uploading your code.
 * 2102541 IOT Fundamentals, Electrical Engineering, Chulalongkorn University
 * Rewrite code by Dr.Natthakorn Kasamsumran (NKR) February 3, 2026
 */

#include <Wire.h>
#include <Adafruit_HTS221.h>
#include <Adafruit_SHT4x.h>

// --- Configuration ---
#define SDA_PIN 41
#define SCL_PIN 40

// Mutex for Thread Safety
SemaphoreHandle_t I2C_Mutex;
SemaphoreHandle_t Serial_Mutex;

// Task Prototype
void TaskReadTempHumid(void *pvParameters);

void setup() {
  Serial.begin(115200);
  Wire.begin(SDA_PIN, SCL_PIN);

  // Create Mutexes
  I2C_Mutex = xSemaphoreCreateMutex();
  Serial_Mutex = xSemaphoreCreateMutex();

  Serial.println("Starting Temperature & Humidity RTOS Task...");

  xTaskCreate(
    TaskReadTempHumid,  // Function to run
    "ReadTempHum",      // Name for debugging
    2048,               // Stack size (bytes)
    NULL,               // Parameters
    1,                  // Priority (1 is standard)
    NULL                // Task handle
  );
}

void loop() {
  // void loop() is free. (No code)
}

void TaskReadTempHumid(void *pvParameters) {
  Adafruit_HTS221 hts;
  Adafruit_SHT4x sht4;
  
  // Local enum for this task
  enum SensorType { NONE, SENSOR_HTS221, SENSOR_SHT4X };
  SensorType activeSensor = NONE;

  // Detection Phase (Protected by Mutex) ---
  if (xSemaphoreTake(I2C_Mutex, portMAX_DELAY) == pdTRUE) {
    
    // Try HTS221 first
    if (hts.begin_I2C()) {
      activeSensor = SENSOR_HTS221;
      hts.setDataRate(HTS221_RATE_1_HZ);
    } 
    // Try SHT4x second
    else if (sht4.begin()) {
      activeSensor = SENSOR_SHT4X;
      sht4.setPrecision(SHT4X_HIGH_PRECISION);
      sht4.setHeater(SHT4X_NO_HEATER);
    }
    
    xSemaphoreGive(I2C_Mutex); // Release I2C
  }

  // Report status to Serial
  if (xSemaphoreTake(Serial_Mutex, portMAX_DELAY) == pdTRUE) {
    if (activeSensor == SENSOR_HTS221) Serial.println("Task: Using HTS221");
    else if (activeSensor == SENSOR_SHT4X) Serial.println("Task: Using SHT4x");
    else Serial.println("Task: No Sensor Found!");
    xSemaphoreGive(Serial_Mutex);
  }

  // If no sensor, kill this specific task to save memory
  if (activeSensor == NONE) {
    vTaskDelete(NULL);
  }

  // Loop Phase ---
  sensors_event_t humidity, temp;
  
  for (;;) {
    // Lock I2C to read data
    if (xSemaphoreTake(I2C_Mutex, portMAX_DELAY) == pdTRUE) {
      if (activeSensor == SENSOR_HTS221) {
        hts.getEvent(&humidity, &temp);
      } else {
        sht4.getEvent(&humidity, &temp);
      }
      xSemaphoreGive(I2C_Mutex);
    }

    // Lock Serial to print data
    if (xSemaphoreTake(Serial_Mutex, portMAX_DELAY) == pdTRUE) {
      Serial.print("[Humidity Task] ");
      if (activeSensor == SENSOR_HTS221) Serial.print("HTS221 -> ");
      else Serial.print("SHT4x -> ");
      
      Serial.print("Temperature: "); Serial.print(temp.temperature);
      Serial.print(" degrees C, Humidity: "); Serial.print(humidity.relative_humidity);
      Serial.println("% rH");
      xSemaphoreGive(Serial_Mutex);
    }
    vTaskDelay(2000 / portTICK_PERIOD_MS);
  }
}