#include <Wire.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

Adafruit_BNO055 imu1 = Adafruit_BNO055(55, 0x28);
Adafruit_BNO055 imu2 = Adafruit_BNO055(&Wire1, 56, 0x28);

void setup() {
  Serial.begin(115200);
  Wire.begin();
  Wire1.begin();
  
  Serial.println("Scanning I2C devices on Wire (default I2C bus)...");
  scanI2C(Wire);

  Serial.println("Scanning I2C devices on Wire1 (second I2C bus)...");
  scanI2C(Wire1);

  if (!imu1.begin()) {
    Serial.println("IMU 1 (Wire) initialization failed!");
  } else {
    Serial.println("IMU 1 initialized successfully.");
    imu1.setExtCrystalUse(true);
  }

  if (!imu2.begin()) {
    Serial.println("IMU 2 (Wire1) initialization failed!");
  } else {
    Serial.println("IMU 2 initialized successfully.");
    imu2.setExtCrystalUse(true);
  }
}

void loop() {
  Serial.println("Reading IMU data...");
  
  imu::Quaternion q1 = imu1.getQuat();
  imu::Quaternion q2 = imu2.getQuat();
  
  Serial.print("IMU1: "); Serial.print(q1.w(), 4); Serial.print(" ");
  Serial.print(q1.x(), 4); Serial.print(" "); Serial.print(q1.y(), 4);
  Serial.print(" "); Serial.println(q1.z(), 4);

  Serial.print("IMU2: "); Serial.print(q2.w(), 4); Serial.print(" ");
  Serial.print(q2.x(), 4); Serial.print(" "); Serial.print(q2.y(), 4);
  Serial.print(" "); Serial.println(q2.z(), 4);

  delay(500);
}

void scanI2C(TwoWire &wire) {
  for (byte address = 1; address < 127; address++) {
    wire.beginTransmission(address);
    if (wire.endTransmission() == 0) {
      Serial.print("Found device at 0x");
      Serial.println(address, HEX);
    }
  }
}
