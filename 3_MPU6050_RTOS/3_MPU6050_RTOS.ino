/*
 * Project: MPU6050 Sensor Demo v1.0 in RTOS Multitasking
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

// Mutex handles for shared resources
SemaphoreHandle_t I2C_Mutex;
SemaphoreHandle_t Serial_Mutex;

void TaskReadMPU6050(void *pvParameters);

void setup() {
  Serial.begin(115200);
  Wire.begin(SDA_PIN, SCL_PIN);

  // Create Mutexes
  I2C_Mutex = xSemaphoreCreateMutex();
  Serial_Mutex = xSemaphoreCreateMutex();

  // Initialize MPU6050 (Safe to do in setup before tasks start)
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) delay(10);
  }
  
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  Serial.println("Starting RTOS MPU6050 Task...");

  // Create the Task
  xTaskCreate(
    TaskReadMPU6050,    // Function to run
    "ReadMPU",          // Name for debugging
    4096,               // Stack size (bytes) MPU6050 library can be heavy.
    NULL,               // Parameters
    1,                  // Priority (1 is standard)
    NULL                // Task handle
  );
}

void loop() {
  // void loop() is free. (No code)
}

void TaskReadMPU6050(void *pvParameters) {
  sensors_event_t a, g, temp;

  for (;;) {
    // Lock I2C Bus before reading
    if (xSemaphoreTake(I2C_Mutex, portMAX_DELAY) == pdTRUE) {
      mpu.getEvent(&a, &g, &temp);
      xSemaphoreGive(I2C_Mutex); // Release I2C Bus immediately
    }

    // Lock Serial before printing
    if (xSemaphoreTake(Serial_Mutex, portMAX_DELAY) == pdTRUE) {
      Serial.println("--- MPU6050 RTOS Task ---");
      Serial.printf("Acceleration X:%.2f, Y:%.2f, Z:%.2f m/s^2\n", a.acceleration.x, a.acceleration.y, a.acceleration.z);
      Serial.printf("Rotation X:%.2f, Y:%.2f, Z:%.2f rad/s\n", g.gyro.x, g.gyro.y, g.gyro.z);        
      Serial.printf("Temperature: %.2f C\n\n", temp.temperature);
      xSemaphoreGive(Serial_Mutex); // Release Serial
    }
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}