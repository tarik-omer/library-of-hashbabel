// Copyright 2021 - 2022 Omer Tarik Ilhan 314CA
#ifndef USER_COMMANDS_H_
#define USER_COMMANDS_H_

#include "./structs.h"

void add_user(hashtable_t* ht);
void return_book(hashtable_t* users, hashtable_t* library);
void lost_book(hashtable_t* users, hashtable_t* library);
void borrow_book(hashtable_t* users, hashtable_t* library);

#endif  // USER_COMMANDS_H_
