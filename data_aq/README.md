# Save COM Port Data to .mat for MATLAB (main.py)

## Setup and Requirements
This script reads **COM port data** from an **Arduino** and saves it as a `.mat` file for MATLAB processing.

```sh
# set the duration first
duration=10 
```

### **Requirements:**
- **Python 3.11**
- Required libraries: Install using:
  ```sh
  pip install pyserial scipy numpy
  ```
- uninstall if needed 
  ```sh
  pip uninstall pyserial scipy numpy
  ```
- **Directory Structure:**
  ```
  /data_logs  # Where .mat files will be saved
  ├── save_com_data_matlab.py  # This script
  ```


## **Usage**
1. Connect your **Arduino** and ensure it's transmitting **comma-separated sensor data**.
2. Run the script:
   ```sh
   python save_com_data_matlab.py
   ```
3. The script will create a **timestamped .mat file** in `data_logs/`.
4. Open the `.mat` file in **MATLAB** using:
   ```matlab
   load('com_data_YYYYMMDD_HHMMSS.mat')
   ```

This allows easy **data logging and analysis** in MATLAB! 🚀

## .mat file will look like ...  
 ```sh
1699893145123, 512, 523, 498, 505, ..., 600, [30.12 ... (only if IMU data exsists otherwise, NaN)]
 ```
Format: timestamp, {flex sensor's anaog values, n = 26}, {IMU x y z values, n = 6} 
