
#include "secrets.h"
#include "WiFi.h"
#include <Adafruit_INA219.h>

Adafruit_INA219 ina219;

// const char *ssid = "IOTs@Informatics";
// const char *pass = "Dec,09@2552";

// const char *ssid = "oyasumi";
// const char *pass = "11119999";

// Connect to WiFi for ESP32
// This code assumes you have the WiFi library installed and configured for your ESP32 board
void initWiFi()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  WiFi.setTxPower(WIFI_POWER_8_5dBm);
  Serial.print("Connecting to WiFi..");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print('.');
    delay(1000);
  }
  Serial.println();
  Serial.println("Connected to WiFi");
  Serial.print("Ip Address: ");
  Serial.println(WiFi.localIP());
}

// Function to read from DHT sensor and print the values
// // This function reads the humidity and temperature from the DHT sensor and prints them to the Serial
void readFromDHT()
{
// DHT sensor setup
#include <DHT.h>
#define DHTPIN 25     // Pin where the DHT22 is connected
#define DHTTYPE DHT22 // DHT 22 (AM2302)

  DHT dht(DHTPIN, DHTTYPE);
  dht.begin();
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // Read temperature as Celsius (the default)
  if (isnan(h) || isnan(t))
  {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  else
  {
    Serial.print("Humidity: ");
    Serial.print(h);
    Serial.print(" %\t");
    Serial.print("Temperature: ");
    Serial.print(t);
    Serial.println(" *C");
  }
}

void readFromWindSpeedSensor()
{
  int raw = analogRead(33); // ADC reading from GPIO 33

  // Convert ADC value to voltage (0–3.3V)
  float voltage = raw * (3.3 / 4095.0);

  // Convert voltage to current (I = V / R), R = 165Ω
  float current_mA = (voltage / 165.0) * 1000.0; // in milliamps

  // Convert current to wind speed (example: 4mA = 0 m/s, 20mA = 32.4 m/s)
  float windSpeed = (current_mA - 4.0) * (32.4 / 16.0); // adjust max speed as needed

  if (windSpeed < 0)
    windSpeed = 0;

  Serial.print("Raw ADC: ");
  Serial.print(raw);
  Serial.print(" | Voltage: ");
  Serial.print(voltage, 2);
  Serial.print(" V | Current: ");
  Serial.print(current_mA, 2);
  Serial.print(" mA | Wind Speed: ");
  Serial.print(windSpeed, 2);
  Serial.println(" m/s");

  delay(500);
}

void readFromWindDirectionSensor()
{
  int my_sensorValue1 = analogRead(35);
  float my_voltage = my_sensorValue1 * (5.0 / 1023.0);
  int my_direction = map(my_sensorValue1, 1168, 4095, 0, 360);
  // print out the value you read:
  Serial.println(my_sensorValue1);
  Serial.print("Voltage : ");
  Serial.println(my_voltage);
  Serial.print("Direction : ");
  Serial.println(my_direction);
  delay(300);
}

void setup()
{
  Serial.begin(9600);

  // Set WiFi to station mode and disconnect from an AP if it was previously connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  initWiFi();
}

void loop()
{
  readFromDHT();
  readFromWindSpeedSensor();
  readFromWindDirectionSensor();
}