#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NB_LIGNES 1000         // Nombre de lignes à générer
#define MAX_USERS 100          // ID utilisateurs de 1 à 100
#define MAX_ITEMS 200          // ID articles de 1 à 200
#define MAX_CATEGORIES 10      // ID catégories de 1 à 10

int main() {
    FILE *file = fopen("data/ratings.txt", "w");
    if (!file) {
        perror("Erreur ouverture fichier");
        return 1;
    }

    srand(time(NULL)); // Initialisation aléatoire

    for (int i = 0; i < NB_LIGNES; i++) {
        int user_id = rand() % MAX_USERS + 1;
        int item_id = rand() % MAX_ITEMS + 1;
        int category_id = rand() % MAX_CATEGORIES + 1;
        float rating = (rand() % 5) + 1 + ((float)(rand() % 10)) / 10; // Note entre 1.0 et 5.9
        long timestamp = (rand() % (20 * 365 * 24 * 3600)) + 946684800; // Entre 2000 et 2020

        fprintf(file, "%d %d %d %.1f %ld\n", user_id, item_id, category_id, rating, timestamp);
    }

    fclose(file);
    printf("Fichier ratings.txt généré avec %d lignes.\n", NB_LIGNES);
    return 0;
}
