
import tkinter as tk
import json
from urllib import request, error

# Function to fetch weather data and update the GUI
def fetch_weather():
    city = city_entry.get()
    api_key = 'bfdc0033a0d5849afccd6732442d1ba8'  # Your OpenWeatherMap API key here
    weather_url = f'http://api.openweathermap.org/data/2.5/weather?q={city}&appid={api_key}&units=metric'

    try:
        with request.urlopen(weather_url) as response:
            data = response.read().decode('utf-8')
            weather_data = json.loads(data)

            if weather_data['cod'] == 200:
                description = weather_data['weather'][0]['description']
                temperature = weather_data['main']['temp']
                humidity = weather_data['main']['humidity']
                wind_speed = weather_data['wind']['speed']

                weather_label.config(text=f"Description: {description}\nTemperature: {temperature}°C\nHumidity: {humidity}%\nWind Speed: {wind_speed} m/s")
            else:
                weather_label.config(text="City not found")
    except error.URLError as e:
        weather_label.config(text=f"Error: {str(e)}")
    except Exception as e:
        weather_label.config(text=f"Error: {str(e)}")

# Create the main application window
root = tk.Tk()
root.title("Battery Status and Weather Information")

# Create a frame for battery status and weather
status_frame = tk.Frame(root)
status_frame.pack(padx=10, pady=10)

# Create a label and entry for city
city_label = tk.Label(status_frame, text="Enter City:")
city_label.grid(row=0, column=0)
city_entry = tk.Entry(status_frame, width=30)
city_entry.grid(row=0, column=1)
fetch_weather_button = tk.Button(status_frame, text="Fetch Weather", command=fetch_weather)
fetch_weather_button.grid(row=0, column=2)
weather_label = tk.Label(status_frame, text="")
weather_label.grid(row=1, column=0, columnspan=3)

# Battery Status (Replace with your battery status data)
battery_status_data = [
    (11, 3.6, 100, 50, 500, 25, 250, 25, 1, 12),
    (12, 3.7, 150, 60, 600, 30, 300, 25, 3 ,19),
    # Add more data as needed
]

# Create labels for battery status
battery_status_label = tk.Label(status_frame, text="Battery Status:")
battery_status_label.grid(row=2, column=0, columnspan=3)

# Function to display battery status
def display_battery_status():
    battery_status_text = ""
    for row in battery_status_data:
        time_s, Ecell_V, I_mA, EnergyCharge_W_h, QCharge_mA_h, EnergyDischarge_W_h, QDischarge_mA_h, Temperature__C, cycleNumber, Ns = row
        battery_status_text += f"Time: {time_s}, Cell Voltage: {Ecell_V}, Current: {I_mA}mA, Energy Charge: {EnergyCharge_W_h}Wh, Energy Remaining: {EnergyDischarge_W_h}Wh\n"
    battery_status_label.config(text=battery_status_text)

# Display battery status
display_battery_status()

# Start the Tkinter event loop
root.mainloop()
