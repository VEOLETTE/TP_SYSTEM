#include <stdio.h>
#include "eval_metrics.h"

int main() {
    float true_ratings[] = {4, 3, 5, 2};
    float predicted[] = {3.8, 3.2, 5.0, 2.5};
    int n = 4;

    printf("RMSE = %.4f\n", compute_rmse(true_ratings, predicted, n));
    printf("MAE  = %.4f\n", compute_mae(true_ratings, predicted, n));

    return 0;
}
