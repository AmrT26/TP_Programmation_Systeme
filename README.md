# Compte rendu de TP : Programmation Système – Micro Shell ENSEA

Ce document retrace le développement du micro-shell **enseash**. L'objectif est de comprendre les mécanismes internes d'un shell Unix à travers la manipulation des processus, des descripteurs de fichiers et des mesures de performances.

---

## Détail des étapes de développement

### Question 1 : Message d'accueil et prompt

L'objectif est d'initialiser l'interface utilisateur au lancement du programme.

**Fonctionnement :** Le programme affiche un message de bienvenue, puis un prompt fixe (`enseash %`).

**Appel système `write` :**

- **Rôle :** Envoyer des données vers un descripteur de fichier.
- **Entrées :** Le descripteur (`STDOUT_FILENO`), le pointeur vers la chaîne de caractères et la taille en octets.
- **Sorties :** Retourne le nombre d'octets écrits ou `-1` en cas d'erreur.

---

### Question 2 : Lecture de la commande utilisateur, exécution d'une commande simple

Le shell doit pouvoir capturer une instruction saisie au clavier.

**Fonctionnement :** Le programme attend une saisie, puis remplace le caractère `\n` final par un caractère nul `\0` pour permettre le traitement de la chaîne.

**Appel système `read` :**

- **Rôle :** Lire des données depuis un descripteur (clavier).
- **Entrées :** Le descripteur (`STDIN_FILENO`), un tampon (buffer) de réception et la taille maximale à lire.
- **Sorties :** Retourne le nombre d'octets lus ou `0` en cas de fin de fichier.

Le shell doit être capable de lancer un programme externe (ex: `fortune`(n'a pas fonctionné), `ls`, `date` ) sans s'arrêter lui-même.

**Fonctionnement :** Le shell crée un clone de lui-même. Le fils exécute la commande demandée tandis que le père attend la fin du traitement.

**Appel système `fork` :**

- **Rôle :** Créer un nouveau processus par duplication.
- **Sorties :** Retourne `0` dans le processus fils et le PID du fils dans le parent.

**Appel système `execvp` :**

- **Rôle :** Remplacer l'image du processus courant par un nouveau programme.
- **Entrées :** Le nom du fichier exécutable et un tableau d'arguments.

**Appel système `wait` :**

- **Rôle :** Suspendre le père jusqu'à ce qu'un fils change d'état.
- **Sorties :** Retourne le PID du fils terminé et remplit une variable de statut.

---

### Question 3 : Gestion de la sortie du shell (`exit` et Ctrl+D)

Permettre une fermeture propre du shell par l'utilisateur.
Si on  saisit la commande `exit`, le shell affiche un message de sortie et se termine.
Si on ferme l’entrée standard avec `Ctrl+D`, l’appel système `read` retourne `0`,
ce qui est interprété comme une fin de fichier (EOF).
Le shell détecte cette condition et se termine proprement.

**Fonctionnement :** Le programme compare la saisie avec la chaîne `"exit"`.

**Fonction `strcmp` :**

- **Rôle :** Comparer deux chaînes de caractères.
- **Sorties :** `0` si les chaînes sont identiques.

Le shell doit s'arrêter si le flux d'entrée est fermé.

---

### Question 4 : Affichage du code de retour

Le prompt devient dynamique pour afficher les informations de diagnostic de la commande précédente.

**Fonctionnement :** Le shell analyse si le fils s'est terminé normalement (exit) ou via un signal(sign). Ainsi :
- Si le processus fils se termine normalement, le code de retour est affiché.
- S’il est interrompu par un signal, le numéro du signal est affiché.
**Appel système `clock_gettime` :**

- **Rôle :** Récupérer la valeur d'une horloge système avec précision.
- **Entrées :** L'identifiant de l'horloge (`CLOCK_MONOTONIC`) et une structure `timespec`.

**Macros `WIFEXITED` / `WIFSIGNALED` :**

- **Rôle :** Interpréter l'entier de statut rempli par `wait`.

---

### Question 5 : mesure du temps d’exécution des commandes

Le shell mesure la durée d’exécution de chaque commande lancée.

**Fonctionnement :**
- L’horloge est lue avant la création du processus fils.
- Une seconde lecture est effectuée après la fin de la commande.
- La durée est calculée en millisecondes.

**Appel système `clock_gettime` :**
- Utilisation de l’horloge `CLOCK_MONOTONIC` afin d’éviter les problèmes liés
  aux changements de l’horloge système.

---

### Question 6 : Exécution d’une commande complexe (avec arguments) 

Le shell doit être capable d’exécuter des commandes comportant des arguments.

**Fonctionnement :**
- La ligne de commande qu'on saisi est découpée en plusieurs éléments
  (nom de la commande et arguments).
- Ces éléments sont stockés dans un tableau de chaînes de caractères.
- Le processus fils appelle `execvp` avec ce tableau pour exécuter la commande
  avec ses arguments.

**Fonction utilisée :**
- `strtok` pour découper la chaîne de caractères en tokens séparés par des espaces.

**Appel système `execvp` :**
- Permet d’exécuter une commande en recherchant automatiquement l’exécutable
  dans les répertoires définis par la variable d’environnement `PATH`.
- Prend en paramètre le nom de la commande et un tableau d’arguments terminé par `NULL`.

Cette façon de faire permet au shell d’exécuter des commandes complexes comme
`hostname -i` ou `fortune -s osfortune`, tout en gardant le même mécanisme
de création de processus que pour les commandes simples.

---

### Question 7 : Gestion des redirections vers stdin et stdout avec ‘<’ et ‘>’  

Support des commandes avec paramètres et redirection vers un fichier via le symbole `>`. Exemple de l'écriture du résultat de **ls** dans un fichier texte.
Le contenu de la commande `ls` est alors écrit dans le fichier `files.txt` au lieu d’être affiché à l’écran.

**Fonctionnement :**
- La commande est découpée en tokens pour détecter la présence du symbole `>`.
- Le nom du fichier de redirection est récupéré.
- La sortie standard (`STDOUT_FILENO`) est redirigée vers le fichier indiqué avant
  l’exécution de la commande.

**Appel système `open` :**
- Ouvre ou crée le fichier de sortie.
- Utilisé avec les options :
  - `O_CREAT` : création du fichier s’il n’existe pas
  - `O_WRONLY` : ouverture en écriture seule
  - `O_TRUNC` : écrasement du contenu existant

**Appel système `dup2` :**
- Duplique le descripteur du fichier de sortie sur `STDOUT_FILENO`.
- Permet à la commande exécutée d’écrire directement dans le fichier.

**Appel système `execvp` :**
- Lance la commande avec ses arguments après la redirection.

---

## Conclusion

Ce TP nous a permis de manipuler les principaux concepts de la programmation système. La structuration finale du projet avec l'utilisation de `utils.c` et `utils.h` améliore la lisibilité du code et limite la redondance.

---

**Étudiants :** TAOUIS Amr & EL KHAMLICHI Chahid  
**Encadrante :** BERANGER Claire  
**Groupe :** 2G2 – TD1 – TP1
