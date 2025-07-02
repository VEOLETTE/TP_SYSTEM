#include <stdio.h>
#include "data.h"
#include "knn.h"

extern int load_ratings(const char* filename, Rating* ratings, int max_ratings);

int main() {
    Rating ratings[100000];
    int count = load_ratings("data/ratings.txt", ratings, 100000);

    recommend_items(ratings, count, 1, 5, 10); // user 1, K=5, top 10

    return 0;
}
