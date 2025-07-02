#ifndef GRAPH_H
#define GRAPH_H

#include "data.h"

void build_graph(Rating* ratings, int count, int nb_users, int nb_items);
void pagerank(int user_id, int nb_items, int top_n);

#endif
