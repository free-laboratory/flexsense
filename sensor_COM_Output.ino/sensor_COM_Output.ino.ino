#include <Wire.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

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

// --- BNO055 IMU Definitions (I2C mode) ---
// Using the same address (0x28) for both sensors because they are on independent I2C buses.
// Sensor 1 is connected to the default I2C bus (Wire)
// Sensor 2 is connected to the second I2C bus (Wire1)
#ifdef USE_IMU
  Adafruit_BNO055 imu1 = Adafruit_BNO055(55, 0x28);
  Adafruit_BNO055 imu2 = Adafruit_BNO055(56, 0x28);
#endif

// Variable to record loop timing (optional)
unsigned long lastTime = 0;

void setup() {
  Serial.begin(115200);   // USB Serial for PC output
  
  // Initialize default I2C bus
  Wire.begin();
  // Initialize second I2C bus on Arduino Due
  Wire1.begin();

  // Configure multiplexer control pins as outputs
  pinMode(s0Pin1, OUTPUT);
  pinMode(s1Pin1, OUTPUT);
  pinMode(s2Pin1, OUTPUT);
  pinMode(s3Pin1, OUTPUT);
  
  pinMode(s0Pin2, OUTPUT);
  pinMode(s1Pin2, OUTPUT);
  pinMode(s2Pin2, OUTPUT);
  pinMode(s3Pin2, OUTPUT);

  // Initialize BNO055 IMUs (I2C mode)
  #ifdef USE_IMU
    // Initialize sensor 1 on Wire
    if (!imu1.begin()) {
      Serial.println("Failed to initialize IMU 1 (Wire)!");
      while (1);
    }
    imu1.setExtCrystalUse(true);
    
    // Initialize sensor 2 on Wire1
    // Temporarily switch Wire pointer for imu2 initialization:
    // The Adafruit_BNO055 library does not support selecting the I2C bus directly,
    // so you may need to modify the library or use a workaround.
    // Here we assume that imu2.begin() uses Wire1 (e.g., if the library is modified).
    if (!imu2.begin()) {
      Serial.println("Failed to initialize IMU 2 (Wire1)!");
      while (1);
    }
    imu2.setExtCrystalUse(true);
  #endif

  lastTime = millis();
}

void loop() {
  unsigned long currentTime = millis();

  // --- Read Multiplexer 1 channels (0 to 12) ---
  for (int i = 0; i < 13; i++) {
    selectChannel(1, i);
    flexData1[i] = analogRead(sigPin1);
  }

  // --- Read Multiplexer 2 channels ---
  int index = 0;
  for (int i = 0; i < 8; i++) {
    selectChannel(2, i);
    flexData2[index++] = analogRead(sigPin2);
  }
  for (int i = 11; i < 16; i++) {
    selectChannel(2, i);
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
  Serial.print(","); Serial.print(imu1_quat);
  Serial.print(","); Serial.print(imu2_quat);
  Serial.println();

  delay(50);
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
