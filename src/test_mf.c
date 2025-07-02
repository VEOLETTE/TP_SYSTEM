#include <stdio.h>
#include "data.h"
#include "mf.h"

extern int load_ratings(const char* filename, Rating* ratings, int max_ratings);

int main() {
    Rating ratings[100000];
    int count = load_ratings("data/ratings.txt", ratings, 100000);

    train_mf(ratings, count, 20, 0.01, 0.1); // 20 epochs
    recommend_mf(ratings, count, 1, 10);     // recommandations pour user 1

    return 0;
}
