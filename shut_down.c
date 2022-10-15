// Copyright 2021 - 2022 Omer Tarik Ilhan 314CA
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./functions.h"
#include "./structs.h"
#include "./hashtable_base.h"
#include "./library_commands.h"
#include "./linked_list_base.h"

void remove_user_at(hashtable_t* users, void* key)
{
    // get the bucket where the user is at
    int index = users->hash_function(key) % users->hmax;
    linked_list_t* bucket = users->buckets[index];
    ll_node_t* entry = bucket->head;
    // iterate through bucket entries until matching key is found
    for (unsigned int i = 0; i < bucket->size; i++) {
        info* inf = entry->data;
        if (!users->compare_function(key, inf->key)) {
            // remove entry from bucket
            entry = ll_remove_nth_node(bucket, i);
            user_t* user = inf->value;
            // free book name, book content (hashtable), and the book itself
            free(user->name);
            free(user->borrowing->borrowed_book);
            free(user->borrowing);
            // free the info structure with its fields
            free(inf->key);
            free(inf->value);
            free(inf);
            free(entry);
            // once found, stop searching
            break;
        }
        entry = entry->next;
    }
    // userbase gets smaller by 1 :(
    users->size--;
}

// print top books and free memory
void shut_down_library(hashtable_t* library)
{
    printf("Books ranking:\n");
    int index = 1;
    // iterate through all the library and search for the top book
    while (library->size) {
        int first = 0;
        book_t* top_book = NULL;
        for (unsigned int i = 0; i < library->hmax; i++) {
            linked_list_t* bucket = library->buckets[i];
            ll_node_t* entry = bucket->head;
            while (entry != NULL) {
                // get the top book based on rating and purchases
                info* inf = entry->data;
                book_t* book = inf->value;
                if (first == 0) {
                    top_book = book;
                    first = 1;
                } else if (book->rating > top_book->rating) {
                    top_book = book;
                } else if (book->rating == top_book->rating &&
                           book->purchases > top_book->purchases) {
                    top_book = book;
                } else if (book->rating == top_book->rating &&
                           book->purchases == top_book->purchases &&
                           strcmp(top_book->name, book->name) > 0) {
                    top_book = book;
                }
                entry = entry->next;
            }
        }

        // print the top user and then remove him
        // (so we can move on to the next one)
        printf("%d. Name:%s Rating:%.3lf Purchases:%d\n", index,
               top_book->name, top_book->rating, top_book->purchases);
        remove_book_at(library, top_book->name);
        index++;
    }
    // free remaining memory (buckets array and the hashtable itself)
    ht_free(library);
}

// print top users and free memory
void shut_down_userbase(hashtable_t* users)
{
    printf("Users ranking:\n");
    int index = 1;

    // iterate through all hashtable entries, looking for banned users
    for (unsigned int i = 0; i < users->hmax; i++) {
        linked_list_t* bucket = users->buckets[i];
        ll_node_t* entry = bucket->head;
        int len = bucket->size;
        for (int i = 0; i < len; i++) {
            info* inf = entry->data;
            user_t* user = inf->value;
            ll_node_t* next_entry = entry->next;
            // remove user if he is banned
            if (user->is_banned == 1) {
                remove_user_at(users, inf->key);
            }

            entry = next_entry;
        }
    }
    // iterate through remaining users
    while (users->size) {
        int first = 0;
        user_t* top_user = NULL;
        for (unsigned int i = 0; i < users->hmax; i++) {
            linked_list_t* bucket = users->buckets[i];
            ll_node_t* entry = bucket->head;
            while (entry != NULL) {
                // get the top user based on points
                info* inf = entry->data;
                user_t* user = inf->value;
                if (first == 0) {
                    top_user = user;
                    first = 1;
                } else if (user->points > top_user->points &&
                           user->is_banned == 0) {
                    top_user = user;
                } else if (user->points == top_user->points &&
                           strcmp(top_user->name, user->name) > 0 &&
                           user->is_banned == 0) {
                    top_user = user;
                }
                entry = entry->next;
            }
        }
        // print the top user along with his points, and then remove it
        // (so we can move on to the next one)
        printf("%d. Name:%s Points:%d\n", index, top_user->name,
               top_user->points);
        remove_user_at(users, top_user->name);
        index++;
    }
    // free remaining memory (the buckets array and the hashtable itself)
    ht_free(users);
}
