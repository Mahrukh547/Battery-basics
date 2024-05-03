import tkinter as tk
import json
from urllib import request, error
from paho.mqtt import client as mqtt_client
import sys

# MQTT Configuration
broker = 'mqtt.netpie.io'
port = 1883
python_topic = "@msg/to_me"
client_id = '4a6ba9c8-3ade-4e0b-a1ad-2f1734bf7268'
username = 'pXuGFeEKVZaNMgXT1t38LtyrS2V1YoGG'
password = 'xiv2b3xeAaG5MuYVrjcBh4SsxKPgy11T'

# Weather API Configuration
api_key = 'bfdc0033a0d5849afccd6732442d1ba8'

# Define client in the global scope
client = mqtt_client.Client(client_id)

def connect_mqtt():
    def on_connect(client, userdata, flags, rc):
        if rc == 0:
            print("Connected to MQTT Broker!")
        else:
            print(f"Failed to connect, return code {rc}")

    client.username_pw_set(username, password)
    client.on_connect = on_connect
    client.connect(broker, port)

def publish(client, topic, message):
    result = client.publish(topic, message)
    status = result[0]
    if status == 0:
        print(f"Send {message} to topic {topic}")
    else:
        print(f"Failed to send message to topic {topic}")

def fetch_weather(city, client, battery_status_text):
    weather_url = f'http://api.openweathermap.org/data/2.5/weather?q={city}&appid={api_key}&units=metric'

    try:
        with request.urlopen(weather_url) as response:
            data = response.read().decode('utf-8')
            weather_data = json.loads(data)

            if  weather_data['cod'] == 200:
                # Extract weather information
                weather_info = {
                    'description': weather_data['weather'][0]['description'],
                    'temperature': weather_data['main']['temp'],
                    'humidity': weather_data['main']['humidity'],
                    'wind_speed': weather_data['wind']['speed']
                }
                battery_status_data = [
                    (11, 3.6, 100, 50, 500, 25, 250, 25, 1, 12),
                    (12, 3.7, 150, 60, 600, 30, 300, 25, 3 ,19),
                    # Add more data as needed
                ]

                battery_status_text = ""
                for row in battery_status_data:
                    time_s, Ecell_V, I_mA, EnergyCharge_W_h, QCharge_mA_h, EnergyDischarge_W_h, QDischarge_mA_h, Temperature__C, cycleNumber, Ns = row
                    battery_status_text += f"Time: {time_s}, Cell Voltage: {Ecell_V}, Current: {I_mA}mA, Energy Charge: {EnergyCharge_W_h}Wh, Energy Remaining: {EnergyDischarge_W_h}Wh\n"
                # Create a JSON message containing weather info and battery status text
                message_data = {
    
                    'battery_status_text': battery_status_text,
                    #'weather_info': weather_info
                }
                message = json.dumps(message_data)
                print("Message size:", len(message))
                # Calculate the memory size of the string
                memory_size = sys.getsizeof(message)

                # Print the memory size of the message
                print("Memory size of message:", memory_size, "bytes")
                # Publish the combined message to the Python topic
                publish(client, python_topic, Ecell_V)
                publish(client,"@msg/to_maha", message)
                return True
            else:
                print("City not found")
    except error.URLError as e:
        print(f"Error: {str(e)}")
    except Exception as e:
        print(f"Error: {str(e)}")
    return False

def display_battery_status():
    battery_status_data = [
        (11, 3.6, 100, 50, 500, 25, 250, 25, 1, 12),
        (12, 3.7, 150, 60, 600, 30, 300, 25, 3 ,19),
        # Add more data as needed
    ]

    battery_status_text = ""
    for row in battery_status_data:
        time_s, Ecell_V, I_mA, EnergyCharge_W_h, QCharge_mA_h, EnergyDischarge_W_h, QDischarge_mA_h, Temperature__C, cycleNumber, Ns = row
        battery_status_text += f"Time: {time_s}, Cell Voltage: {Ecell_V}, Current: {I_mA}mA, Energy Charge: {EnergyCharge_W_h}Wh, Energy Remaining: {EnergyDischarge_W_h}Wh\n"
    


def fetch_weather_and_display(city_entry):
    city = city_entry.get()
    battery_status_text = display_battery_status()
    if fetch_weather(city, client, battery_status_text):
        # Clear previous weather info
        weather_label.config(text="")
    else:
        # Display error message
        weather_label.config(text="City not found")

def fetch_weather_button_clicked():
    fetch_weather_and_display(city_entry)

# GUI Configuration
root = tk.Tk()
root.title("Battery Status and Weather Information")

status_frame = tk.Frame(root)
status_frame.pack(padx=10, pady=10)

city_label = tk.Label(status_frame, text="Enter City:")
city_label.grid(row=0, column=0)
city_entry = tk.Entry(status_frame, width=30)
city_entry.grid(row=0, column=1)
fetch_weather_button = tk.Button(status_frame, text="Fetch Weather", command=fetch_weather_button_clicked)
fetch_weather_button.grid(row=0, column=2)
weather_label = tk.Label(status_frame, text="")
weather_label.grid(row=1, column=0, columnspan=3)

# Connect to MQTT Broker
connect_mqtt()

# Start the Tkinter event loop
root.mainloop()
