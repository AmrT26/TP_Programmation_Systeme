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

### Question 2 : Lecture de la commande utilisateur

Le shell doit pouvoir capturer une instruction saisie au clavier.

**Fonctionnement :** Le programme attend une saisie, puis remplace le caractère `\n` final par un caractère nul `\0` pour permettre le traitement de la chaîne.

**Appel système `read` :**

- **Rôle :** Lire des données depuis un descripteur (clavier).
- **Entrées :** Le descripteur (`STDIN_FILENO`), un tampon (buffer) de réception et la taille maximale à lire.
- **Sorties :** Retourne le nombre d'octets lus ou `0` en cas de fin de fichier.

---

### Question 3 : Exécution d'une commande simple

Le shell doit être capable de lancer un programme externe (ex: `fortune`) sans s'arrêter lui-même.

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

### Question 4 : Gestion de la commande `exit`

Permettre une fermeture propre du shell par l'utilisateur.

**Fonctionnement :** Le programme compare la saisie avec la chaîne `"exit"`.

**Fonction `strcmp` :**

- **Rôle :** Comparer deux chaînes de caractères.
- **Sorties :** `0` si les chaînes sont identiques.

---

### Question 5 : Gestion de la fin de fichier (Ctrl + D)

Le shell doit s'arrêter si le flux d'entrée est fermé.

**Fonctionnement :** Lorsque l'utilisateur presse Ctrl+D, l'appel `read` renvoie `0`. Le shell détecte cette valeur, affiche un message de sortie et termine le processus.

---

### Question 6 : Affichage du code de retour et du temps d'exécution

Le prompt devient dynamique pour afficher les informations de diagnostic de la commande précédente.

**Fonctionnement :** Le shell calcule la durée d'exécution et analyse si le fils s'est terminé normalement ou via un signal.

**Appel système `clock_gettime` :**

- **Rôle :** Récupérer la valeur d'une horloge système avec précision.
- **Entrées :** L'identifiant de l'horloge (`CLOCK_MONOTONIC`) et une structure `timespec`.

**Macros `WIFEXITED` / `WIFSIGNALED` :**

- **Rôle :** Interpréter l'entier de statut rempli par `wait`.

---

### Question 7 : Arguments et redirections de sortie

Support des commandes avec paramètres et redirection vers un fichier via le symbole `>`.

**Fonctionnement :** La commande est découpée en tokens. Si `>` est présent, le shell redirige la sortie standard vers le fichier spécifié.

**Appel système `open` :**

- **Rôle :** Ouvrir ou créer un fichier.
- **Entrées :** Chemin du fichier, options de contrôle (`O_CREAT`, `O_WRONLY`, `O_TRUNC`) et droits d'accès.

**Appel système `dup2` :**

- **Rôle :** Dupliquer un descripteur de fichier vers un autre.
- **Entrées :** Le descripteur source (le fichier) et la destination (`STDOUT_FILENO`).

---

## Conclusion

Ce TP a permis de manipuler les bases de la programmation système Unix. Le passage à une structure modulaire avec `utils.c` et `utils.h` facilite l'évolution du shell et la gestion des redirections complexes.

---

**Étudiants :** TAOUIS Amr & EL KHAMLICHI Chahid  
**Encadrante :** BERANGER Claire  
**Groupe :** 2G2 – TD1 – TP1