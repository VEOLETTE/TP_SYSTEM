
Système de Recommandation Client-Serveur en C
Description du projet
Ce projet implémente un système de recommandation distribué en langage C, basé sur une architecture client-serveur, où :

Le serveur central gère les données et les algorithmes de recommandation.

Les clients envoient des requêtes pour obtenir des recommandations personnalisées.

Plusieurs algorithmes de recommandation sont disponibles :

KNN (K-Nearest Neighbors)

MF (Matrix Factorization)

GRAPH (PageRank personnalisé)

Architecture générale

+----------+        requête TCP/IP        +----------+
|  CLIENT  |  ------------------------->  |  SERVEUR |
+----------+     (user_id, algo, nb)      +----------+
                       <-----------------
                    réponse = recommandations
Le projet est modulaire et respecte une organisation propre avec des bibliothèques statiques, des structures de données, et une interface terminale.

Structure du projet

tp_recommandation/
├── data/                → Données brutes (ratings.txt)
├── include/             → Fichiers .h
├── src/                 → Code source C (.c)
├── lib/                 → Bibliotheque statique (.o)
├── resultats.txt        → Fichier généré contenant les recommandations
├── Makefile             → Compilation rapide
└── README.md            → Ce fichier

Fonctionnalités disponibles
Fonction	Description
./client	Envoie une requête au serveur
./server	Reçoit la requête et répond avec des recommandations
menu (interface terminale)	Permet de tester localement tous les algorithmes
Sauvegarde des résultats	Enregistre les recommandations dans un fichier
Évaluation avec RMSE, MAP, NDCG...	Compare la qualité des recommandations

Détail des algorithmes
KNN (Collaboratif utilisateur)
Recommande selon les utilisateurs les plus similaires

Mesure de similarité : Pearson

Note prédite = moyenne pondérée des voisins

MF (Matrix Factorization)
Décompose la matrice des notes en facteurs latents

Optimisé par descente de gradient stochastique

Prédit les notes inconnues pour générer des recommandations

GRAPH (PageRank personnalisé)
Modélise les interactions utilisateur-article comme un graphe biparti

Utilise une version personnalisée de PageRank

Recommande selon la propagation de score depuis un utilisateur donné

Évaluation
Le projet intègre les métriques suivantes :

RMSE / MAE (pour les algorithmes de prédiction comme MF ou KNN)

MAP : Précision moyenne

NDCG : Précision pondérée par position

HR (Hit Ratio) : % de bonnes recommandations dans le top-N

Données utilisées
Le fichier ratings.txt contient les interactions sous le format :


Compilation et exécution
Compilation
make menu     
make server   
make client   
Exécution

./menu        # Interface locale avec les 3 algorithmes
./server &    # Lancer le serveur
./client      # Simuler une requête client distante

Résultat
Les recommandations sont automatiquement sauvegardées dans :

resultats.txt
Format :

Algo: MF | User: 1 | Top 3 recommandations:
Item 14 - Score: 4.52
Item 87 - Score: 4.31
Item 91 - Score: 4.12


Programmation en C structurée

Réseaux (sockets TCP/IP)

Traitement de fichiers et matrices

Implémentation d'algorithmes de recommandation

Métriques d’évaluation et sauvegarde de résultats

