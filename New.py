import random
import time
from paho.mqtt import client as mqtt_client

broker = 'mqtt.netpie.io'
port = 1883
python_topic = "@msg/to_me"  # Topic for Python device


# Generate a Client ID with the publish prefix.
client_id = 'd89f22f9-f25a-434d-93fe-fbb696c0e5df'
username = 'QYoGRt7xdVG9utY3esP3G8osNhCNZxp9'
password = 'Pj3KEwVmvGoQPT4EGf2Dxjn6nCcaAuHH'

def connect_mqtt():
    def on_connect(client, userdata, flags, rc):
        if rc == 0:
            print("Connected to MQTT Broker!")
        else:
            print("Failed to connect, return code %d\n", rc)

    client = mqtt_client.Client(client_id)
    client.username_pw_set(username, password)
    client.on_connect = on_connect
    client.connect(broker, port)
    return client

def publish(client, topic):
    msg_count = 1
    while True:
        time.sleep(1)
        msg = f"Message from Python: {msg_count}"
        result = client.publish(topic, msg)
        # result: [0, 1]
        status = result[0]
        if status == 0:
            print(f"Send `{msg}` to topic `{topic}`")
        else:
            print(f"Failed to send message to topic {topic}")
        msg_count += 1
        if msg_count > 5:
            break

def run():
    client = connect_mqtt()
    client.loop_start()
    # Publish data to Python topic
    publish(client, python_topic)
    client.loop_stop()

if __name__ == '__main__':
    run()
