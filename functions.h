// Copyright 2021 - 2022 Omer Tarik Ilhan 314CA
#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

#include "./structs.h"

void get_name(char* buff);
hashtable_t* resize_hashtable(hashtable_t* old_ht, int value_size);
void remove_function_library(hashtable_t* ht, void* key);

#endif  // FUNCTIONS_H_

