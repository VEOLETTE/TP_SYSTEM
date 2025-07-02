#ifndef KNN_H
#define KNN_H

#include "data.h"

float pearson_similarity(Rating* ratings, int count, int u1, int u2);
float predict_rating(Rating* ratings, int count, int user_id, int item_id, int k);
void recommend_items(Rating* ratings, int count, int user_id, int k, int top_n);

#endif
