import serial
import scipy.io
import numpy as np
import os
import datetime

def create_filename():
    """Generate a unique filename with date and time."""
    now = datetime.datetime.now()
    return f"data_logs/com_data_{now.strftime('%Y%m%d_%H%M%S')}.mat"

def read_com_data(port='COM3', baudrate=115200, duration=100):
    """Read data from the COM port and save as a .mat file."""
    ser = serial.Serial(port, baudrate, timeout=1)
    
    data_list = []
    start_time = datetime.datetime.now()
    print(f"Reading data from {port} for {duration} seconds...")

    # Expected data lengths
    expected_length_no_imu = 27  # 1 timestamp + 26 flex sensor values
    expected_length_with_imu = 35  # 1 timestamp + 26 flex values + 8 quaternion values (2 IMUs * 4)

    while (datetime.datetime.now() - start_time).seconds < duration:
        try:
            line = ser.readline().decode('utf-8').strip()

            if line:
                # Step 1: Ensure commas properly separate the quaternion values
                line = line.replace(" ", ",")  # Convert all spaces to commas
                
                values = list(map(float, line.split(',')))

                # Step 2: Standardize data row length
                if len(values) == expected_length_no_imu:
                    values.extend([np.nan] * 8)  # Pad missing IMU quaternion values
                elif len(values) != expected_length_with_imu:
                    print(f"Warning: Unexpected data length ({len(values)}) - Skipping row")
                    continue  # Skip invalid row

                data_list.append(values)
        except Exception as e:
            print(f"Error reading data: {e}")
            break
    
    ser.close()
    
    if not data_list:
        print("No data received. Exiting.")
        return
    
    # Convert to numpy array (now guaranteed to have uniform shape)
    data_array = np.array(data_list)

    # Save to .mat file
    filename = create_filename()
    os.makedirs("data_logs", exist_ok=True)
    scipy.io.savemat(filename, {"data": data_array})
    
    print(f"Data saved to {filename}")

if __name__ == "__main__":
    read_com_data(port='COM3', baudrate=115200, duration=10)
