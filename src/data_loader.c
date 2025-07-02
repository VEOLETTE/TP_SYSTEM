#include <stdio.h>
#include <stdlib.h>
#include "data.h"

#define MAX_RATINGS 100000

int load_ratings(const char* filename, Rating* ratings, int max_ratings) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Erreur ouverture fichier");
        return -1;
    }

    int count = 0;
    while (count < max_ratings &&
           fscanf(file, "%d %d %d %f %ld",
                  &ratings[count].user_id,
                  &ratings[count].item_id,
                  &ratings[count].category_id,
                  &ratings[count].rating,
                  &ratings[count].timestamp) == 5) {
        count++;
    }

    fclose(file);
    return count;
}
