#include <Wire.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

#define USE_IMU  // Enable IMU functionality

// --- Multiplexer 1 Definitions ---
const int sigPin1 = A0;
const int s0Pin1 = 22;
const int s1Pin1 = 24;
const int s2Pin1 = 26;
const int s3Pin1 = 28;

// --- Multiplexer 2 Definitions ---
const int sigPin2 = A1;
const int s0Pin2 = 46;
const int s1Pin2 = 48;
const int s2Pin2 = 50;
const int s3Pin2 = 52;

// --- Flex sensor data storage arrays ---
int flexData1[13], flexData2[13];

#ifdef USE_IMU
  Adafruit_BNO055 imu1 = Adafruit_BNO055(55, 0x28);
  Adafruit_BNO055 imu2 = Adafruit_BNO055(56, 0x28, &Wire1);
#endif

void setup() {
  Serial.begin(115200);
  
  // Initialize I2C buses
  Wire.begin();
  Wire1.begin();
  
  //Serial.println("Scanning I2C devices on Wire...");
  scanI2C(Wire);

  //Serial.println("Scanning I2C devices on Wire1...");
  scanI2C(Wire1);

  // Configure multiplexer control pins
  pinMode(s0Pin1, OUTPUT);
  pinMode(s1Pin1, OUTPUT);
  pinMode(s2Pin1, OUTPUT);
  pinMode(s3Pin1, OUTPUT);
  
  pinMode(s0Pin2, OUTPUT);
  pinMode(s1Pin2, OUTPUT);
  pinMode(s2Pin2, OUTPUT);
  pinMode(s3Pin2, OUTPUT);

  // Initialize IMUs
  bool imu1_ready = imu1.begin();
  bool imu2_ready = imu2.begin();


}

void loop() {
  unsigned long currentTime = millis();

  // --- Read Multiplexer 1 channels (0 to 12) ---
  for (int i = 0; i < 13; i++) {
    selectChannel(1, i);
    delay(5); // Allow multiplexer stabilization
    flexData1[i] = analogRead(sigPin1);
  }

  // --- Read Multiplexer 2 channels (0 to 12) ---
  int index = 0;
  for (int i = 0; i < 8; i++) {
    selectChannel(2, i);
    delay(5);
    flexData2[index++] = analogRead(sigPin2);
  }
  for (int i = 11; i < 16; i++) {
    selectChannel(2, i);
    delay(5);
    flexData2[index++] = analogRead(sigPin2);
  }

  // --- Read IMU quaternion data ---
  String imu1_quat = "";
  String imu2_quat = "";
  #ifdef USE_IMU
    imu::Quaternion q1 = imu1.getQuat();
    imu::Quaternion q2 = imu2.getQuat();
    imu1_quat = String(q1.w(), 4) + " " + String(q1.x(), 4) + " " + String(q1.y(), 4) + " " + String(q1.z(), 4);
    imu2_quat = String(q2.w(), 4) + " " + String(q2.x(), 4) + " " + String(q2.y(), 4) + " " + String(q2.z(), 4);
  #endif

  // --- Print CSV Data ---
  Serial.print(currentTime);
  for (int i = 0; i < 13; i++) {
    Serial.print(",");
    Serial.print(flexData1[i]);
  }
  for (int i = 0; i < 13; i++) {
    Serial.print(",");
    Serial.print(flexData2[i]);
  }
  Serial.print(",");
  Serial.print(imu1_quat);
  Serial.print(",");
  Serial.print(imu2_quat);
  Serial.println();

  delay(25);
}

// --- Function to set the multiplexer channel ---
void selectChannel(int multiplexer, int channel) {
  if (multiplexer == 1) {
    digitalWrite(s0Pin1, channel & 0x01);
    digitalWrite(s1Pin1, (channel >> 1) & 0x01);
    digitalWrite(s2Pin1, (channel >> 2) & 0x01);
    digitalWrite(s3Pin1, (channel >> 3) & 0x01);
  } else if (multiplexer == 2) {
    digitalWrite(s0Pin2, channel & 0x01);
    digitalWrite(s1Pin2, (channel >> 1) & 0x01);
    digitalWrite(s2Pin2, (channel >> 2) & 0x01);
    digitalWrite(s3Pin2, (channel >> 3) & 0x01);
  }
}

// --- Function to scan I2C devices ---
void scanI2C(TwoWire &wire) {
  for (byte address = 1; address < 127; address++) {
    wire.beginTransmission(address);
    if (wire.endTransmission() == 0) {
      //Serial.print("Found device at 0x");
      //Serial.println(address, HEX);
    }
  }
}