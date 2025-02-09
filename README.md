# flexsense
### Soft-Embeded Bend Sensor Design for Soft Robots

![Group 10](https://github.com/user-attachments/assets/ac94b2d1-4139-417a-9797-de6d28e05a24)

## sensor_arduino.ino
directory: flexsense/sensor_arduino/sensor_arduino.ino
### Multiplexer and Sensor Data Acquisition

#### Purpose
This code is used to read data from **flex sensors** via two multiplexers and later integrate **BNO055 IMU sensors**. The primary purpose is to **check signal integrity, sensor locations, and addresses** before further implementation.

#### Hardware Components
- **Multiplexer 1 (Reads from A0)**: Channels **CH0 to CH12** are scanned.
- **Multiplexer 2 (Reads from A1)**: Channels **CH0 to CH7 and CH11 to CH15** are scanned.
- **Flex Sensors**: Connected through multiplexers to analog inputs.
- **IMU Sensors (BNO055)**: Currently commented out but will be used later.

#### Pin Configuration
##### **Multiplexer 1 (Connected to A0)**
- Signal Pin: `A0`
- Select Pins: `S0 (22)`, `S1 (24)`, `S2 (26)`, `S3 (28)`

##### **Multiplexer 2 (Connected to A1)**
- Signal Pin: `A1`
- Select Pins: `S0 (46)`, `S1 (48)`, `S2 (50)`, `S3 (52)`
- Reads **CH0-CH7 and CH11-CH15** only

#### Functionality
- Reads **13 channels** per multiplexer.
- Prints sensor data to the Serial Monitor for verification.
- Uses `selectChannel(multiplexer, channel)` to cycle through sensor inputs.
- Ensures **signal accuracy and correct sensor placement**.



## sensor_COM_Output.ino
directory: flexsense/sensor_COM_Output/sensor_COM_Output.ino
### Multiplexer and Sensor Data Acquisition - Data Sequence Overview

#### Purpose
This script reads **flex sensor data** from two multiplexers and outputs the readings in a **comma-separated format (CSV) over a COM port** for easy data acquisition and processing. The **IMU sensor integration is optional** and can be toggled on or off.

#### Data Sequence Format
Each line of output follows this format:
```
{timestamp}, {v1_1}, {v1_2}, {v1_3}, {v1_4}, {v1_5}, {v1_6}, {v1_7}, {v1_8}, {v1_9}, {v1_10}, {v1_11}, {v1_12}, {v1_13}, {v2_1}, {v2_2}, {v2_3}, {v2_4}, {v2_5}, {v2_6}, {v2_7}, {v2_8}, {v2_9}, {v2_10}, {v2_11}, {v2_12}, {v2_13}, {imu_1.x}, {imu_1.y}, {imu_1.z}, {imu_2.x}, {imu_2.y}, {imu_2.z}
```

#### Data Acquisition Order
1. **Timestamp** - Current system time in milliseconds.
2. **Multiplexer 1 Readings (A0, CH0-CH12)** - 13 sensor values from the first multiplexer.
3. **Multiplexer 2 Readings (A1, CH0-CH7, CH11-CH15)** - 13 sensor values from the second multiplexer (excluding CH8-CH10).
4. **IMU Sensor Data (Optional)** - If enabled, adds `imu_1.x, imu_1.y, imu_1.z, imu_2.x, imu_2.y, imu_2.z` at the end.

#### Notes
- IMU readings are **commented out by default** and can be enabled by defining `USE_IMU`.
- The data is structured for **easy parsing and analysis** in applications like Python, MATLAB, or Excel.
- The script ensures correct **sensor-to-data mapping**, making it easy to track individual sensor values.

This ensures a **consistent, structured data stream** for analysis and real-time applications.


