#include <Wire.h>

void setup() {
  Serial.begin(115200);
  Wire.begin();
  Wire1.begin();
  
  Serial.println("Scanning I2C devices on Wire (default I2C bus)...");
  scanI2C(Wire);
  
  Serial.println("Scanning I2C devices on Wire1 (second I2C bus)...");
  scanI2C(Wire1);
}

void loop() {}

void scanI2C(TwoWire &wire) {
  for (byte address = 1; address < 127; address++) {
    wire.beginTransmission(address);
    if (wire.endTransmission() == 0) {
      Serial.print("Found device at 0x");
      Serial.println(address, HEX);
    }
  }
}
