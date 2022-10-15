// Copyright 2021 - 2022 Omer Tarik Ilhan 314CA
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "./structs.h"
#include "./hashtable_base.h"
#include "./linked_list_base.h"
#include "./utils.h"
#include "./functions.h"

void remove_book_at(hashtable_t* library, void* key)
{
    int index = library->hash_function(key) % library->hmax;
    linked_list_t* bucket = library->buckets[index];
    ll_node_t* entry = bucket->head;
    // iterate through bucket entries until matching key is found
    for (unsigned int i = 0; i < bucket->size; i++) {
        info* inf = entry->data;
        if (!library->compare_function(key, inf->key)) {
            // remove entry from bucket
            entry = ll_remove_nth_node(bucket, i);
            book_t* book = inf->value;
            // free book name, book content (hashtable), and the book itself
            free(book->name);
            ht_free(book->content);
            free(book);
            // free the info structure with its fields
            free(inf->key);
            free(inf);
            free(entry);
            // once found, stop searching
            break;
        }
        entry = entry->next;
    }
    library->size--;
}

void populate_book(hashtable_t* book, int num_def)
{
    char* key = malloc(MAX_WORD_SIZE * sizeof(char));
    DIE(key == NULL, "key malloc failed\n");
    char* value = malloc(MAX_WORD_SIZE * sizeof(char));
    DIE(value == NULL, "value malloc failed\n");

    for (int i = 0; i < num_def; i++) {
        scanf("%s %s", key, value);
        ht_put(book, key, strlen(key) + 1, value, strlen(value) + 1);
    }
    free(key);
    free(value);
}

void add_book(hashtable_t* ht)
{
    // read book name (into a buffer) and number of definitions
    char* buff = malloc(sizeof(char) * MAX_NAME_SIZE);
    DIE(buff == NULL, "buff malloc failed\n");
    get_name(buff);

    int num_def;
    scanf("%d", &num_def);
    // safeguard in case hmax is 0 or lower than 0
    int min_hmax = 1;
    if (num_def > 0)
        min_hmax = num_def;

    if (ht_has_key(ht, buff)) {
        remove_book_at(ht, buff);
    }

    // initialize a book structure
    book_t* book = malloc(sizeof(book_t));
    DIE(book == NULL, "book malloc failed\n");

    book->unreachable = 0;
    book->purchases = 0;
    book->rating = 0;
    book->rating_sum = 0;

    // place the name into the book name field, with its actual size
    // (not max buffer size)
    book->name = malloc(strlen(buff) + 1);
    DIE(book->name == NULL, "book name malloc failed\n");
    memcpy(book->name, buff, strlen(buff) + 1);

    // create a hashtable for the book content
    book->content = ht_create(min_hmax, hash_function_string,
                              compare_function_strings);

    // populate the book with definitions and place the book into the library
    populate_book(book->content, num_def);
    ht_put(ht, book->name, strlen(book->name) + 1, book, sizeof(book_t));
    // free remamining pointers (the content was copied somewhere else)
    free(buff);
    free(book);
}

void rmv_book(hashtable_t* library)
{
    // read book name (into a buffer)
    char* buff = malloc(sizeof(char) * MAX_NAME_SIZE);
    DIE(buff == NULL, "buff malloc failed\n");
    get_name(buff);

    // check whether the book exists
    if (!ht_has_key(library, buff)) {
        printf("The book is not in the library.\n");
        free(buff);
        return;
    }
    // remove book with given name
    remove_book_at(library, buff);
    free(buff);
}

void get_book(hashtable_t* library)
{
    // read book name (into a buffer)
    char* buff = malloc(sizeof(char) * MAX_NAME_SIZE);
    DIE(buff == NULL, "buff malloc failed\n");
    get_name(buff);

    // check whether the book exists
    if (ht_has_key(library, buff)) {
        // if it does, get it and print the required data
        book_t* book = ht_get(library, buff);
        printf("Name:%s Rating:%.3lf Purchases:%d\n", book->name,
               book->rating, book->purchases);
    } else {
        printf("The book is not in the library.\n");
    }
    // free the remaining buffer
    free(buff);
}

void add_def(hashtable_t* library)
{
    // read book name (into a buffer)
    char* buff = malloc(sizeof(char) * MAX_NAME_SIZE);
    DIE(buff == NULL, "buff malloc failed\n");
    get_name(buff);

    // read the key and value to be added
    char* key = malloc(MAX_WORD_SIZE * sizeof(char));
    DIE(key == NULL, "key malloc failed\n");
    char* value = malloc(MAX_WORD_SIZE * sizeof(char));
    DIE(value == NULL, "value malloc failed\n");
    scanf("%s %s", key, value);

    if (!ht_has_key(library, buff)) {
        printf("The book is not in the library.\n");
        free(key);
        free(value);
        free(buff);
        return;
    }

    // get the book
    book_t* book = ht_get(library, buff);
    // add the definition to the book
    ht_put(book->content, key, strlen(key) + 1, value, strlen(value) + 1);

    // free remaining buffers
    free(key);
    free(value);
    free(buff);

    // resize if the load factor is greater than 1
    double load_factor = (double)book->content->size /
                        (double)book->content->hmax;
    if (load_factor > 1) {
        book->content = resize_hashtable(book->content, 0);
        if (book->content == NULL) {
            puts("Something went wrong..\n");
            exit(1);
        }
    }
}

void get_def(hashtable_t* library)
{
    // read book name (into a buffer)
    char* buff = malloc(sizeof(char) * MAX_NAME_SIZE);
    DIE(buff == NULL, "buff malloc failed\n");
    get_name(buff);
    // read definition (into a buffer)
    char* key = malloc(MAX_WORD_SIZE * sizeof(char));
    DIE(key == NULL, "key malloc failed\n");
    scanf("%s", key);

    if (!ht_has_key(library, buff)) {
        printf("The book is not in the library.\n");
        free(key);
        free(buff);
        return;
    }

    // get the book
    book_t* book = ht_get(library, buff);

    if (!ht_has_key(book->content, key)) {
        printf("The definition is not in the book.\n");
        free(key);
        free(buff);
        return;
    }

    // get the definition
    char* value = ht_get(book->content, key);
    printf("%s\n", value);

    // free remaining buffers
    free(key);
    free(buff);
}

void rmv_def(hashtable_t* library)
{
    // read book name (into a buffer)
    char* buff = malloc(sizeof(char) * MAX_NAME_SIZE);
    DIE(buff == NULL, "buff malloc failed\n");
    get_name(buff);
    // read definition (into a buffer)
    char* key = malloc(MAX_WORD_SIZE * sizeof(char));
    DIE(key == NULL, "key malloc failed\n");
    scanf("%s", key);

    if (!ht_has_key(library, buff)) {
        printf("The book is not in the library.\n");
        free(buff);
        free(key);
        return;
    }

    // get the book
    book_t* book = ht_get(library, buff);

    if (!ht_has_key(book->content, key)) {
        printf("The definition is not in the book.\n");
        free(buff);
        free(key);
        return;
    }
    // remove given definition
    ht_remove_entry(book->content, key);
    // free remaining buffers
    free(buff);
    free(key);
}
