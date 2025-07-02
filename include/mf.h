#ifndef MF_H
#define MF_H

#include "data.h"

#define LATENT_FACTORS 10
#define MAX_USERS 1000
#define MAX_ITEMS 1000

void train_mf(Rating* ratings, int count, int num_epochs, float lr, float lambda);
float predict_mf(int user_id, int item_id);
void recommend_mf(Rating* ratings, int count, int user_id, int top_n);

#endif
