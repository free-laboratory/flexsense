#include <Wire.h>

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

// --- BNO055 IMU Definitions (UART mode) ---
#define USE_IMU
#ifdef USE_IMU
  #define IMU_BAUDRATE 115200
#endif

// Flex sensor data storage arrays
int flexData1[13], flexData2[13];

// Variable to record loop timing (optional)
unsigned long lastTime = 0;

void setup() {
  Serial.begin(115200);   // USB Serial for PC output
  Wire.begin();

  // Configure multiplexer control pins as outputs
  pinMode(s0Pin1, OUTPUT);
  pinMode(s1Pin1, OUTPUT);
  pinMode(s2Pin1, OUTPUT);
  pinMode(s3Pin1, OUTPUT);
  
  pinMode(s0Pin2, OUTPUT);
  pinMode(s1Pin2, OUTPUT);
  pinMode(s2Pin2, OUTPUT);
  pinMode(s3Pin2, OUTPUT);

  // Initialize BNO055 IMUs (UART mode) if enabled
  #ifdef USE_IMU
    Serial1.begin(IMU_BAUDRATE);  // First IMU (TX1/RX1)
    Serial2.begin(IMU_BAUDRATE);  // Second IMU (TX2/RX2)
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
  // First, channels 0 to 7
  int index = 0;
  for (int i = 0; i < 8; i++) {
    selectChannel(2, i);
    flexData2[index++] = analogRead(sigPin2);
  }
  // Then, channels 11 to 15 (to make 13 channels total)
  for (int i = 11; i < 16; i++) {
    selectChannel(2, i);
    flexData2[index++] = analogRead(sigPin2);
  }

  // --- Print CSV Data ---
  // 1. Timestamp
  Serial.print(currentTime);
  
  // 2. Multiplexer 1 data (13 channels)
  for (int i = 0; i < 13; i++) {
    Serial.print(","); 
    Serial.print(flexData1[i]);
  }
  
  // 3. Multiplexer 2 data (13 channels)
  for (int i = 0; i < 13; i++) {
    Serial.print(","); 
    Serial.print(flexData2[i]);
  }
  
  // 4. IMU quaternion data (if enabled)
  #ifdef USE_IMU
    String imu1_quat = readIMUQuaternion(Serial1);
    String imu2_quat = readIMUQuaternion(Serial2);
    Serial.print(","); Serial.print(imu1_quat);
    Serial.print(","); Serial.print(imu2_quat);
  #endif

  Serial.println();  // End of CSV line

  delay(50);  // Short delay for sensor stabilization
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

#ifdef USE_IMU
// --- Function to read quaternion data from a BNO055 IMU via UART ---
// This function reads characters until a newline character is found.
String readIMUQuaternion(HardwareSerial &imuSerial) {
  String quaternionData = "";
  while (imuSerial.available()) {
    char c = imuSerial.read();
    if (c == '\n') break;
    quaternionData += c;
  }
  return quaternionData;
}
#endif
