# TP Programmation Système – ENSEA Micro Shell (README)

## Description

Ce projet consiste à développer un **micro shell Unix minimal** nommé `enseash`, dans le cadre du TP de Programmation Système (2ᵉ année – ENSEA).

Ce README fait office de **compte rendu de TP**. Il décrit les fonctionnalités implémentées, les **fonctions système utilisées**, ainsi que les **entrées et sorties** associées. Aucun code source n’est inclus.

---

## Question 1 – Message d’accueil et sortie simple

### Objectif

* Afficher un message d’accueil stocké dans la macro `WELCOME`
* Afficher un prompt `enseash %`
* Quitter le shell lorsque l’utilisateur saisit `exit`

### Fonctionnement

* À l’entrée dans la console ENSEA, le message d’accueil est affiché
* Le shell attend une entrée utilisateur
* L’entrée est comparée à la chaîne `"exit"`
* En cas de correspondance, le shell se termine

### Fonctions utilisées

**`write(int fd, const void *buffer, size_t count)`**

* Rôle : affichage du message d’accueil et du prompt
* Entrée : sortie standard (1), message, taille
* Sortie : nombre d’octets écrits

**`read(int fd, void *buffer, size_t count)`**

* Rôle : lecture de l’entrée utilisateur
* Entrée : entrée standard (0), buffer, taille maximale
* Sortie : nombre d’octets lus

**`strncmp(const char *s1, const char *s2, size_t n)`**

* Rôle : comparaison de l’entrée utilisateur avec `exit`
* Sortie : 0 si les chaînes sont identiques

### Entrée / Sortie

* Entrée : `exit`
* Sortie : arrêt du shell

---

## Question 2 – Lecture et exécution d’une commande

### Objectif

* Lire une commande saisie dans la console
* Exécuter cette commande (exemple : `fortune`)
* Revenir au prompt après l’exécution
* Quitter avec `exit`

### Fonctionnement

* L’utilisateur saisit une commande
* Un processus fils est créé
* La commande est exécutée dans le processus fils
* Le processus père attend la fin de l’exécution
* Le prompt est réaffiché

### Fonctions utilisées

**`fork()`**

* Rôle : création d’un processus fils
* Sortie : 0 dans le fils, PID dans le père

**`execvp(const char *file, char *const argv[])`**

* Rôle : exécution de la commande saisie
* Entrée : nom de la commande, liste d’arguments
* Sortie : aucune en cas de succès

**`wait(int *status)`**

* Rôle : attente de la fin du processus fils
* Sortie : PID du processus terminé

### Entrée / Sortie

* Entrée : commande utilisateur (ex. `fortune`)
* Sortie : affichage du résultat de la commande

---

## Question 3 – Gestion de la sortie du shell

### Objectif

* Quitter le shell avec la commande `exit`
* Quitter le shell avec `Ctrl + D` (EOF)
* Afficher un message de fin avant fermeture

### Fonctionnement

* Si l’utilisateur saisit `exit`, le shell se ferme
* Si `read()` retourne 0 (EOF), le shell se ferme
* Le message `Bye bye...` est affiché avant la fin

### Fonctions utilisées

**`read()`**

* Rôle : détection de la fin de fichier (Ctrl + D)
* Sortie : 0 en cas d’EOF

**`write()`**

* Rôle : affichage du message de sortie

**`exit(int status)`**

* Rôle : terminaison du processus du shell

### Entrée / Sortie

* Entrée : `exit` ou `Ctrl + D`
* Sortie : message `Bye bye...` et fermeture du shell

---

## Conclusion

Ce TP permet de comprendre les bases d’un shell Unix :

* Entrées/sorties bas niveau (`read`, `write`)
* Gestion des processus (`fork`, `exec`, `wait`)
* Interaction utilisateur et contrôle du flot d’exécution

---

## Auteurs
TAOUIS AMR
EL KHAMLICHI CHAHID
2G2 TD1 TP1
