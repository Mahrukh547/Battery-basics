#include <WiFi.h>
#include <PubSubClient.h>

// Replace with your WiFi credentials
const char* ssid = "TPLINK";
const char* password = "23jan1990";

// Replace with your MQTT broker credentials
const char* mqtt_server = "mqtt.netpie.io";
const int mqtt_port = 1883;
const char* mqtt_client = "bdf19010-3a39-44c3-bcbb-46e6ca994735"; // Client ID
const char* mqtt_username = "YnnnTXURYVoxft6k2Zuzz68wCNYXdz8q";
const char* mqtt_password = "nbeHAitP1Ho7y3EWcjuHwqDjyvhiPgTW";
const char* mqtt_sub_topic = "@msg/to_me"; // Topic to subscribe to
const char* mqtt_pub_topic = "@msg/from_me"; // Topic to publish to

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
  delay(10);
  // Connect to WiFi
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");
}

void callback(char* topic, byte* payload, unsigned int length) {
  // Handle message arrived
  Serial.print("Message received on topic: ");
  Serial.print(topic);
  Serial.print(". Payload: ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Process the received message (you can add your logic here)

  // Example: Publish a new message based on the received data
  String receivedMessage;
  for (int i = 0; i < length; i++) {
    receivedMessage += (char)payload[i];
  }
  client.publish(mqtt_pub_topic, receivedMessage.c_str());
}

void reconnect() {
  while (!client.connected()) {
    Serial.println("Attempting MQTT connection...");
    
    if (client.connect(mqtt_client, mqtt_username, mqtt_password)) {
      Serial.println("Connected to MQTT broker");
      client.subscribe(mqtt_sub_topic);
    } else {
      Serial.print("Failed to connect to MQTT broker, rc=");
      Serial.print(client.state());
      Serial.println(". Trying again in 5 seconds...");
      
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
