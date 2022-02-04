#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <PubSubClient.h>

const char *ssid = "";       // Enter your WiFi name
const char *password = "";   // Enter WiFi password
const char *mqttServer = ""; // mqttServer Ip
const int mqttPort = 1883;

// Initialisation du module Wifi et du service PubSub
WiFiClient espClient;
PubSubClient client(espClient);

#define ONE_WIRE_BUS 4 // Le pin ou le capteur de température est branché (D4 du coup)

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature sensor
DallasTemperature sensors(&oneWire);

// Fonction de callback : est utile si l'on est subscribed à un topic et se déclanche quand on reçoit un message
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

  // Initialisation du wifi
  WiFi.begin(ssid, password);

  // Reste dans cette boucle tant que l'on n'est pas connecté au wifi
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");

  // Initialisation de l'instance vers le serveur mqtt
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);

  //Tant qu'on est pas connecté au mqtt, on essaie
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

  // Ecoute en permanance le mqtt
  client.loop();

  // récupère la température
  sensors.requestTemperatures();
  double num = sensors.getTempCByIndex(0);
  char text[20];

  // converti la température en string
  sprintf(text, "%f", num);

  // publish dans le topic temp la température
  client.publish("temp", text);

  // On attend 10 secondes avant de recommencer
  delay(10000);
}