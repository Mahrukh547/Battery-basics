#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

// Serial Communication Constants
const int BUFFER_SIZE = 200;
const char DELIMITER = ',';
char buffer[BUFFER_SIZE];
#define NR 20
float value[NR];
bool newData = false;

// WiFi Credentials
const char* ssid ="Maha ";
const char* password = "TGGS-DAAD23";

// NETPIE2020 Credentials
const char* mqtt_server = "broker.netpie.io";
const char* mqtt_Client = "d89f22f9-f25a-434d-93fe-fbb696c0e5df";
const char* mqtt_username = "QYoGRt7xdVG9utY3esP3G8osNhCNZxp9";
const char* mqtt_password = "Pj3KEwVmvGoQPT4EGf2Dxjn6nCcaAuHH";
const char* LEDTOPIC = "@msg/LEDStatus";
const char* SolarTopic = "@msg/Solar";
const char* BatteryTopic = "@msg/Battery";
const char* LoadTopic = "@msg/Load";
const char* TrailerTopic = "@msg/Trailer"; // New MQTT Topic for Trailer Energy

// Define LED pin
const int LED_PIN = 16; // Change to the appropriate pin number

// Initialize WiFi client and MQTT client
WiFiClient espClient;
PubSubClient client(espClient);

// Trailer energy parameters
const float trailerCapacity_kWh = 15.0;
const float solarPanelEfficiency = 0.8; // Efficiency of solar panels
const float batteryEfficiency = 0.9; // Efficiency of battery charging and discharging
const int trailerOperatingHours = 9;

float remainingTrailerEnergy_kWh = trailerCapacity_kWh;

// Function declarations
void setup_wifi();
void reconnect();
void callback(char* topic, byte* payload, unsigned int length);
void readSerialData();
void parseSerialData();
void writeSerialData();
float readSolarCurrentFromMATLAB();
float readSolarVoltageFromMATLAB();
float readSolarIrradianceFromMATLAB();
float readBatterySOCFromMATLAB();
float readBatteryTemperatureFromMATLAB();
float readBatteryVoltageFromMATLAB();
float readBatteryCurrentFromMATLAB();
float readLoadVoltageFromMATLAB();
float readLoadCurrentFromMATLAB();
float readLoadPowerFromMATLAB();
float calculateTrailerEnergyRemaining();

void setup() {
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  readSerialData();
  if (newData) {
    parseSerialData();
    writeSerialData();
    newData = false;
  }

  // Example: Read parameters from MATLAB and publish them to MQTT topics
  float solarCurrent = readSolarCurrentFromMATLAB();
  float solarVoltage = readSolarVoltageFromMATLAB();
  float solarIrradiance = readSolarIrradianceFromMATLAB();
  float batterySOC = readBatterySOCFromMATLAB();
  float batteryTemperature = readBatteryTemperatureFromMATLAB();
  float batteryVoltage = readBatteryVoltageFromMATLAB();
  float batteryCurrent = readBatteryCurrentFromMATLAB();
  float loadVoltage = readLoadVoltageFromMATLAB();
  float loadCurrent = readLoadCurrentFromMATLAB();
  float loadPower = readLoadPowerFromMATLAB();


void setup_wifi() 
{
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

void reconnect() 
{
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

void callback(char* topic, byte* payload, unsigned int length) 
{
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
      digitalWrite(LED_PIN, HIGH);
    } else if (message == "OFF") {
      digitalWrite(LED_PIN, LOW);
    }
  }
}

void readSerialData() 
{
  static byte index = 0;
  while (!Serial.available()) {}

  while (Serial.available() > 0 && !newData) {
    char receivedChar = Serial.read();
    if (receivedChar != 'X') {
      buffer[index] = receivedChar;
      index++;
      if (index >= BUFFER_SIZE) {
        index = BUFFER_SIZE - 1;
      }
    } else {
      buffer[index] = '\0'; 
      index = 0;
      newData = true;
    }
  }
}

void parseSerialData() 
{
  char *token = strtok(buffer, ",");
  int count = 0;
  while (token != NULL && count < NR) {
    value[count] = atof(token); 
    token = strtok(NULL, ",");
    count++;
  }
}

void writeSerialData()
 {
  for (int i = 0; i < (NR - 1); i++) {
    Serial.print(value[i], 6);
    Serial.print(",");
  }
  Serial.println(value[NR - 1], 6);
}

// Function to publish parameters to MQTT topic
void publishParameter(const char* topic, float param1, float param2, float param3 = 0, float param4 = 0) 
{
  String dataString = String(param1) + "," + String(param2);
  if (param3 != 0) {
    dataString += "," + String(param3);
  }
  if (param4 != 0) {
    dataString += "," + String(param4);
  }
  char msg[100];
  dataString.toCharArray(msg, (dataString.length() + 1));
  client.publish(topic, msg);
}
// Replace the publishParameter function with direct MQTT message publishing
// Replace the publishParameter function with direct MQTT message publishing

// Fix the max() function call by explicitly casting 0.0 to a float
float calculateTrailerEnergyRemaining() 
{
  // Your existing code
  
  // Calculate remaining energy in the trailer
  float remainingEnergy_kWh = trailerCapacity_kWh - energyConsumed_kWh;
  
  // Ensure the remaining energy is not negative
return max(remainingEnergy_kWh, float(0.0));

}
void publishParameter(const char* topic, float param1, float param2, float param3 = 0, float param4 = 0) 
  {
  String dataString = String(param1) + "," + String(param2);
  if (param3 != 0) {
    dataString += "," + String(param3);
  }
  if (param4 != 0) {
    dataString += "," + String(param4);
  }
  client.publish(topic, dataString.c_str());
}


  // Calculate remaining energy in the trailer
  remainingTrailerEnergy_kWh = calculateTrailerEnergyRemaining();

  // Publish parameters to MQTT topics
  publishParameter(SolarTopic, solarCurrent, solarIrradiance, solarVoltage);
  publishParameter(BatteryTopic, batterySOC, batteryTemperature, batteryVoltage, batteryCurrent);
  publishParameter(LoadTopic, loadVoltage, loadCurrent, loadPower);
  client.publish(TrailerTopic, String(remainingTrailerEnergy_kWh).c_str()); // Publish remaining trailer energy
  
  delay(2000); // delay before next action
}

// Function to calculate remaining energy in the trailer
float calculateTrailerEnergyRemaining() {
  // Example: Read parameters from the solar, battery, and load topics
  // These values should be read from MQTT topics
  float solarPower = 100; // Example solar power in watts
  float batteryPower = 50; // Example battery power in watts
  float loadPower = 30; // Example load power in watts
  
  // Calculate total power consumed (negative) by the system (solar + battery + load)
  float totalPowerConsumed = -(solarPower + batteryPower + loadPower);
  
  // Calculate energy consumed in kilowatt-hours (kWh)
  float energyConsumed_kWh = totalPowerConsumed / 1000.0 * (millis() / (1000.0 * 3600.0)); // Convert milliseconds to hours
  
  // Calculate remaining energy in the trailer
  float remainingEnergy_kWh = trailerCapacity_kWh - energyConsumed_kWh;
  
  // Ensure the remaining energy is not negative
  return max(remainingEnergy_kWh, 0.0);
}

//Example functions to read parameters from MATLAB
float readSolarVoltageFromMATLAB() {
  // Replace with code to read solar voltage from MATLAB
  float solarvoltage = 1629.0 + random(0,10);
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
