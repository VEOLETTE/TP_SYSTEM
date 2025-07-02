#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "graph.h"

#define MAX_NODES 2000
#define MAX_USERS 1000
#define MAX_ITEMS 1000
#define MAX_EDGES 100000
#define ALPHA 0.85
#define ITERATIONS 50

float adjacency[MAX_NODES][MAX_NODES];  // Matrice d’adjacence
int node_count = 0;
int offset_items = 1000; // Décalage pour que user=0..999 et item=1000..1999

void build_graph(Rating* ratings, int count, int nb_users, int nb_items) {
    node_count = nb_users + nb_items;
    memset(adjacency, 0, sizeof(adjacency));

    for (int i = 0; i < count; i++) {
        int user = ratings[i].user_id;
        int item = ratings[i].item_id + offset_items;

        adjacency[user][item] = 1;
        adjacency[item][user] = 1;
    }

    // Normalisation par ligne (probabilités de transition)
    for (int i = 0; i < node_count; i++) {
        float sum = 0;
        for (int j = 0; j < node_count; j++) {
            sum += adjacency[i][j];
        }
        if (sum > 0) {
            for (int j = 0; j < node_count; j++) {
                adjacency[i][j] /= sum;
            }
        }
    }
}

void pagerank(int user_id, int nb_items, int top_n) {
    float pr[MAX_NODES] = {0};
    float temp[MAX_NODES] = {0};
    int start_node = user_id;

    // Vecteur de personnalisation : 1 sur l’utilisateur
    for (int i = 0; i < node_count; i++) {
        pr[i] = (i == start_node) ? 1.0 : 0.0;
    }

    for (int it = 0; it < ITERATIONS; it++) {
        for (int i = 0; i < node_count; i++) {
            temp[i] = (1 - ALPHA) * (i == start_node ? 1.0 : 0.0);
            for (int j = 0; j < node_count; j++) {
                temp[i] += ALPHA * adjacency[j][i] * pr[j];
            }
        }

        for (int i = 0; i < node_count; i++) {
            pr[i] = temp[i];
        }
    }

    // Extraire les top-N articles (non notés)
    printf("Top %d articles recommandés pour l’utilisateur %d (Graph):\n", top_n, user_id);
    for (int t = 0; t < top_n; t++) {
        int best = -1;
        float best_score = -1;
        for (int i = offset_items; i < offset_items + nb_items; i++) {
            if (pr[i] > best_score) {
                best_score = pr[i];
                best = i;
            }
        }
        if (best != -1) {
            printf("Article %d avec score %.4f\n", best - offset_items, pr[best]);
            pr[best] = 0;
        }
    }
}
