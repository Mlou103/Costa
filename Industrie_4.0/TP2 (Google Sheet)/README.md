# TP 2 (Google Sheet)
Dans un premier temps, vous câblerez les éléments suivants à votre NodeMCU:
- Une __LED__.
- Un __bouton-poussoir__ (BP).
- Un capteur __DHT11__ (ou DHt22).
- Une __LDR__ avec sa résistance (qui n’est pas représenté sur la photo) qui sera connectée à l’entrée A0.

## Fonctionnement du programme :
A chaque appuie sur le __BP__, le NodeMCU enverra les données du capteur __DHT11__ (Température et humidité)
et celle de la luminosité via la __LDR__ (0 à 100%) dans la feuille de calcul et la __LED__ devra s’illuminer
brièvement (100ms) pour avertir qu’il y a eu une transmission de données. Chaque donnée doit avoir son
«TimeStamp».
Ajouter une __nouvelle feuille__ pour afficher les __graphes des 3 valeurs__ (Température, humidité et luminosité)
en fonction du TimeStamp
