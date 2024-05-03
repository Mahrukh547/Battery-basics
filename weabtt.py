import paho.mqtt.client as mqtt

# MQTT broker credentials
broker_address = "mqtt.netpie.io"
port = 1883
username = "pXuGFeEKVZaNMgXT1t38LtyrS2V1YoGG"
password = "pXuGFeEKVZaNMgXT1t38LtyrS2V1YoGG"
client_id = "4a6ba9c8-3ade-4e0b-a1ad-2f1734bf7268"

# Callback when connecting to the MQTT broker
def on_connect(client, userdata, flags, rc):
    print(f"Connected with result code {rc}")

# Create an MQTT client instance with a client ID
client = mqtt.Client(client_id)

# Set authentication credentials
client.username_pw_set(username, password)

# Set the callback function for connection
client.on_connect = on_connect

# Connect to the MQTT broker
client.connect(broker_address, port, 60)

# Publish data strings to a topic
def publish_data(topic, data):
    client.publish(topic, data)

# Example usage: publish a data string to a topic
topic = "BATTERYMANAGEMENTSYSTEM"
data = "Hello, world!"
publish_data(topic, data)

# Keep the client running to maintain the connection
client.loop_forever()
