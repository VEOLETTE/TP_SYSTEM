#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "data.h"
#include "knn.h"
#include "mf.h"
#include "graph.h"
#include "eval_metrics.h"
#include "data_loader.h"

#define MAX_RATINGS 100000
#define MAX_USERS 1000
#define MAX_ITEMS 1000

Rating ratings[MAX_RATINGS];
int rating_count = 0;

void menu() {
    printf("\n==== SYSTÈME DE RECOMMANDATION ====\n");
    printf("1. Charger les données\n");
    printf("2. Recommandation par KNN\n");
    printf("3. Recommandation par MF\n");
    printf("4. Recommandation par Graphe (PageRank)\n");
    printf("5. Évaluer avec RMSE/MAE\n");
    printf("0. Quitter\n");
    printf("> Choix : ");
}

int main() {
    int choix;
    int user_id, nb;

    do {
        menu();
        scanf("%d", &choix);

        switch (choix) {
            case 1:
                rating_count = load_ratings("data/ratings.txt", ratings, MAX_RATINGS);
                if (rating_count > 0)
                    printf("Données chargées : %d ratings.\n", rating_count);
                else
                    printf("Échec du chargement.\n");
                break;

            case 2:
                printf("ID utilisateur : "); scanf("%d", &user_id);
                printf("Nombre de recommandations : "); scanf("%d", &nb);
                recommend_items(ratings, rating_count, user_id, 5, nb); // K = 5
                break;

            case 3:
                printf("ID utilisateur : "); scanf("%d", &user_id);
                printf("Nombre de recommandations : "); scanf("%d", &nb);
                train_mf(ratings, rating_count, 20, 0.01, 0.1);
                recommend_mf(ratings, rating_count, user_id, nb);
                break;

            case 4:
                printf("ID utilisateur : "); scanf("%d", &user_id);
                printf("Nombre de recommandations : "); scanf("%d", &nb);
                build_graph(ratings, rating_count, MAX_USERS, MAX_ITEMS);
                pagerank(user_id, MAX_ITEMS, nb);
                break;

            case 5: {
                float true_r[] = {4, 3, 5, 2};
                float pred_r[] = {3.9, 3.1, 4.9, 2.2};
                printf("RMSE = %.4f\n", compute_rmse(true_r, pred_r, 4));
                printf("MAE  = %.4f\n", compute_mae(true_r, pred_r, 4));

                int relevant[] = {10, 20, 30};
                int recommended[] = {20, 40, 30, 50, 10};
                printf("MAP  = %.4f\n", compute_map(relevant, recommended, 3, 5));
                printf("NDCG = %.4f\n", compute_ndcg(relevant, recommended, 3, 5));
                printf("HR   = %d\n", compute_hit_ratio(relevant, recommended, 3, 5));
                break;
            }

            case 0:
                printf("Au revoir !\n");
                break;

            default:
                printf("Choix invalide !\n");
        }

    } while (choix != 0);

    return 0;
}
