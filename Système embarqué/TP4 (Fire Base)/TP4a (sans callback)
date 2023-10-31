// Inclure les bibliothèques nécessaires pour le projet
#include <Arduino.h>
#include <WiFi.h>
#include <DHT.h>
#include <Firebase_ESP_Client.h>

// Inclure des fichiers d'aide pour le jeton et la base de données
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

// Mettez vos identifiants WiFi ici
#define WIFI_SSID "CHEVAL_DE_3 9097"
#define WIFI_PASSWORD "3E14h:67"

// Mettez la clé API de votre projet Firebase ici
#define API_KEY "AIzaSyBITx2fRtPBMOkWFVhCSCUtwVryAuCRMKw"

// URL de la base de données en temps réel Firebase
#define DATABASE_URL "https://tp4a-12245-default-rtdb.europe-west1.firebasedatabase.app/"

// Configuration de la LED
#define  LED 2
bool Etat_LED;

// Configuration du capteur DHT11
#define DHTPIN 26
#define DHTTYPE DHT11
DHT dht (DHTPIN, DHTTYPE);

// Initialisation des objets pour Firebase
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// Variables pour la logique du programme
unsigned long sendDataPrevMillis = 0;
int count = 0;
int temp, humi, valeur_ldr;
bool signupOK = false;
const int ldr = 39;

// Configuration initiale
void setup(){
  // Configuration de la communication série et du WiFi
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  
  // Attendre la connexion au WiFi
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(300);
  }
  
  // Informations de connexion WiFi
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  // Configuration de Firebase
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  
  // Inscription
  if (Firebase.signUp(&config, &auth, "", "")){
    Serial.println("ok");
    signupOK = true;
  }
  else{
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }
  
  // Autres configurations pour Firebase
  config.token_status_callback = tokenStatusCallback;
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  // Configuration de la LED
  pinMode(LED, OUTPUT); //configuration de la Pin en sortie/
  digitalWrite(LED, LOW); //Etat de base à 0/

  // Démarrage du capteur DHT11 et configuration du capteur de luminosité
  dht.begin();
  pinMode(ldr, INPUT);
}

// Boucle principale
void loop(){
  // Envoyer des données si Firebase est prêt et si 15 secondes se sont écoulées
  if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 15000 || sendDataPrevMillis == 0)){
    sendDataPrevMillis = millis();
    
    // Lecture des capteurs
    temp = dht.readTemperature();
    humi = dht.readHumidity();
    valeur_ldr = analogRead(ldr);
    // Mise en % de la valeur de la ldr
    valeur_ldr = (2500 - valeur_ldr) * 100 / 2500;

    // Envoi de la température à Firebase
    if (Firebase.RTDB.setInt(&fbdo, "TP4A/Temperature", temp)){
      Serial.println("Temp : PASSED");
    }
    else {
      Serial.println("Temp : FAILED");
    }

    // Envoi de l'humidité à Firebase
    if (Firebase.RTDB.setInt(&fbdo, "TP4A/Humidite", humi)){
      Serial.println("humidity : PASSED");
    }
    else {
      Serial.println("humidity : FAILED");
    }

    // Envoi de la luminosité à Firebase
    if (Firebase.RTDB.setInt(&fbdo, "TP4A/Luminosite", valeur_ldr)){
      Serial.println("Luminosite : PASSED");
    }
    else {
      Serial.println("luminosite : FAILED");
    }
    
    if (Firebase.RTDB.getBool(&fbdo, "TP4A/LED")) {
      if (fbdo.dataType() == "boolean") {
        Etat_LED = fbdo.boolData();
        digitalWrite(LED, Etat_LED);
        Serial.println("Lecture LED : PASSED ");
      }
      else {
        Serial.println("Lecture LED : FAILED ");
      }
    }
  }
}
