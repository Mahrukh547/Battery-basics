#include <WiFi.h>
#include <PubSubClient.h>

// --------------- NETPIE 2020 configuration ---------------
const char* mqtt_server = "broker.netpie.io";
const int mqtt_port = 1883;
const char* mqtt_Client = "d89f22f9-f25a-434d-93fe-fbb696c0e5df";
const char* mqtt_username = "QYoGRt7xdVG9utY3esP3G8osNhCNZxp9";
const char* mqtt_password = "Pj3KEwVmvGoQPT4EGf2Dxjn6nCcaAuHH";
const char* SolarTopic = "@msg/Solar";
const char* BatteryTopic = "@msg/Battery";
const char* LoadTopic = "@msg/Load";
const char*  ShadowTopic ="@shadow/data/update";

// ----- Wifi configuration---------------------
const char* ssid     = "Maha ";       // WiFi ssid
const char* password ="TGGS-DAAD23";   // WiFi password

WiFiClient espClient;
PubSubClient client(espClient); 
char msg[100];

/***************Reconnect function***************/
void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection…");
    if (client.connect(mqtt_Client, mqtt_username, mqtt_password)) {
      Serial.println("connected");
    }
    else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

/***************CallBack function***************/
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  String message;
  for (int i = 0; i < length; i++) {
    message = message + (char)payload[i];
  }
  Serial.println(message);
}

// Function to publish parameters to MQTT topic
void publishParameter(const char* topic, float param1, float param2, float param3 = 0, float param4 = 0) {
  Serial.println(param1);
  String dataString = String(param1) + "," + String(param2);
  if (param3 != 0) {
    dataString += "," + String(param3);
  }
  if (param4 != 0) {
    dataString += "," + String(param4);
  }
  dataString.toCharArray(msg, (dataString.length() + 1));
  client.publish(topic, msg);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Starting...");

  // Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // Initialize MQTT client
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi not connected, attempting to reconnect...");
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("\nWiFi reconnected");
  }

  if (!client.connected()) {
    reconnect();
  }
  
  client.loop();
  
  // Here you can add your code to read parameters from MATLAB and publish them
  
  // Example:
  float solarVoltage = readSolarVoltageFromMATLAB();
  float solarCurrent = readSolarCurrentFromMATLAB();
  float solarIrradiance = readSolarIrradianceFromMATLAB();
  
  float batterySOC = readBatterySOCFromMATLAB();
  float batteryTemperature = readBatteryTemperatureFromMATLAB();
  float batteryVoltage = readBatteryVoltageFromMATLAB();
  float batteryCurrent = readBatteryCurrentFromMATLAB();
  
  float loadVoltage = readLoadVoltageFromMATLAB();
  float loadCurrent = readLoadCurrentFromMATLAB();
  float loadPower = readLoadPowerFromMATLAB();
  
  // Publish parameters to MQTT topics
  publishParameter(SolarTopic, solarVoltage, solarCurrent, solarIrradiance);
  publishParameter(BatteryTopic, batterySOC, batteryTemperature, batteryVoltage, batteryCurrent);
  publishParameter(LoadTopic, loadVoltage, loadCurrent, loadPower);
  
  delay(2000); // delay before next action
}

// Example functions to read parameters from MATLAB
float readSolarVoltageFromMATLAB() {
  // Replace with code to read solar voltage from MATLAB
    float solarvoltage =1629.0 + random(0,10);
  return solarvoltage;
 
}

float readSolarCurrentFromMATLAB() {
  // Replace with code to read solar current from MATLAB
  float solarcurrent= 4.68+ random(0.0043,0.0005);
  return solarcurrent;
}

float readSolarIrradianceFromMATLAB() {
  // Replace with code to read solar irradiance from MATLAB
  float solarIrradiance=1000.0 + random(100,200);
  return solarIrradiance;
}

float readBatterySOCFromMATLAB() {
  // Replace with code to read battery SOC from MATLAB
  float SOC=70+ random(2,5);
  return SOC ;
}
float readBatteryTemperatureFromMATLAB() {
  // Replace with code to read battery temperature from MATLAB
  float temp= 25+random(25,20);
  return temp;
}
float readBatteryVoltageFromMATLAB() {
  // Replace with code to read battery voltage from MATLAB
  float batteryvoltage= 11.23 +random(2,5.5) ;
  return batteryvoltage;
}
float readBatteryCurrentFromMATLAB() {
  // Replace with code to read battery current from MATLAB
float batterycurrent= 0.000897 +random(0.002,0.0006) ;
  return batterycurrent;
}
float readLoadVoltageFromMATLAB() {
  // Replace with code to read load voltage from MATLAB
float loadvoltage= 11.248 +random(0.002,0.0006) ;
  return loadvoltage;
}
float readLoadCurrentFromMATLAB() {
  // Replace with code to read load current from MATLAB
  float loadcurrent= 1.599 +random(0.002,0.0006) ;
  return loadcurrent;
}
float readLoadPowerFromMATLAB() {
  // Replace with code to read load power from MATLAB
 float loadpower= 15.248 +random(0.002,0.0006) ;
  return loadpower;
}

