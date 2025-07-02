#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mf.h"
#include "utils.h" 

float U[MAX_USERS][LATENT_FACTORS];
float V[MAX_ITEMS][LATENT_FACTORS];
float bu[MAX_USERS];
float bi[MAX_ITEMS];
float global_mean = 0;

float predict_mf(int user_id, int item_id) {
    float dot = 0.0;
    for (int f = 0; f < LATENT_FACTORS; f++) {
        dot += U[user_id][f] * V[item_id][f];
    }
    return global_mean + bu[user_id] + bi[item_id] + dot;
}

void train_mf(Rating* ratings, int count, int num_epochs, float lr, float lambda) {
    // Initialisation aléatoire
    for (int u = 0; u < MAX_USERS; u++) {
        for (int f = 0; f < LATENT_FACTORS; f++) {
            U[u][f] = ((float)rand() / RAND_MAX) * 0.1;
        }
        bu[u] = 0;
    }

    for (int i = 0; i < MAX_ITEMS; i++) {
        for (int f = 0; f < LATENT_FACTORS; f++) {
            V[i][f] = ((float)rand() / RAND_MAX) * 0.1;
        }
        bi[i] = 0;
    }

    // Moyenne globale
    for (int i = 0; i < count; i++) {
        global_mean += ratings[i].rating;
    }
    global_mean /= count;

    // Apprentissage
    for (int epoch = 0; epoch < num_epochs; epoch++) {
        for (int i = 0; i < count; i++) {
            int u = ratings[i].user_id;
            int j = ratings[i].item_id;
            float r = ratings[i].rating;

            float pred = predict_mf(u, j);
            float e = r - pred;

            bu[u] += lr * (e - lambda * bu[u]);
            bi[j] += lr * (e - lambda * bi[j]);

            for (int f = 0; f < LATENT_FACTORS; f++) {
                float uf = U[u][f];
                float vf = V[j][f];

                U[u][f] += lr * (e * vf - lambda * uf);
                V[j][f] += lr * (e * uf - lambda * vf);
            }
        }
    }
}

void recommend_mf(Rating* ratings, int count, int user_id, int top_n) {
    float scores[MAX_ITEMS] = {0};
    int recommended[top_n];
    float top_scores[top_n];

    for (int item = 1; item < MAX_ITEMS; item++) {
        int already_rated = 0;
        for (int i = 0; i < count; i++) {
            if (ratings[i].user_id == user_id && ratings[i].item_id == item) {
                already_rated = 1;
                break;
            }
        }

        if (!already_rated) {
            scores[item] = predict_mf(user_id, item);
        }
    }

    printf("Top %d recommandations MF pour l’utilisateur %d :\n", top_n, user_id);
    for (int t = 0; t < top_n; t++) {
        int best = -1;
        float best_score = -1;
        for (int i = 1; i < MAX_ITEMS; i++) {
            if (scores[i] > best_score) {
                best_score = scores[i];
                best = i;
            }
        }

        if (best != -1) {
            printf("Item %d avec score %.2f\n", best, best_score);
            recommended[t] = best;
            top_scores[t] = best_score;
            scores[best] = 0;
        }
    }

    // ⬇️ Sauvegarde dans le fichier
    save_recommendations("MF", user_id, recommended, top_scores, top_n);
}
