#include "HX711.h"

// Define HX711 data and clock pins
// HX711_0
// #define DT 27  // HX711 Data pin (DT)
// #define SCK 14 // HX711 Clock pin (SCK)

// // // HX711_1
// #define DT 26  // HX711 Data pin (DT)
// #define SCK 25 // HX711 Clock pin (SCK)

// // HX711_2
// #define DT 33  // HX711 Data pin (DT)
// #define SCK 32 // HX711 Clock pin (SCK)

// HX711_3
#define DT 18  // HX711 Data pin (DT)
#define SCK 19 // HX711 Clock pin (SCK)

// Create an instance of HX711
HX711 scale;

void setup() {
  Serial.begin(115200);
  scale.begin(DT, SCK);

  Serial.println("HX711 Test Started...");
  if (scale.is_ready()) {
    Serial.println("HX711 is connected and ready.");
  } else {
    Serial.println("HX711 not detected. Check wiring!");
  }
}

void loop() {
  if (scale.is_ready()) {
    long reading = scale.read();  // Read raw ADC value
    Serial.print("Raw ADC Value: ");
    Serial.println(reading);
  } else {
    Serial.println("HX711 not responding...");
  }
  delay(500);
}