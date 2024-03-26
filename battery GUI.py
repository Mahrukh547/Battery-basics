import tkinter as tk
from tkinter import ttk
import logging

logging.basicConfig(level=logging.INFO)

class BatteryManagementSystem:
    def __init__(self, cell_voltage_threshold=2.8):
        self._cell_voltage_threshold = cell_voltage_threshold
        self._is_charging = False

    @property
    def cell_voltage_threshold(self):
        return self._cell_voltage_threshold

    @property
    def is_charging(self):
        return self._is_charging

    def check_cell_voltage(self, cell_voltage):
        if not isinstance(cell_voltage, (int, float)) or cell_voltage <= 0:
            logging.error("Invalid cell voltage value.")
            return
        
        if cell_voltage < self._cell_voltage_threshold:
            self._is_charging = False
            logging.info(f"Cell voltage ({cell_voltage} V) is below threshold. Auto cutoff activated.")
        elif cell_voltage >= 3.7:
            self._is_charging = False
            logging.info(f"Cell voltage ({cell_voltage} V) achieved charging limit. Auto cutoff activated.")
        else:
            self._is_charging = True
            logging.info(f"Cell voltage ({cell_voltage} V) is above threshold. Charging allowed.")

class BatteryManagementSystemGUI:
    def __init__(self, root):
        self.root = root
        self.root.title("Battery Management System")

        self.bms = BatteryManagementSystem()

        self.create_widgets()

    def create_widgets(self):
        # Label and Entry for cell voltage
        ttk.Label(self.root, text="Enter Cell Voltage (V):").grid(row=0, column=0, padx=10, pady=10)
        self.cell_voltage_entry = ttk.Entry(self.root)
        self.cell_voltage_entry.grid(row=0, column=1, padx=10, pady=10)

        # Button to check cell voltage
        self.check_voltage_button = ttk.Button(self.root, text="Check Voltage", command=self.check_voltage)
        self.check_voltage_button.grid(row=0, column=2, padx=10, pady=10)

        # Label to display charging status
        self.charging_status_label = ttk.Label(self.root, text="Charging Status: N/A")
        self.charging_status_label.grid(row=1, columnspan=3, padx=10, pady=10)

    def check_voltage(self):
        try:
            cell_voltage = float(self.cell_voltage_entry.get())
            self.bms.check_cell_voltage(cell_voltage)
            if self.bms.is_charging:
                self.charging_status_label.config(text="Charging Status: Charging Allowed")
            else:
                self.charging_status_label.config(text="Charging Status: Auto Cutoff Activated")
        except ValueError:
            logging.error("Invalid cell voltage value entered.")

if __name__ == "__main__":
    root = tk.Tk()
    app = BatteryManagementSystemGUI(root)
    root.mainloop()
