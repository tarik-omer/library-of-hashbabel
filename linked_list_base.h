// Copyright 2021 - 2022 Omer Tarik Ilhan 314CA
#ifndef LINKED_LIST_BASE_H_
#define LINKED_LIST_BASE_H_

#include "./structs.h"

linked_list_t *ll_create(unsigned int data_size);
void ll_add_nth_node(linked_list_t* list,
                     unsigned int n, const void* new_data);
ll_node_t *ll_remove_nth_node(linked_list_t* list, unsigned int n);
unsigned int ll_get_size(linked_list_t* list);
void ll_free(linked_list_t** pp_list);

#endif  // LINKED_LIST_BASE_H_
