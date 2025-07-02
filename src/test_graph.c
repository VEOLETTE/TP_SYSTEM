#include <stdio.h>
#include "data.h"
#include "graph.h"

extern int load_ratings(const char* filename, Rating* ratings, int max_ratings);

int main() {
    Rating ratings[100000];
    int count = load_ratings("data/ratings.txt", ratings, 100000);

    int nb_users = 1000;
    int nb_items = 1000;

    build_graph(ratings, count, nb_users, nb_items);
    pagerank(1, nb_items, 10); // utilisateur 1, top 10

    return 0;
}
