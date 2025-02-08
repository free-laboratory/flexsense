# flexsense
Soft-Embeded Bend Sensor Design for Soft Robots
![r5](https://github.com/user-attachments/assets/aa1341d6-7d2b-42a2-8708-10a04a5eecf6)

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


