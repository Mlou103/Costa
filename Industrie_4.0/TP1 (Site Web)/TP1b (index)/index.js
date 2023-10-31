// Sélectionnez le bouton LED par son ID
const boutonLed = document.getElementById('bouton1');
const humiditeValeur = document.getElementById('humiditeValeur');
const temperatureValeur = document.getElementById('temperatureValeur');

// Définissez une variable pour suivre l'état de la LED (allumée ou éteinte)
let ledAllumee = false;

function mettreAJourValeurs() {
    // Effectuez une requête GET au serveur pour obtenir les nouvelles valeurs
    fetch("/valeursDHT")
        .then(response => response.json()) // Supposons que le serveur renvoie les données au format JSON
        .then(data => {
            // Mettez à jour les éléments HTML avec les nouvelles valeurs
            humiditeValeur.textContent = data.humidite + "%";
            temperatureValeur.textContent = data.temperature + "°C";
        })
        .catch(error => {
            console.error("Erreur lors de la récupération des valeurs : " + error);
        });
}

// Ajoutez un gestionnaire d'événement pour le clic sur le bouton LED
boutonLed.addEventListener('click', () => {
    // Inversez l'état de la LED
    ledAllumee = !ledAllumee;
    // Mettez à jour le texte du bouton en fonction de l'état de la LED
    boutonLed.textContent = ledAllumee ? 'LED Off' : 'LED On';
    boutonLed.classList.toggle('active');
    // Vous pouvez également envoyer une commande au dispositif pour allumer ou éteindre la LED ici
    fetch("Led"+ledAllumee)
});
setInterval(mettreAJourValeurs, 2000); // 5000 millisecondes = 2 secondes
