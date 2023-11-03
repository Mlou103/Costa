// Inclusion des bibliothèques nécessaires
#include <Arduino.h>             // Bibliothèque de base pour les projets Arduino
#include <WiFi.h>                // Bibliothèque pour connecter l'ESP32 au réseau WiFi
#include <FS.h>                  // Bibliothèque pour interagir avec le système de fichiers
#include <LittleFS.h>            // Bibliothèque pour utiliser LittleFS, un système de fichiers optimisé pour les mémoires flash
#include <DHT.h>                 // Bibliothèque pour interagir avec les capteurs de température et d'humidité DHT
#include <DHT_U.h>               // Bibliothèque unifiée pour les capteurs DHT
#include "ESPAsyncWebServer.h"   // Bibliothèque pour créer un serveur web asynchrone sur l'ESP32
#include <AsyncTCP.h>            // Bibliothèque pour la gestion asynchrone des connexions TCP

//bibliothèques pour OLED
#include <Wire.h>               // Inclure la bibliothèque pour la communication I2C
#include <Adafruit_GFX.h>       // Inclure la bibliothèque pour les fonctionnalités graphiques
#include <Adafruit_SSD1306.h>   // Inclure la bibliothèque pour l'écran OLED
#define SCREEN_WIDTH 128        // Largeur de l'écran OLED en pixels
#define SCREEN_HEIGHT 64        // Hauteur de l'écran OLED en pixels
#define OLED_RESET    -1        // Pas de pin de réinitialisation pour la plupart des modules OLED
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
bool start = true;

// Définition des informations de connexion WiFi
const char* ssid = "CHEVAL_DE_3 9097";  // Nom du réseau WiFi
const char* password = "3E14h:67";      // Mot de passe du réseau WiFi

// Définition des broches et du type de capteur DHT
#define LED 2                  // Broche où la LED est connectée
#define DHTPIN 26              // Broche où le capteur DHT est connecté
#define DHTTYPE DHT11          // Type du capteur DHT (DHT11 dans ce cas)

// Création d'un objet DHT pour interagir avec le capteur
DHT_Unified dht(DHTPIN, DHTTYPE);
String Temp;                  // Chaîne de caractères pour stocker la valeur de la température
String Hum;                   // Chaîne de caractères pour stocker la valeur de l'humidité
bool ledAllumee = false;      // Variable booléenne pour stocker l'état de la LED (allumée ou éteinte)

// Création du serveur web et du WebSocket
AsyncWebServer server(80);    // Serveur web écoutant sur le port 80
AsyncWebSocket ws("/ws");     // WebSocket écoutant sur le chemin "/ws"
int clientsConnectes = 0;
// Fonction pour notifier tous les clients connectés via WebSocket des valeurs de température et d'humidité
void notifyClients() {
  // Crée et envoie un message JSON contenant les valeurs de température et d'humidité à tous les clients connectés
  ws.textAll(String("{\"temperature\":") + Temp + String(",\"humidite\":") + Hum + String("}"));
}

// Fonction pour dessiner la température et l'humidité sur l'écran OLED
void draw() {
  display.clearDisplay();
  //LED
  display.setTextSize(2); // Taille du texte plus grande pour l'état de la LED
  display.setTextColor(SSD1306_WHITE); // Couleur du texte en blanc
  display.setCursor(0, 0); // Position du début du texte en haut à gauche
  display.println(ledAllumee ? "LED ON" : "LED OFF");
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

  // Nombre de clients connectés
  display.setCursor(0, 40); // Position du début du texte pour le nombre de clients
  display.print("Clients: ");
  display.print(clientsConnectes);
  
  display.display(); // Actualisez l'affichage avec les nouvelles données
}

// Fonction pour notifier tous les clients connectés de l'état actuel de la LED
void notifyLedState() {
  // Détermine l'état de la LED et stocke le message correspondant dans "message"
  String message = ledAllumee ? "allumerLED" : "eteindreLED";
  
  // Envoie le message à tous les clients connectés via le WebSocket
  ws.textAll(message);
}

