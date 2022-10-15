// Copyright 2021 - 2022 Omer Tarik Ilhan 314CA
#ifndef STRUCTS_H_
#define STRUCTS_H_

#define HMAX 5
#define MAX_STRING_SIZE 64
#define MAX_NAME_SIZE 40
#define MAX_WORD_SIZE 20

// node structure for linked list
typedef struct ll_node_t
{
    void* data;
    struct ll_node_t* next;
} ll_node_t;

// linked list structure
typedef struct linked_list_t
{
    ll_node_t* head;
    unsigned int data_size;
    unsigned int size;
} linked_list_t;

// info structure for hashtable entry: key - value pair
typedef struct info {
	void *key;
	void *value;
} info;

// hashtable structure
typedef struct hashtable_t {
	// linked list array (bucket array)
	linked_list_t **buckets;
	// number of entries
	unsigned int size;
	// number of buckets
	unsigned int hmax;
	// pointer to a function that hashes given key
	unsigned int (*hash_function)(void*);
	// pointer to a function that compares 2 keys
	int (*compare_function)(void*, void*);
} hashtable_t;

// borrow structure - data about a borrowing
typedef struct borrow_t {
	// whether a user has or has not a book borrowed
	char has_borrowed;
	// name of the borrowed book
	char* borrowed_book;
	// borrow time
	int time;
} borrow_t;

// book structure - data about a book
typedef struct book_t {
	// book name (may contain spaces)
	char* name;
	// book content - hashtable with word - definition pairs
	hashtable_t* content;
	// book rating
	double rating;
	// sum of all ratings so far (so we can recalculate rating every time)
	int rating_sum;
	// number of purchases (borrows)
	int purchases;
	// whether the book is reachable or not (not borrowed / borrowed)
	char unreachable;
} book_t;

// user structure - data about a user
typedef struct user_t {
	// user name
	char* name;
	// user points
	int points;
	// whether the user is or isn't banned
	char is_banned;
	// data about user's borrowed book (if there is one)
	borrow_t* borrowing;
} user_t;

#endif  // STRUCTS_H_
