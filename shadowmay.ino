#include <WiFi.h>
#include <PubSubClient.h>

// --------------- NETPIE 2020 configuration ---------------
const char* mqtt_server = "broker.netpie.io";
const int mqtt_port = 1883;
const char* mqtt_Client = "d89f22f9-f25a-434d-93fe-fbb696c0e5df";
const char* mqtt_username = "QYoGRt7xdVG9utY3esP3G8osNhCNZxp9";
const char* mqtt_password = "Pj3KEwVmvGoQPT4EGf2Dxjn6nCcaAuHH";
const char* LEDTOPIC = "@msg/LEDStatus";
const char* SolarTopic = "@msg/Solar";
const char* BatteryTopic = "@msg/Battery";
const char* LoadTopic = "@msg/Load";
const char* ShadowTopic = "@shadow/data/update";

// ----- Wifi configuration---------------------
const char* ssid = "Maha ";     // WiFi ssid
const char* password = "TGGS-DAAD23";  // WiFi password

WiFiClient espClient;
PubSubClient client(espClient);
const int ledPin = 2; // Assuming your LED is connected to pin 2

// Function declarations
void setup_wifi();
void reconnect();
void callback(char* topic, byte* payload, unsigned int length);
void publishParameter(const char* topic, float param1, float param2, float param3 = 0, float param4 = 0, float shadowData = 0);
float readSolarCurrentFromMATLAB();
float readSolarVoltageFromMATLAB();
float readSolarIrradianceFromMATLAB();
float readBatterySOCFromMATLAB();
float readBatteryTemperatureFromMATLAB();
float readBatteryVoltageFromMATLAB();
float readBatteryCurrentFromMATLAB();
float readLoadPowerFromMATLAB();
float readShadowDataFromMATLAB(); // New function to read shadow data

void setup() {
  pinMode(ledPin, OUTPUT);
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

  // Example: Read parameters from MATLAB and publish them to MQTT topics
  float solarCurrent = readSolarCurrentFromMATLAB();
  float solarVoltage = readSolarVoltageFromMATLAB();
  float solarIrradiance = readSolarIrradianceFromMATLAB();
  float batterySOC = readBatterySOCFromMATLAB();
  float batteryTemperature = readBatteryTemperatureFromMATLAB();
  float batteryVoltage = readBatteryVoltageFromMATLAB();
  float batteryCurrent = readBatteryCurrentFromMATLAB();
  float loadPower = readLoadPowerFromMATLAB();
  float shadowData = readShadowDataFromMATLAB(); // Read shadow data

  // Publish parameters to MQTT topics including shadow data
  publishParameter(SolarTopic, solarCurrent, solarVoltage, solarIrradiance, shadowData);
  publishParameter(BatteryTopic, batterySOC, batteryTemperature, batteryVoltage, batteryCurrent, shadowData);
  publishParameter(LoadTopic,loadPower, shadowData);

  delay(2000); // delay before next action
}

// Function definitions
void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection…");
    if (client.connect(mqtt_Client, mqtt_username, mqtt_password)) {
      Serial.println("connected");
      client.subscribe(LEDTOPIC);
    }
    else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  String message;
  for (int i = 0; i < length; i++) {
    message = message + (char)payload[i];
  }
  Serial.println(message);

  if (String(topic) == LEDTOPIC) {
    if (message == "ON") {
      digitalWrite(ledPin, HIGH);
    } else if (message == "OFF") {
      digitalWrite(ledPin, LOW);
    }
  }
}

void publishParameter(const char* topic, float param1, float param2, float param3, float param4, float shadowData) {
  String dataString = String(param1) + "," + String(param2) + "," + String(param3) + "," + String(param4) + "," + String(shadowData);
  char msg[100];
  dataString.toCharArray(msg, (dataString.length() + 1));
  client.publish(topic, msg);
}

// Example functions to read parameters from MATLAB
float readSolarVoltageFromMATLAB() {
  // Replace with code to read solar voltage from MATLAB
  float solarvoltage = 1629.0 + random(0, 10);
  return solarvoltage;
}

float readSolarCurrentFromMATLAB() {
  // Replace with code to read solar current from MATLAB
  float solarcurrent = 4.68 + random(0.0043, 0.0005);
  return solarcurrent;
}

float readSolarIrradianceFromMATLAB() {
  // Replace with code to read solar irradiance from MATLAB
  float solarIrradiance = 1000.0 + random(100, 200);
  return solarIrradiance;
}

float readBatterySOCFromMATLAB() {
  // Replace with code to read battery SOC from MATLAB
  float SOC = 70 + random(2, 5);
  return SOC;
}

float readBatteryTemperatureFromMATLAB() {
  // Replace with code to read battery temperature from MATLAB
  float temp = 25 + random(25, 20);
  return temp;
}

float readBatteryVoltageFromMATLAB() {float batteryvoltage = 11.23 + random(2, 5.5);
  return batteryvoltage;
}

float readBatteryCurrentFromMATLAB() {
  // Replace with code to read battery current from MATLAB
  float batterycurrent = 0.000897 + random(0.002, 0.0006);
  return batterycurrent;
}



float readLoadPowerFromMATLAB() {
  // Replace with code to read load power from MATLAB
  float loadpower = 15.248 + random(0.0002, 0.0006);
  return loadpower;
}

float readShadowDataFromMATLAB() {
  // Replace with code to read shadow data from MATLAB
  float shadowData = 0.0; // Example, replace with actual reading
  return shadowData;
}
  // Replace with code to read battery voltage from MATLAB
