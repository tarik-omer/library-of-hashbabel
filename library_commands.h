// Copyright 2021 - 2022 Omer Tarik Ilhan 314CA
#ifndef LIBRARY_COMMANDS_H_
#define LIBRARY_COMMANDS_H_

#include "./structs.h"

void add_book(hashtable_t* ht);
void shut_down_library(hashtable_t* ht);
void get_book(hashtable_t* ht);
void rmv_book(hashtable_t* ht);
void get_def(hashtable_t* ht);
void add_def(hashtable_t* ht);
void rmv_def(hashtable_t* ht);
void populate_book(hashtable_t* book);
void remove_book_at(hashtable_t* library, void* key);

#endif  // LIBRARY_COMMANDS_H_
