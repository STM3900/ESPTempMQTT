#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <PubSubClient.h>

const char *ssid = "DevStation";     // Enter your WiFi name
const char *password = "import_dev"; // Enter WiFi password
const char *mqttServer = "192.168.213.14";
const int mqttPort = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

#define ONE_WIRE_BUS 4

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature sensor
DallasTemperature sensors(&oneWire);

void callback(char *topic, byte *payload, unsigned int length)
{

  Serial.print("Message arrived in topic: ");
  Serial.println(topic);

  // Faire des trucs

  Serial.println();
  Serial.println("-----------------------");
}

void setup(void)
{
  // Start serial communication for debugging purposes
  Serial.begin(9600);
  // Start up the library
  sensors.begin();

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");

  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);

  while (!client.connected())
  {
    Serial.println("Connecting to MQTT...");

    if (client.connect("EspTemp"))
    {

      Serial.println("connected");
    }
    else
    {
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
    }
  }
}

void loop(void)
{

  client.loop();

  sensors.requestTemperatures();
  double num = sensors.getTempCByIndex(0);
  char text[20];

  sprintf(text, "%f", num);

  client.publish("temp", text);

  delay(10000);

  /*
  sensors.requestTemperatures();

  Serial.print("Celsius temperature: ");

  Serial.print(sensors.getTempCByIndex(0));
  Serial.print(" - Fahrenheit temperature: ");
  Serial.println(sensors.getTempFByIndex(0));
  */
}