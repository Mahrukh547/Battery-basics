# Battery Management System (BMS) with Auto Cutoff for Cell Voltage

class BatteryManagementSystem:
    def __init__(self, cell_voltage_threshold=3.2):
        self.cell_voltage_threshold = cell_voltage_threshold
        self.is_charging = False

    def check_cell_voltage(self, cell_voltage):
        if cell_voltage < self.cell_voltage_threshold:
            self.is_charging = False
            print(f"Cell voltage ({cell_voltage} V) is is below threshold. Charging allowed.")
        else:
            self.is_charging = True
            print(f"Cell voltage ({cell_voltage} V) above charging limit. Auto cutoff activated.")

    def charge_battery(self):
        if self.is_charging:
            print("Charging battery...")
        else:
            print("Battery voltage is too high. Charging not allowed.")

# Example usage
if __name__ == "__main__":
    bms = BatteryManagementSystem(cell_voltage_threshold=3.0)
    bms.check_cell_voltage(3.0)  # Cell voltage below threshold, charging allowed  
    bms.charge_battery()
    bms.check_cell_voltage(3.7)  # Cell voltage achieved charging limit, auto cutoff activated 
    bms.charge_battery()
