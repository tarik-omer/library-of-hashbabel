// Copyright 2021 - 2022 Omer Tarik Ilhan 314CA
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "./utils.h"
#include "./linked_list_base.h"
#include "./structs.h"

int
compare_function_strings(void *a, void *b)
{
	char *str_a = (char *)a;
	char *str_b = (char *)b;

	return strcmp(str_a, str_b);
}

unsigned int
hash_function_string(void *a)
{
	/*
	 * Credits: http://www.cse.yorku.ca/~oz/hash.html
	 */
	unsigned char *puchar_a = (unsigned char*) a;
	int64_t hash = 5381;
	int c;

	while ((c = *puchar_a++))
		hash = ((hash << 5u) + hash) + c; /* hash * 33 + c */

	return hash;
}

hashtable_t *ht_create(unsigned int hmax, unsigned int (*hash_function)(void*),
					   int (*compare_function)(void*, void*))
{
	hashtable_t* hashtable = malloc(sizeof(hashtable_t));
	DIE(hashtable == NULL, "ht malloc failed\n");
	hashtable->buckets = malloc(sizeof(linked_list_t*) * hmax);
	DIE(hashtable->buckets == NULL, "buckets malloc failed\n");

	for (unsigned int i = 0; i < hmax; i++) {
		hashtable->buckets[i] = ll_create(sizeof(info));
	}

	hashtable->hmax = hmax;
	hashtable->size = 0;
	hashtable->compare_function = compare_function;
	hashtable->hash_function = hash_function;

	return hashtable;
}

int
ht_has_key(hashtable_t *ht, void *key)
{
	int index = ht->hash_function(key) % ht->hmax;

	linked_list_t* bucket = ht->buckets[index];

	if (bucket->size == 0) {
		return 0;
	} else {
		ll_node_t* entry = bucket->head;
		for (unsigned int i = 0; i < bucket->size; i++) {
			info* inf = (info*)entry->data;

			if (!(ht->compare_function(inf->key, key)))
				return 1;

			entry = entry->next;
		}
	}
	return 0;
}

void *
ht_get(hashtable_t *ht, void *key)
{
	int index = ht->hash_function(key) % ht->hmax;

	linked_list_t* bucket = ht->buckets[index];
	ll_node_t* node = bucket->head;

	if (bucket->size == 0) {
		// case: empty bucket
		return NULL;
	} else {
		// go through all the bucket entries and compare keys
		void* searched_key = ((info*)(node->data))->key;
		for (unsigned int i = 0; i < bucket->size; i++) {
			if (ht->compare_function(key, searched_key) == 0) {
				info* inf = (info*)node->data;
				return inf->value;
			}
			node = node->next;
			searched_key = ((info*)(node->data))->key;
		}
	}
	// last return NULL that takes care of the case in which
	// the key has no value
	return NULL;
}

void
ht_put(hashtable_t *ht, void *key, unsigned int key_size,
	void *value, unsigned int value_size)
{
	int index = ht->hash_function(key) % ht->hmax;

	linked_list_t* bucket = ht->buckets[index];

	ll_node_t* entry = bucket->head;
	// remove old entry (if there is one)
	for (unsigned int i = 0; i < bucket->size; i++) {
		info* inf = entry->data;

		if (!(ht->compare_function(key, inf->key))) {
			ll_node_t* to_remove = ll_remove_nth_node(bucket, i);
			info* to_remove_info = (info*)to_remove->data;
			free(to_remove_info->key);
			free(to_remove_info->value);
			free(to_remove->data);
			free(to_remove);
			break;
		}

		entry = entry->next;
	}
	// copy the key / value pair to an info struct
	info* new_inf = malloc(sizeof(info));
	DIE(new_inf == NULL, "info malloc failed\n");

	new_inf->key = malloc(key_size);
	DIE(new_inf->key == NULL, "info key malloc failed\n");

	new_inf->value = malloc(value_size);
	DIE(new_inf->value == NULL, "info value malloc failed\n");

	memcpy(new_inf->key, key, key_size);
	memcpy(new_inf->value, value, value_size);

	// add the info to the bucket (list)
	ll_add_nth_node(bucket, bucket->size, new_inf);
	free(new_inf);
	ht->size++;
}

void
ht_remove_entry(hashtable_t *ht, void *key)
{
	int index = ht->hash_function(key) % ht->hmax;
	linked_list_t* bucket = ht->buckets[index];

	ll_node_t* node = bucket->head;
	// go through the bucket until we find the correct key
	for (unsigned int i = 0; i < bucket->size; i++) {
		void* searched_key = ((info*)(node->data))->key;
		if (ht->compare_function(key, searched_key) == 0) {
			// when we find it, we remove it
			node = ll_remove_nth_node(ht->buckets[index], i);
			free(((info*)(node->data))->key);
			free(((info*)(node->data))->value);
			free(node->data);
			free(node);
			break;
		}
		node = node->next;
	}
	ht->size--;
}

void
ht_free(hashtable_t *ht)
{
	// iterate through the buckets and remove each entry
	for (unsigned int i = 0; i < ht->hmax; i++) {
		ll_node_t* curr;
		while (ht->buckets[i]->head) {
			// remove each entry and free (surface) memory
			curr = ht->buckets[i]->head;
			ht->buckets[i]->head = ht->buckets[i]->head->next;
			free(((info*)(curr->data))->value);
			free(((info*)(curr->data))->key);
			free(curr->data);
			free(curr);
		}
		free(ht->buckets[i]);
	}
	// free buckets array and the hashtable itself
	free(ht->buckets);
	free(ht);
}

unsigned int
ht_get_size(hashtable_t *ht)
{
	if (ht == NULL)
		return 0;

	return ht->size;
}

unsigned int
ht_get_hmax(hashtable_t *ht)
{
	if (ht == NULL)
		return 0;

	return ht->hmax;
}
