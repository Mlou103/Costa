#include <Arduino.h>
#include <DHT.h>
#include <DHT_U.h>
#include <FirebaseFS.h>
#include <Wifi.h>
#include <Update.h>

/* 1. Define the WiFi credentials */
#define WIFI_SSID "WIFI_AP"
#define WIFI_PASSWORD "WIFI_PASSWORD"

/* 2. Define the API Key */
#define API_KEY "API_KEY"

/* 3. Define the project ID */
#define FIREBASE_PROJECT_ID "PROJECT_ID"

/* 4. Define the user Email and password that alreadey registerd or added in your project */
#define USER_EMAIL "USER_EMAIL"
#define USER_PASSWORD "USER_PASSWORD"

//Variable Bouton
const int buttonPin = 12; // La broche à laquelle le bouton est connecté
int ButtonState = LOW; // État précédent du bouton (initialisé à LOW)
bool press = 0;
//Variable Led
#define  LED 2

//Variable LDR
const int LIGHT_SENSOR_PIN = 39;
int analogValue;

//Varibale DHT11
#define DHTPIN 26
#define DHTTYPE    DHT11 
DHT_Unified dht(DHTPIN, DHTTYPE);
uint32_t delayMS;
String Temp;
String Hum;

void setup() {
  Serial.begin(115200);

//Config Bouton
  pinMode(buttonPin, INPUT_PULLUP); // Active la résistance de pull-up interne

//Config LED
  pinMode(LED, OUTPUT); //configuration de la Pin en sortie/
  digitalWrite(LED, LOW); //Etat de base à 0/

  //Config DHT11
  dht.begin(); //Lance la lecture des données/
}

void loop() {
  //DHT
  sensors_event_t event;
  dht.temperature().getEvent(&event);

  //Bouton
  ButtonState = digitalRead(buttonPin) == LOW;

  // LDR
  analogValue = analogRead(LIGHT_SENSOR_PIN);
  float lightPercentage = 100-((analogValue / 4095.0) * 100.0); // Calcul du pourcentage inversé
  //Serial.println(lightPercentage, 2); // Affiche le pourcentage avec 2 décimales
if (ButtonState == 1 && press){
  press = 0;
  }
if (ButtonState == 0 && !press){
  press = 1;
  // Prise de données du DHT11
  if (isnan(event.temperature)) {
    Serial.println(F("Error reading temperature!"));
    }
  else {
    Temp = (event.temperature);
    Serial.println(Temp);
    }
  // Get humidity event and print its value.
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    Serial.println(F("Error reading humidity!"));
    }
  else {
    Hum = (event.relative_humidity);
    Serial.println(Hum);
    }
  }

  if (false) { // demande Led On
    digitalWrite(LED, HIGH);
  }
  if (false) { // demande Led Off
  digitalWrite(LED, LOW);
  }
}
