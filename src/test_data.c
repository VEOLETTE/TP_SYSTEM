#include <stdio.h>
#include "data.h"

extern int load_ratings(const char* filename, Rating* ratings, int max_ratings);

int main() {
    Rating ratings[100000];
    int count = load_ratings("data/ratings.txt", ratings, 100000);

    if (count < 0) {
        printf("Erreur chargement\n");
        return 1;
    }

    printf("Nombre de lignes chargées : %d\n", count);
    printf("Exemple : User %d -> Item %d, Note %.1f\n",
           ratings[0].user_id,
           ratings[0].item_id,
           ratings[0].rating);

    return 0;
}
