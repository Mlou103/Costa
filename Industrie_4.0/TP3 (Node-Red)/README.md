# TP3 (Node-Red)
Réaliser un __Dashboard__ en lien avec votre ESP32 avec les __mêmes ressources Hardware__ que le TP2 en remplaçant la LED classique par une __LED RGB__.

__Attention__ que les données provenant du __DHT11_ (température & humidité) seront stockées dans une base de données (dB) de type MariaDB et seront affichées dans le Dashboard via un graphe.

Astuce: Pour connaître l’__IP__ du serveur __MQTT__ dans le contenair Docker, il faut connaître l’__ID__ de celui‐ci en utilisant la commande suivante : __docker ps__

![image](https://github.com/Mlou103/Costa/assets/115471197/a2bbe64a-3c62-4b2c-aa96-e6f11aa3cb15)
Ensuite, pour connaître l’__IP__, lancer la commande suivante : __docker inspect d10f045b3934__

![image](https://github.com/Mlou103/Costa/assets/115471197/237a7347-91d0-46db-9482-94e4274bbc84)
