# Projet C

##### Louis CHARNAY & Léon DUMESTRE

## Sujet

L'objectif du projet est de créer un environnement client-serveur pour l'analyse et la gestion des images et de leurs métadonnées. Cependant,
contrairement aux applications client-serveur traditionnelles, nous n'enverrons pas les images au serveur pour analyse, mais nous effectuerons une analyse d'image côté client et enverrons des rapports de synthèse au serveur.

## Travail réalisé

L'intégralité des tâches du sujet de TP ont étés réalisés sont disponibles au lien suivant : https://github.com/johnsamuelwrites/AlgoC/blob/master/Projet/Projet.md

## Commandes disponibles

Les messages écrient par l'utilisateur côté Client sont envoyés au Serveur dans le format JSON. 

Par exemple :

```json
{
    "code" : "message",
    "valeurs" : [ "bonjour"]
}
```

Il existe actuellement 4 commandes différentes utilisables grâce à un slash accompagné d'une lettre puis du message adapté à la commande :

- message -> `/m` ou défaut (si pas de /)
- calcule -> `/n`
- couleurs -> `/c`
- balises -> `/b`

## Format des commandes

### Message

La commande `message` peut contenir un texte avec n'importe quel caractère. 

Par exemple :
```
bonjour
/m bonjour
```

Par défaut si l'utilisateur ne met pas de slash, le message est envoyé en tant que message.

### Calcule

La commande `calcule` doit contenir un opérateur (`+ - * / & | ~`) puis de 2 nombres avec ou sans décimales. Tout est séparé par un espace. 

Par exemple :
```
/n + 51 2.5
```

Les opérateurs suivant sont disponibles : `+ - * / & | ~` pour les opérations de base avec 2 nombres.

Les opérateurs suivants sont disponibles : `avg min max gap` pour les opérations statistiques pour plage de données (<100).

### Couleurs

La commande `couleurs` doit contenir le nombre de couleurs a ajouté puis chaque couleur contenant un `#` puis 6 caractères. Tout est séparé par une virgule. 

Par exemple :
```
/c 3,#123456,#ABCDEF,#123ABC 
```

Si aucunes couleurs n'est spécifiée, la commande renvoie une erreur.

### Balises

La commande `balises` doit contenir le nombre de balises a ajouté puis chaque balise contenant un `#` puis des caractères. Tout est séparé par une virgule. 

Par exemple :
```
/b 2,#bonjour,#arbre8
```

Si aucunes balises n'est spécifiée, la commande renvoie une erreur.

### Images
Au lancement du programme côté Client, il est possible de charger une image en spécifiant son chemin. 

L'utilisateur est invité à rentrer un nombre de couleurs à analyser.

Par exemple :
```
./client images/image.bmp
```

Si le nombre de couleurs n'est pas spécifié ou bien qu'il est inférieur à 1 ou supérieur à 30, le nombre de couleurs par défaut est de 10.