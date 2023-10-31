# TP3 (Node-Red)
Réaliser un Dashboard en lien avec votre ESP32 avec les mêmes ressources Hardware que le TP5 en remplaçant la LED classique par une LED RGB.
Attention que les données provenant du DHT11 (température & humidité) seront stockées dans une base de données (dB) de type MariaDB et seront affichées dans le Dashboard via un graphe.
Astuce: Pour connaître l’IP du serveur MQTT dans le contenair Docker, il faut connaître l’ID de celui‐ci en utilisant la commande suivante : docker ps

![image](https://github.com/Mlou103/Costa/assets/115471197/a2bbe64a-3c62-4b2c-aa96-e6f11aa3cb15)
Ensuite, pour connaître l’IP, lancer la commande suivante : docker inspect d10f045b3934

![image](https://github.com/Mlou103/Costa/assets/115471197/237a7347-91d0-46db-9482-94e4274bbc84)
