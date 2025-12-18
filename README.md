# TP Programmation Système – Micro Shell ENSEA

## Présentation générale et objectifs

Le but de ce TP de Programmation Système est de développer un micro shell Unix minimaliste, appelé **enseash**, afin de mieux comprendre le fonctionnement des entrées/sorties bas niveau, la création de processus et l'exécution de commandes.

Ce document est le **compte rendu du TP**. Il décrit les fonctionnalités demandées dans les questions 1 à 7, ainsi que les principaux appels système utilisés. Pour plus de clarté, seules les explications de fonctionnement sont présentées, sans extraits de code.

---

## Question 1 – Message d'accueil et prompt

### Objectif

La première étape consiste à afficher un message d'accueil au démarrage du shell, puis un prompt pour inviter l'utilisateur à saisir une commande.

### Fonctionnement

Quand on lance le programme, un message d'accueil s'affiche sur la sortie standard. Ensuite, le shell affiche un prompt fixe (`enseash %`) et se met en attente d'une saisie utilisateur.

### Fonctions utilisées

**write**

* Rôle : permet d'afficher le message d'accueil et le prompt à l'écran
* Entrée : descripteur de la sortie standard, message à afficher, taille du message
* Sortie : nombre d'octets effectivement écrits

---

## Question 2 – Lecture de la commande utilisateur

### Objectif

Il s'agit de lire la commande entrée au clavier et de la stocker pour pouvoir la traiter par la suite.

### Fonctionnement

Le shell lit l'entrée utilisateur depuis l'entrée standard. On retire le caractère de fin de ligne pour simplifier le traitement de la commande.

### Fonctions utilisées

**read**

* Rôle : permet de lire la commande saisie par l'utilisateur
* Entrée : descripteur de l'entrée standard, buffer de réception, taille maximale
* Sortie : nombre d'octets lus (renvoie 0 si on atteint la fin de fichier)

---

## Question 3 – Exécution d'une commande simple

### Objectif

L'objectif ici est de permettre l'exécution d'une commande système (comme `fortune` par exemple) directement depuis le shell.

### Fonctionnement

Quand une commande est saisie, le shell crée un processus fils. Ce processus remplace son image mémoire par celle du programme à exécuter. Pendant ce temps, le processus père attend que l'exécution se termine avant de réafficher le prompt.

### Fonctions utilisées

**fork**

* Rôle : permet de créer un nouveau processus
* Sortie : renvoie 0 dans le processus fils, et l'identifiant du fils dans le processus père

**execvp**

* Rôle : exécute la commande demandée
* Entrée : nom de la commande et liste de ses arguments
* Sortie : ne renvoie rien si l'exécution réussit

**wait**

* Rôle : fait attendre la fin du processus fils
* Sortie : renvoie le statut de terminaison du fils

---

## Question 4 – Gestion de la commande `exit`

### Objectif

Il faut permettre à l'utilisateur de quitter le shell proprement.

### Fonctionnement

Si l'utilisateur tape la commande `exit`, le shell affiche un message de fin et se termine correctement.

### Fonctions utilisées

**strcmp**

* Rôle : compare la commande saisie avec la chaîne `exit`
* Sortie : indique si les chaînes sont identiques ou différentes

**exit**

* Rôle : termine l'exécution du shell

---

## Question 5 – Gestion de la fin de fichier (Ctrl + D)

### Objectif

Il faut gérer correctement la fermeture du shell quand l'utilisateur envoie un signal de fin de fichier.

### Fonctionnement

Quand la fonction de lecture renvoie 0, cela signifie qu'on a atteint la fin de fichier. Le shell détecte cette situation, affiche un message de sortie et se ferme proprement.

### Fonctions utilisées

**read**

* Sortie : renvoie 0 pour indiquer une fin de fichier

---

## Question 6 – Affichage du code de retour ou du signal

### Objectif

Avant chaque nouveau prompt, on doit afficher comment la commande précédente s'est terminée.

### Fonctionnement

Une fois qu'une commande a été exécutée, le shell analyse le statut de fin du processus fils. Il affiche ensuite soit le code de retour, soit le numéro du signal qui a causé l'arrêt du processus.

### Fonctions utilisées

**wait** et macros associées

* Rôle : récupèrent et interprètent le statut du processus fils
* Sortie : donnent des informations sur la terminaison normale ou par signal

---

## Question 7 – Mesure du temps d'exécution

### Objectif

On souhaite afficher le temps d'exécution de chaque commande lancée depuis le shell.

### Fonctionnement

On prend un horodatage juste avant et juste après l'exécution de la commande. La différence entre ces deux moments permet de calculer le temps écoulé, qui est ensuite affiché en millisecondes dans le prompt.

### Fonctions utilisées

**clock_gettime**

* Rôle : obtient une mesure de temps précise
* Entrée : horloge monotone
* Sortie : structure contenant le temps courant

---

## Conclusion

Ce TP nous a permis de mettre en pratique les notions fondamentales de la programmation système sous Unix : les entrées/sorties bas niveau, la création et la synchronisation de processus, l'exécution de programmes externes et la gestion des signaux.

Le micro shell qu'on a développé constitue une base fonctionnelle et claire pour bien comprendre le fonctionnement interne d'un shell Unix.

---

### Chargée de TP : BERANGER Claire
### TAOUIS Amr
### EL KHAMLICHI Chahid
### 2G2 – TD1 – TP1