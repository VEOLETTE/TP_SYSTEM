#include <stdio.h>
#include "utils.h"

void save_recommendations(const char* algo, int user_id, int* items, float* scores, int n) {
    FILE* f = fopen("resultats.txt", "a"); // 'a' pour ajouter à la fin
    if (!f) {
        perror("Erreur ouverture fichier");
        return;
    }

    fprintf(f, "Algo: %s | User: %d | Top %d recommandations:\n", algo, user_id, n);
    for (int i = 0; i < n; i++) {
        fprintf(f, "Item %d - Score: %.4f\n", items[i], scores[i]);
    }
    fprintf(f, "\n");
    fclose(f);
}
