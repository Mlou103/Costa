// Récupération des éléments HTML et stockage dans des constantes
const boutonLed = document.getElementById('bouton1'); // Bouton pour contrôler la LED
const humiditeValeur = document.getElementById('humiditeValeur'); // Élément pour afficher la valeur de l'humidité
const temperatureValeur = document.getElementById('temperatureValeur'); // Élément pour afficher la valeur de la température

// Création d'une connexion WebSocket avec le serveur
const ws = new WebSocket('ws://' + window.location.hostname + ':80/ws');

// Variable pour suivre l'état de la LED (allumée ou éteinte)
let ledAllumee = false;

// Fonction exécutée lorsque la connexion WebSocket est établie
ws.onopen = () => {
  console.log('WebSocket connecté'); // Affichage d'un message dans la console
};

// Fonction exécutée à la réception d'un message du serveur
ws.onmessage = (event) => {
  const message = event.data; // Récupération du message reçu
  // Vérification du contenu du message et exécution d'actions en conséquence
  if (message === 'allumerLED') {
    ledAllumee = true; // Mise à jour de l'état de la LED
    mettreAJourBouton(); // Mise à jour de l'affichage du bouton
  } else if (message === 'eteindreLED') {
    ledAllumee = false; // Mise à jour de l'état de la LED
    mettreAJourBouton(); // Mise à jour de l'affichage du bouton
  } else {
    // Si le message n'est pas une commande pour la LED, il contient les valeurs de température et d'humidité
    const data = JSON.parse(message); // Conversion du message JSON en objet
    // Mise à jour des éléments HTML avec les nouvelles valeurs
    humiditeValeur.textContent = data.humidite + "%";
    temperatureValeur.textContent = data.temperature + "°C";
  }
};

// Fonction exécutée lorsque la connexion WebSocket est fermée
ws.onclose = () => {
  console.log('WebSocket déconnecté'); // Affichage d'un message dans la console
};

// Fonction pour mettre à jour l'affichage du bouton en fonction de l'état de la LED
function mettreAJourBouton() {
  boutonLed.textContent = ledAllumee ? 'LED Off' : 'LED On'; // Changement du texte du bouton
  boutonLed.classList.toggle('active', ledAllumee); // Ajout/suppression de la classe 'active' en fonction de l'état de la LED
}

// Ajout d'un écouteur d'événement pour réagir aux clics sur le bouton
boutonLed.addEventListener('click', () => {
  ledAllumee = !ledAllumee; // Inversion de l'état de la LED
  mettreAJourBouton(); // Mise à jour de l'affichage du bouton
  ws.send(ledAllumee ? 'allumerLED' : 'eteindreLED'); // Envoi d'un message au serveur pour changer l'état de la LED
});
