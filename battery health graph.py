import matplotlib.pyplot as plt
import random
import time
import tkinter as tk

# Initialize variables
I_mA = 0
Temperature__C = 25  # Starting temperature at 25°C
QDischarge_mA_h = 100  # Starting state of charge at 100 mA·h

CURRENT_THRESHOLD = 100  # Current threshold value in mA
TEMPERATURE_THRESHOLD = 40  # Temperature threshold value in °C
STATE_OF_CHARGE_THRESHOLD = 50  # State of charge threshold value in mA·h

# Lists to store historical data for plotting
time_data = []
I_mA_data = []
Temperature__C_data = []
QDischarge_mA_h_data = []

# Function to monitor battery parameters and trigger cut-off if necessary
def monitor_battery():
    global I_mA, Temperature__C, QDischarge_mA_h
    
    # Simulate battery parameter changes
    I_mA = random.randint(50, 150)  # Random current between 50 mA and 150 mA
    Temperature__C += random.uniform(-1, 1)  # Random temperature change between -1°C and 1°C
    QDischarge_mA_h -= random.randint(1, 5)  # Random discharge between 1 mA·h and 5 mA·h
    
    # Store data for plotting
    current_time = time.time()
    time_data.append(current_time)
    I_mA_data.append(I_mA)
    Temperature__C_data.append(Temperature__C)
    QDischarge_mA_h_data.append(QDischarge_mA_h)
    
    # Check if any parameter exceeds its threshold
    if I_mA > CURRENT_THRESHOLD or Temperature__C > TEMPERATURE_THRESHOLD or QDischarge_mA_h < STATE_OF_CHARGE_THRESHOLD:
        print("Auto cut-off triggered due to threshold breach")
        # Trigger cut-off mechanism here
        # For example:
        # stop_charging()
        # disconnect_load()

    # Plot data
    plot_data()
    
    # Schedule the next monitoring iteration
    root.after(5000, monitor_battery)  # Adjust the time interval as needed (in milliseconds)

# Function to plot battery parameters over time
def plot_data():
    plt.figure(figsize=(12, 6))
    
    plt.subplot(3, 1, 1)
    plt.plot(time_data, I_mA_data, '-o', color='b')
    plt.axhline(y=CURRENT_THRESHOLD, color='r', linestyle='--', label='Current Threshold')
    plt.title('Battery Current (mA)')
    plt.xlabel('Time')
    plt.ylabel('Current (mA)')
    plt.legend()
    
    plt.subplot(3, 1, 2)
    plt.plot(time_data, Temperature__C_data, '-o', color='g')
    plt.axhline(y=TEMPERATURE_THRESHOLD, color='r', linestyle='--', label='Temperature Threshold')
    plt.title('Battery Temperature (°C)')
    plt.xlabel('Time')
    plt.ylabel('Temperature (°C)')
    plt.legend()
    
    plt.subplot(3, 1, 3)
    plt.plot(time_data, QDischarge_mA_h_data, '-o', color='m')
    plt.axhline(y=STATE_OF_CHARGE_THRESHOLD, color='r', linestyle='--', label='State of Charge Threshold')
    plt.title('Battery State of Charge (mA·h)')
    plt.xlabel('Time')
    plt.ylabel('State of Charge (mA·h)')
    plt.legend()
    
    plt.tight_layout()
    plt.show()

# Initialize Tkinter root window
root = tk.Tk()
root.title('Battery Monitoring System')

# Start monitoring battery parameters
monitor_battery()

# Run Tkinter event loop
root.mainloop()
