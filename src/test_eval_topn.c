#include <stdio.h>
#include "eval_metrics.h"

int main() {
    // Simulation :
    // l'utilisateur aime les articles 10, 20, 30
    int relevant[] = {10, 20, 30};
    int total_relevant = 3;

    // Et on lui recommande les suivants :
    int recommended[] = {20, 40, 30, 50, 10};
    int top_n = 5;

    float map = compute_map(relevant, recommended, total_relevant, top_n);
    float ndcg = compute_ndcg(relevant, recommended, total_relevant, top_n);
    int hr = compute_hit_ratio(relevant, recommended, total_relevant, top_n);

    printf("MAP  = %.4f\n", map);
    printf("NDCG = %.4f\n", ndcg);
    printf("HR   = %d\n", hr);

    return 0;
}
