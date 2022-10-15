// Copyright 2021 - 2022 Omer Tarik Ilhan 314CA
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./structs.h"
#include "./hashtable_base.h"
#include "./linked_list_base.h"

void get_name(char* buff)
{
    // get the first quotation mark
    char gh;
    scanf(" %c", &gh);
    // read characters into the buffer until the closing quotation marks
    int i = 0;
    do {
        scanf("%c", &buff[i]);
        i++;
    } while (buff[i - 1] != '\"');
    // place the terminator where the quotation mark would be
    buff[i - 1] = '\0';
}

hashtable_t* resize_hashtable(hashtable_t* old_ht, int value_size)
{
    // set new HMAX to 2 * OLD_HMAX
    int new_hmax = old_ht->hmax * 2;
    // create the new hashtable
    hashtable_t* new_ht = ht_create(new_hmax, hash_function_string,
                                    compare_function_strings);
    // check if the value size is 0; if it is, the value is a word
    // (the hashtable to be resized is a book (its contents))
    int is_word = 0;
    if (value_size == 0)
        is_word = 1;
    // iterate through all entries and re-enter them into the new hashtable
    // ~rehashing~
    for (unsigned int i = 0; i < old_ht->hmax; i++) {
        linked_list_t* bucket = old_ht->buckets[i];
        ll_node_t* entry = bucket->head;

        for (unsigned int j = 0; j < bucket->size; j++) {
            if (entry == NULL) {
                puts("Something went wrong..\n");
                exit(1);
            }
            info* inf = entry->data;
            // if the value is a word, then the length must be determined
            // using strlen(word) + 1 (not sizeof(data_structure))
            if (is_word) {
                value_size = strlen((char*)inf->value) + 1;
            }
            // place the value into the new hashtable
            ht_put(new_ht, inf->key, strlen((char*)inf->key) + 1,
                   inf->value, value_size);
            entry = entry->next;
        }
    }
    // free remaining memory from the old hashtable;

    // we only copy the first layer of memory from the old hashtable
    // so we can use ht_free() to free only the first layer;

    // structures such as book_t or user_t which contain pointers
    // keep the memory the said pointers (which is not copied,
    // but also not free'd)
    ht_free(old_ht);
    return new_ht;
}
