#include <stdio.h>
#include <math.h>
#include "data.h"

float compute_rmse(float* true_ratings, float* predicted, int count) {
    float sum = 0;
    for (int i = 0; i < count; i++) {
        float e = predicted[i] - true_ratings[i];
        sum += e * e;
    }
    return sqrt(sum / count);
}

float compute_mae(float* true_ratings, float* predicted, int count) {
    float sum = 0;
    for (int i = 0; i < count; i++) {
        sum += fabs(predicted[i] - true_ratings[i]);
    }
    return sum / count;
}

// Retourne 1 si l'item est dans la liste des éléments pertinents
int is_relevant(int item, int* relevant_items, int count) {
    for (int i = 0; i < count; i++) {
        if (relevant_items[i] == item)
            return 1;
    }
    return 0;
}

float compute_map(int* relevant_items, int* recommended, int total_relevant, int top_n) {
    float sum_precisions = 0.0;
    int hits = 0;

    for (int i = 0; i < top_n; i++) {
        if (is_relevant(recommended[i], relevant_items, total_relevant)) {
            hits++;
            sum_precisions += (float)hits / (i + 1);
        }
    }

    if (total_relevant == 0) return 0;
    return sum_precisions / total_relevant;
}

float compute_ndcg(int* relevant_items, int* recommended, int total_relevant, int top_n) {
    float dcg = 0.0;
    float idcg = 0.0;

    for (int i = 0; i < top_n; i++) {
        if (is_relevant(recommended[i], relevant_items, total_relevant)) {
            dcg += 1.0 / log2(i + 2);  // +2 car log(1) à i=0
        }
    }

    // IDCG = gain idéal (pertinence parfaite en début de liste)
    int max_rank = (top_n < total_relevant) ? top_n : total_relevant;
    for (int i = 0; i < max_rank; i++) {
        idcg += 1.0 / log2(i + 2);
    }

    if (idcg == 0) return 0;
    return dcg / idcg;
}

int compute_hit_ratio(int* relevant_items, int* recommended, int total_relevant, int top_n) {
    for (int i = 0; i < top_n; i++) {
        if (is_relevant(recommended[i], relevant_items, total_relevant))
            return 1;
    }
    return 0;
}
