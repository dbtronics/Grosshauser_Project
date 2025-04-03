#include <Wire.h>

// Define MPR121 data and clock pins
// MPR121_0
#define SDA 21  // MPR121 Data pin (SDA)
#define SCL 22 // MPR121 Clock pin (SCL)

// MPR121_1
// #define SDA 27  // MPR121 Data pin (SDA)
// #define SCL 14 // MPR121 Clock pin (SCL)

// MPR121_2
// #define SDA 27  // MPR121 Data pin (SDA)
// #define SCL 14 // MPR121 Clock pin (SCL)

// MPR121_3
// #define SDA 27  // MPR121 Data pin (SDA)
// #define SCL 14 // MPR121 Clock pin (SCL)


void setup() {
  Serial.begin(115200);
  Wire.begin(SDA, SCL); // SDA=21, SCL=22 (Default ESP32 I2C Pins)
  
  Serial.println("Scanning I2C Bus...");
}

void loop() {
  Serial.println("Scanning...");
  
  for (byte address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    if (Wire.endTransmission() == 0) {
      Serial.print("I2C device found at address 0x");
      Serial.println(address, HEX);
    }
  }

  Serial.println("Scan complete.\n");
  delay(5000);  // Wait 5 seconds before scanning again
}