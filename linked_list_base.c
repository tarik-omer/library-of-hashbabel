// Copyright 2021 - 2022 Omer Tarik Ilhan 314CA
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./structs.h"
#include "./utils.h"

linked_list_t *
ll_create(unsigned int data_size)
{
    linked_list_t* ll;
    // malloc list
    ll = malloc(sizeof(linked_list_t));
    DIE(ll == NULL, "linked_list malloc failed");

    // initialize list
    ll->head = NULL;
    ll->data_size = data_size;
    ll->size = 0;

    return ll;
}

void
ll_add_nth_node(linked_list_t* list, unsigned int n, const void* new_data)
{
    ll_node_t *prev, *curr;
    ll_node_t* new_node;

    if (!list) {
        return;
    }
    // truncate index if needed
    if (n > list->size) {
        n = list->size;
    }

    // iterate until desired index
    curr = list->head;
    prev = NULL;
    while (n > 0) {
        prev = curr;
        curr = curr->next;
        --n;
    }

    // malloc node and node->data and copy given data to the new node
    new_node = malloc(sizeof(*new_node));
    DIE(new_node == NULL, "new_node malloc failed");
    new_node->data = malloc(list->data_size);
    DIE(new_node->data == NULL, "new_node->data malloc failed");
    memcpy(new_node->data, new_data, list->data_size);

    // insert node
    new_node->next = curr;
    if (prev == NULL) {
        list->head = new_node;
    } else {
        prev->next = new_node;
    }

    list->size++;
}

ll_node_t *
ll_remove_nth_node(linked_list_t* list, unsigned int n)
{
    ll_node_t *prev, *curr;
    // empty or bad list
    if (!list || !list->head) {
        return NULL;
    }
    // trucate the index to remove at
    if (n > list->size - 1) {
        n = list->size - 1;
    }

    curr = list->head;
    prev = NULL;
    // iterate to the desired node
    while (n > 0) {
        prev = curr;
        curr = curr->next;
        --n;
    }
    // remake the links in order to remove the node
    if (prev == NULL) {
        list->head = curr->next;
    } else {
        prev->next = curr->next;
    }

    list->size--;
    // return the removed node
    return curr;
}

unsigned int
ll_get_size(linked_list_t* list)
{
     if (!list) {
        return -1;
    }

    return list->size;
}

void
ll_free(linked_list_t** pp_list)
{
    ll_node_t* currNode;

    if (!pp_list || !*pp_list) {
        return;
    }
    // iterate through the list, removing & freeing one node at a time
    while (ll_get_size(*pp_list) > 0) {
        currNode = ll_remove_nth_node(*pp_list, 0);
        free(currNode->data);
        currNode->data = NULL;
        free(currNode);
        currNode = NULL;
    }
    // free the list structure itself
    free(*pp_list);
    *pp_list = NULL;
}
