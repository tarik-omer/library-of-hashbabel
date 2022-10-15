// Copyright 2021 - 2022 Omer Tarik Ilhan 314CA
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "./utils.h"
#include "./hashtable_base.h"
#include "./structs.h"
#include "./library_commands.h"
#include "./user_commands.h"
#include "./shut_down.h"
#include "./functions.h"

int main(void)
{
    // create the library - to be filled with books
    hashtable_t* library = ht_create(HMAX, hash_function_string,
                                     compare_function_strings);
    // create the user database - to be filled with users and user info
    hashtable_t* users = ht_create(HMAX, hash_function_string,
                                           compare_function_strings);

    double load_factor = 0;
    // buffers for commands / wrong input (trash)
    char* input = malloc(sizeof(char) * MAX_STRING_SIZE);
    DIE(input == NULL, "input malloc failed\n");
    char* trash = malloc(sizeof(char) * MAX_STRING_SIZE);
    DIE(trash == NULL, "trash malloc failed\n");
    // iterate until EXIT command
    while (1) {
        scanf("%s", input);
        // library commands
        if (!strcmp("ADD_BOOK", input)) {
            add_book(library);
        } else if (!strcmp("RMV_BOOK", input)) {
            rmv_book(library);
        } else if (!strcmp("GET_BOOK", input)) {
            get_book(library);
        } else if (!strcmp("ADD_DEF", input)) {
            add_def(library);
        } else if (!strcmp("GET_DEF", input)) {
            get_def(library);
        } else if (!strcmp("RMV_DEF", input)) {
            rmv_def(library);
        // user commands
        } else if (!strcmp("ADD_USER", input)) {
            add_user(users);
        } else if (!strcmp("BORROW", input)) {
            borrow_book(users, library);
        } else if (!strcmp("RETURN", input)) {
            return_book(users, library);
        } else if (!strcmp("LOST", input)) {
            lost_book(users, library);
        // others
        } else if (!strcmp("EXIT", input)) {
            shut_down_library(library);
            shut_down_userbase(users);
            break;
        } else {
            // read the rest of the line if the command is invalid
            fgets(trash, MAX_STRING_SIZE, stdin);
            printf("Invalid command.\n");
        }
        // resize book database if the load factor is greater than 1
        load_factor = (double)library->size / (double)library->hmax;
        if (load_factor > 1) {
            library = resize_hashtable(library, sizeof(book_t));
            // check that the resizing was done correctly
            if (library == NULL) {
                puts("Something went wrong..\n");
                exit(1);
            }
        }
        // resize user database if the load factor is greater than 1
        load_factor = (double)users->size / (double)users->hmax;
        if (load_factor > 1) {
            users = resize_hashtable(users, sizeof(user_t));
            // check that the resizing was done correctly
            if (users == NULL) {
                puts("Something went wrong..\n");
                exit(1);
            }
        }
    }
    free(input);
    free(trash);
    return 0;
}
