# Projet C

##### Louis CHARNAY & Léon DUMESTRE

## Sujet

L'objectif du projet est de créer un environnement client-serveur pour l'analyse et la gestion des images et de leurs métadonnées. Cependant,
contrairement aux applications client-serveur traditionnelles, nous n'enverrons pas les images au serveur pour analyse, mais nous effectuerons une analyse d'image côté client et enverrons des rapports de synthèse au serveur.

## Travail réalisé

Pour le moment nous avons réalisé l'intégralité des tâches 1 et 2 visibles ici : https://github.com/johnsamuelwrites/AlgoC/blob/master/Projet/Projet.md.

## Commandes disponibles

Les messages écrient par l'utilisateur côté Client sont envoyés au Serveur dans le format JSON. Par exemple :

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

La commande `message` peut contenir un texte avec n'importe quel caractère. Par exemple :
```
bonjour
/m bonjour
```

### Calcule

LA commande `calcule` doit contenir un opérateur (`+ - * / & | ~`) puis de 2 nombres avec ou sans décimales. Tout est séparé par un espace. Par exemple :
```
/n + 51 2.5
```

### Couleurs

La commande `couleurs` doit contenir le nombre de couleurs a ajouté puis chaque couleur contenant un `#` puis 6 caractères. Tout est séparé par une virgule. Par exemple :
```
/c 3,#123456,#ABCDEF,#123ABC 
```

### Balises

La commande `balises` doit contenir le nombre de balises a ajouté puis chaque balise contenant un `#` puis des caractères. Tout est séparé par une virgule. Par exemple :
```
/b 2,#bonjour,#arbre8
```