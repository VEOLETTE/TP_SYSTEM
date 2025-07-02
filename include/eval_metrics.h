
#ifndef EVAL_METRICS_H
#define EVAL_METRICS_H

float compute_rmse(float* true_ratings, float* predicted, int count);
float compute_mae(float* true_ratings, float* predicted, int count);

// Top-N metrics
float compute_map(int* relevant_items, int* recommended, int total_relevant, int top_n);
float compute_ndcg(int* relevant_items, int* recommended, int total_relevant, int top_n);
int compute_hit_ratio(int* relevant_items, int* recommended, int total_relevant, int top_n);

#endif
