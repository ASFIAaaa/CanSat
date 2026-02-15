#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include <SoftwareSerial.h>

Adafruit_BMP280 bmp;

float seaLevelPressure = 1015.0;

SoftwareSerial telemetry(10, 11);

// Function to get averaged temperature
float getAverageTemperature(int samples) {
  float total = 0;
  for (int i = 0; i < samples; i++) {
    total += bmp.readTemperature();
    delay(10);
  }
  return total / samples;
}

// Function to get averaged pressure
float getAveragePressure(int samples) {
  float total = 0;
  for (int i = 0; i < samples; i++) {
    total += bmp.readPressure();
    delay(10);
  }
  return total / samples;
}

void setup() {
  Serial.begin(9600);
  telemetry.begin(9600);

  Serial.println(F("BMP280 Telemetry Test"));

  if (!bmp.begin(0x76)) {
    Serial.println(F("Could not find a valid BMP280 sensor!"));
    while (1);
  }

  bmp.setSampling(
    Adafruit_BMP280::MODE_NORMAL,
    Adafruit_BMP280::SAMPLING_X8,
    Adafruit_BMP280::SAMPLING_X16,
    Adafruit_BMP280::FILTER_X16,
    Adafruit_BMP280::STANDBY_MS_500
  );

  Serial.println(F("BMP280 Initialized"));
}

void loop() {

  float temperature = getAverageTemperature(10);
  float pressure = getAveragePressure(10) / 100.0F;   // Convert Pa to hPa
  float altitude = bmp.readAltitude(seaLevelPressure);

  String data = "Temp: " + String(temperature) + " C, " +
                "Pressure: " + String(pressure) + " hPa, " +
                "Altitude: " + String(altitude) + " m";

  Serial.println(data);
  telemetry.println(data);

  delay(2000);
}