// Fonction pour gérer les messages reçus via le WebSocket
void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
  // Convertit l'argument en information sur le cadre WebSocket
  AwsFrameInfo *info = (AwsFrameInfo*)arg;
  
  // Vérifie si le message est complet et est de type texte
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    // Ajoute un caractère de fin de chaîne pour créer une chaîne C++
    data[len] = 0;
    
    // Convertit les données en une chaîne C++ et les stocke dans "message"
    String message = String((char*)data);
    
    // Vérifie le contenu du message et agit en conséquence
    if (message == "allumerLED") {
      // Allume la LED, met à jour l'état et envoie une notification
      digitalWrite(LED, HIGH);
      ledAllumee = true;
      Serial.println("LED allumée");
      notifyLedState();
    } else if (message == "eteindreLED") {
      // Éteint la LED, met à jour l'état et envoie une notification
      digitalWrite(LED, LOW);
      ledAllumee = false;
      Serial.println("LED éteinte");
      notifyLedState();
    } else if (message == "getValues") {
      // Envoie les valeurs de température et d'humidité aux clients
      notifyClients();
    }
  }
}

// Fonction pour gérer les événements du WebSocket
void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
             void *arg, uint8_t *data, size_t len) {
  // Traite les différents types d'événements
  switch (type) {
    case WS_EVT_CONNECT:
      clientsConnectes++;
      draw();
      // Un client s'est connecté, affiche son ID et son adresse IP, puis envoie l'état de la LED et de la température
      Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      notifyLedState();
      notifyClients();

      break;
    case WS_EVT_DISCONNECT:
      clientsConnectes--;
      draw();
      // Un client s'est déconnecté, affiche son ID
      Serial.printf("WebSocket client #%u disconnected\n", client->id());
      break;
    case WS_EVT_DATA:
      // Des données ont été reçues, les traite avec la fonction handleWebSocketMessage
      handleWebSocketMessage(arg, data, len);
      break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
      // Gère les autres types d'événements (pong et erreur), ici rien n'est fait
      break;
  }
}

// Fonction d'initialisation, exécutée une fois au démarrage
void setup() {

  // Initialiser l'écran OLED avec l'adresse I2C standard (0x3C ou 0x3D pour certains écrans)
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Boucle infinie si l'écran ne peut pas être initialisé
  }
  
  display.clearDisplay();  // Effacer l'affichage

  // Configure la broche de la LED en sortie et l'éteint
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);

  // Initialise le capteur DHT
  dht.begin();

  // Initialise la communication série et configure la vitesse de transmission
  Serial.begin(115200);

  // Initialise le système de fichiers LittleFS et vérifie s'il y a une erreur
  if (!LittleFS.begin()) {
    Serial.println("An Error has occurred while mounting LittleFS");
    return;
  }

  // Connecte l'ESP32 au réseau WiFi et attend la connexion
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("Connecté au Wi-fi");
  Serial.println("Adresse IP = ");
  Serial.println(WiFi.localIP());

  // Configure les routes pour le serveur web
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/index.html", "text/html");
  });

  server.on("/index.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/index.css", "text/css");
  });

  server.on("/index.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/index.js", "text/javascript");
  });

  // Configure le gestionnaire d'événements WebSocket et ajoute le WebSocket au serveur
  ws.onEvent(onEvent);
  server.addHandler(&ws);
  
  // Démarre le serveur web
  server.begin();
}

// Fonction de boucle principale, exécutée en continu
void loop() {
  static String lastTemp;
  static String lastHum;
  static bool lastLED;
  

  sensors_event_t event;
  dht.temperature().getEvent(&event);
  if (!isnan(event.temperature)) {
    Temp = String(event.temperature);
  }

  dht.humidity().getEvent(&event);
  if (!isnan(event.relative_humidity)) {
    Hum = String(event.relative_humidity);
  }

  if (start) {
    draw();
    start = false;
  }
  // Vérifiez si les valeurs ont changé
  if (Temp != lastTemp || Hum != lastHum) {
    // Mettre à jour l'affichage OLED avec la température et l'humidité
    draw();
    notifyClients();  // Notifiez les clients des nouvelles valeurs
    lastTemp = Temp;  // Mettez à jour les dernières valeurs connues
    lastHum = Hum;
  }
  // Mettre à jour l'affichage OLED avec l'état de la LED 
  if (ledAllumee != lastLED) {
    draw();
    lastLED = ledAllumee;
  }
}
