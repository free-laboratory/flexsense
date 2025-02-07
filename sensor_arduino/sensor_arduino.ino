#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <array>

// Define: Multiplexer 1
const int sigPin1 = A0;
const int s0Pin1 = 22;
const int s1Pin1 = 24;
const int s2Pin1 = 26;
const int s3Pin1 = 28;

// Define: Multiplexer 2
const int sigPin2 = A1;
const int s0Pin2 = 46;
const int s1Pin2 = 48;
const int s2Pin2 = 50;
const int s3Pin2 = 52;

// Define BNO055 IMU sensors
Adafruit_BNO055 bno1 = Adafruit_BNO055(55, 0x28);
Adafruit_BNO055 bno2 = Adafruit_BNO055(55, 0x29);

// Flex sensor data storage
int flexData1[13], flexData2[13];

// Variables to measure loop speed
unsigned long lastTime = 0;
float loopSpeedHz = 0.0;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  
  pinMode(s0Pin1, OUTPUT);
  pinMode(s1Pin1, OUTPUT);
  pinMode(s2Pin1, OUTPUT);
  pinMode(s3Pin1, OUTPUT);

  pinMode(s0Pin2, OUTPUT);
  pinMode(s1Pin2, OUTPUT);
  pinMode(s2Pin2, OUTPUT);
  pinMode(s3Pin2, OUTPUT);

  // Initialize IMUs
  initializeIMU(bno1);
  initializeIMU(bno2);

  lastTime = millis();
}

void loop() {
  unsigned long currentTime = millis();

  // Read multiplexer 1 & 2 data
  for (int i = 0; i < 13; i++) {
    selectChannel(1, i);
    flexData1[i] = analogRead(sigPin1);

    selectChannel(2, i);
    flexData2[i] = analogRead(sigPin2);
  }

  // Read BNO055 sensor data
  std::array<float, 3> imu1_data = readYawData(bno1);
  std::array<float, 3> imu2_data = readYawData(bno2);

  // Measure loop speed
  unsigned long loopTime = millis() - lastTime;
  loopSpeedHz = 1000.0 / loopTime;
  lastTime = millis();

  // Print Data
  Serial.print(currentTime);
  for (int i = 0; i < 13; i++) {
    Serial.print(","); Serial.print(flexData1[i]);
  }
  for (int i = 0; i < 13; i++) {
    Serial.print(","); Serial.print(flexData2[i]);
  }

  Serial.print(","); Serial.print(imu1_data[0]);
  Serial.print(","); Serial.print(imu1_data[1]);
  Serial.print(","); Serial.print(imu1_data[2]);

  Serial.print(","); Serial.print(imu2_data[0]);
  Serial.print(","); Serial.print(imu2_data[1]);
  Serial.print(","); Serial.print(imu2_data[2]);

  Serial.println();
  delay(50);  // Stabilize sensor readings
}

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

void initializeIMU(Adafruit_BNO055 &imu) {
  if (!imu.begin()) {
    Serial.println("IMU Initialization Failed!");
    delay(5000);
  }
  imu.setExtCrystalUse(false);
}

std::array<float, 3> readYawData(Adafruit_BNO055 &imu) {
  sensors_event_t event;
  imu.getEvent(&event);

  return {event.orientation.x, event.orientation.y, event.orientation.z};
}
