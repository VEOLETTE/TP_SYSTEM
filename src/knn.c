#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "knn.h"

float pearson_similarity(Rating* ratings, int count, int u1, int u2) {
    float sum_x = 0, sum_y = 0, sum_xy = 0, sum_x2 = 0, sum_y2 = 0;
    int n = 0;

    for (int i = 0; i < count; i++) {
        for (int j = 0; j < count; j++) {
            if (ratings[i].user_id == u1 && ratings[j].user_id == u2 &&
                ratings[i].item_id == ratings[j].item_id) {

                float x = ratings[i].rating;
                float y = ratings[j].rating;

                sum_x += x;
                sum_y += y;
                sum_xy += x * y;
                sum_x2 += x * x;
                sum_y2 += y * y;
                n++;
            }
        }
    }

    if (n == 0) return 0;

    float numerator = n * sum_xy - sum_x * sum_y;
    float denominator = sqrt((n * sum_x2 - sum_x * sum_x) * (n * sum_y2 - sum_y * sum_y));
    if (denominator == 0) return 0;

    return numerator / denominator;
}

float predict_rating(Rating* ratings, int count, int user_id, int item_id, int k) {
    float *sims = malloc(sizeof(float) * 1000);
    float *ratings_by_users = malloc(sizeof(float) * 1000);
    int nb = 0;

    for (int u = 1; u <= 100; u++) {
        if (u == user_id) continue;

        float sim = pearson_similarity(ratings, count, user_id, u);
        if (sim <= 0) continue;

        for (int i = 0; i < count; i++) {
            if (ratings[i].user_id == u && ratings[i].item_id == item_id) {
                sims[nb] = sim;
                ratings_by_users[nb] = ratings[i].rating;
                nb++;
                break;
            }
        }
    }

    if (nb == 0) return 0;

    // Tri des top-k voisins
    for (int i = 0; i < nb - 1; i++) {
        for (int j = i + 1; j < nb; j++) {
            if (sims[j] > sims[i]) {
                float tmp = sims[i]; sims[i] = sims[j]; sims[j] = tmp;
                float tmp2 = ratings_by_users[i]; ratings_by_users[i] = ratings_by_users[j]; ratings_by_users[j] = tmp2;
            }
        }
    }

    float sum = 0, sum_sim = 0;
    for (int i = 0; i < k && i < nb; i++) {
        sum += ratings_by_users[i] * sims[i];
        sum_sim += sims[i];
    }

    free(sims);
    free(ratings_by_users);

    if (sum_sim == 0) return 0;
    return sum / sum_sim;
}

void recommend_items(Rating* ratings, int count, int user_id, int k, int top_n) {
    int max_items = 200;
    float* scores = malloc(sizeof(float) * max_items);

    for (int item = 1; item <= max_items; item++) {
        int already_rated = 0;
        for (int i = 0; i < count; i++) {
            if (ratings[i].user_id == user_id && ratings[i].item_id == item) {
                already_rated = 1;
                break;
            }
        }
        if (already_rated) continue;

        scores[item] = predict_rating(ratings, count, user_id, item, k);
    }

    printf("Top %d recommandations pour l'utilisateur %d :\n", top_n, user_id);
    for (int i = 0; i < top_n; i++) {
        float max_score = 0;
        int best_item = 0;
        for (int item = 1; item <= max_items; item++) {
            if (scores[item] > max_score) {
                max_score = scores[item];
                best_item = item;
            }
        }
        if (best_item == 0) break;
        printf("Item %d avec score %.2f\n", best_item, scores[best_item]);
        scores[best_item] = 0; // Empêcher de le reprendre
    }

    free(scores);
}
