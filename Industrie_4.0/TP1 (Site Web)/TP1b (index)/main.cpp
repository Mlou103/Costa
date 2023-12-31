#include <Arduino.h>
#include <WiFi.h>
#include "FS.h"
#include <LittleFS.h>
#include <DHT.h>
#include <DHT_U.h>

//bibliothèques pour OLED
#include <Wire.h>               // Inclure la bibliothèque pour la communication I2C
#include <Adafruit_GFX.h>       // Inclure la bibliothèque pour les fonctionnalités graphiques
#include <Adafruit_SSD1306.h>   // Inclure la bibliothèque pour l'écran OLED
#define SCREEN_WIDTH 128        // Largeur de l'écran OLED en pixels
#define SCREEN_HEIGHT 64        // Hauteur de l'écran OLED en pixels
#define OLED_RESET    -1        // Pas de pin de réinitialisation pour la plupart des modules OLED
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
bool start = true;

// Informations sur le réseau Wi-Fi
// Vous devez remplacer les informations pas celles de votre réseau
const char* ssid     = "CHEVAL_DE_3 9097";
const char* password = "3E14h:67";

//Variable Led
#define  LED 2
bool etatLed = false ;

//Varibale DHT11
#define DHTPIN 26
#define DHTTYPE    DHT11 
DHT_Unified dht(DHTPIN, DHTTYPE);
uint32_t delayMS;
String Temp;
String Hum;

// Variable lié au serveur
WiFiServer serveur(80);
String PageHtml;
File FileHtml;

String PageCSS;
File FileCSS;

String PageJava;
File FileJava;

void draw() {
  display.clearDisplay();
  //LED
  display.setTextSize(2); // Taille du texte plus grande pour l'état de la LED
  display.setTextColor(SSD1306_WHITE); // Couleur du texte en blanc
  display.setCursor(0, 0); // Position du début du texte en haut à gauche
  display.println(etatLed ? "LED ON" : "LED OFF");
// Temp et Hum
  display.setTextSize(1); // Taille du texte normale pour la température et l'humidité
  display.setCursor(0, 20); // Position du début du texte en dessous de l'état de la LED
  display.print("Temp: ");
  display.print(Temp);
  display.println(" C");

  display.setCursor(0, 30); // Position du début du texte pour l'humidité
  display.print("Hum: ");
  display.print(Hum);
  display.println(" %");
  
  display.display(); // Actualisez l'affichage avec les nouvelles données
}

// fonction Fichier serveur
String OpenFileHtml (){
  FileHtml = LittleFS.open("/index.html");
  if(!FileHtml){
    Serial.println("Failed to open file Html for reading");
    return "";
  }
  else{
    PageHtml = FileHtml.readString();
    FileHtml.close(); // Fermez le fichier
    return PageHtml;
  }

}
String OpenFileCSS (){
  FileCSS = LittleFS.open("/index.css");
  if(!FileCSS){
    Serial.println("Failed to open file CSS for reading");
    return "";
  }
  else{
    PageCSS = FileCSS.readString();
    FileCSS.close(); // Fermez le fichier
    return PageCSS;
  }
}

String OpenFileJava (){
  FileJava = LittleFS.open("/index.js");
  if(!FileJava){
    Serial.println("Failed to open file Java for reading");
    return "";
  }
  else{
    PageJava = FileJava.readString();
    FileJava.close(); // Fermez le fichier
    return PageJava;
  }
}
void setup() 
{
  // Initialiser l'écran OLED avec l'adresse I2C standard (0x3C ou 0x3D pour certains écrans)
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Boucle infinie si l'écran ne peut pas être initialisé
  }
  display.clearDisplay();  // Effacer l'affichage

  //Config LED
  pinMode(LED, OUTPUT); /*configuration de la Pin en sortie*/
  digitalWrite(LED, LOW); /*Etat de base à 0*/

  //Config DHT11
  dht.begin(); /*Lance la lecture des données*/

  // Nous allons utiliser la console pour afficher des informations
  Serial.begin(115200);
  if(!LittleFS.begin()){
    Serial.println("An Error has occurred while mounting LittleFS");
    return;
  }

  
  //Début de la connexion au réseau
  Serial.print("Connection au réseau ");
  Serial.println(ssid);
  WiFi.disconnect();
  WiFi.begin(ssid, password);

  // Tant que l'état n'est pas connecté, attendre...
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }

  // Une fois connecté, on affiche l'adresse IP du ESP32
  Serial.println("");
  Serial.println("Connecté au Wi-fi");
  Serial.println("Adresse IP = ");
  Serial.println(WiFi.localIP());

  serveur.begin();
}

void loop() 
{
  // Prise de données du DHT11
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    Serial.println(F("Error reading temperature!"));
  }
  else {
    Temp = (event.temperature);
  }
  // Get humidity event and print its value.
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    Serial.println(F("Error reading humidity!"));
  }
  else {
    Hum = (event.relative_humidity);
  }
  // Regarde s'il y a un client sur le serveur
  WiFiClient client = serveur.available();

  if(client)
  {
    Serial.println("Nouveau Client.");
    String DemandeClientStr = "";
    // rester dans la boucle tant que le client est connecté
    while (client.connected()) 
    { 
     if (client.available()) // Vérifie si le client a envoyé des données
     {
        char DemandeClient = client.read(); 
        DemandeClientStr += DemandeClient;

        Serial.println(DemandeClientStr); // sert à debuguer pour voir les demandes

        if (DemandeClient == '\n'){ // demande d'une nouvelle page
          Serial.println("Charge page HTML");
          client.println("HTTP/1.1 200 OK"); //Message d'accusé de reception
          client.println("Content-Type: text/html"); //securité + identification du language
          client.println();
          client.print(OpenFileHtml());
          client.stop();
        }

        if (DemandeClientStr == ("GET /index.css HTTP/1.1")) { // demande du CSS
          Serial.println("Charge page CSS");
          client.println("HTTP/1.1 200 OK"); //Message d'accusé de reception
          client.println("Content-Type: text/css"); //securité + identification du language
          client.println();
          client.print(OpenFileCSS());
          client.stop();
        }

        if (DemandeClientStr == ("GET /index.js HTTP/1.1")) { // demande du Java
          Serial.println("Charge page Java");
          client.println("HTTP/1.1 200 OK"); //Message d'accusé de reception
          client.println("Content-Type: text/js"); //securité + identification du language
          client.println();
          client.print(OpenFileJava());
          client.stop();
        }

 
        if (DemandeClientStr == ("GET /valeursDHT HTTP/1.1")){
         // demande des valeurs DHT11
          Serial.println("Envoi des valeurs DHT11");
          String reponse = String("{\"temperature\":") + Temp + String(",\"humidite\":") + Hum + String("}");
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: application/json");
          client.println();
          client.print(reponse);
          client.stop();
          etatLed = true ;
          draw();
        }
        if (DemandeClientStr == ("GET /Ledtrue HTTP/1.1")){
          digitalWrite(LED, HIGH);
          draw();
        }
        if (DemandeClientStr == ("GET /Ledfalse HTTP/1.1")) { // demande Led Off
          digitalWrite(LED, LOW);
          etatLed = false ;
          draw();
        }
      }
    }
  }
}
