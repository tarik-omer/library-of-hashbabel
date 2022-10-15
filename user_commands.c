// Copyright 2021 - 2022 Omer Tarik Ilhan 314CA
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./structs.h"
#include "./hashtable_base.h"
#include "./functions.h"
#include "./utils.h"
#include "./library_commands.h"

void add_user(hashtable_t* users)
{
    // read the user name
    char* name = malloc(sizeof(char) * MAX_WORD_SIZE);
	DIE(name == NULL, "name buff malloc failed\n");
    scanf("%s", name);

    if (ht_has_key(users, name)) {
        printf("User is already registered.\n");
        free(name);
        return;
    }
    // create a new user instance
    user_t* user = malloc(sizeof(user_t));
    DIE(user == NULL, "user malloc failed\n");

    // initialize user
    user->is_banned = 0;
    user->name = malloc(strlen(name) + 1);
	DIE(user->name == NULL, "username malloc failed\n");

    user->points = 100;
    // initialize borrowing structure
    user->borrowing = malloc(sizeof(borrow_t));
    DIE(user->borrowing == NULL, "borrowing malloc failed\n");
    user->borrowing->has_borrowed = 0;
    user->borrowing->time = 0;
    user->borrowing->borrowed_book = NULL;

    memcpy(user->name, name, strlen(name) + 1);
    // place the user in the database
    ht_put(users, name, strlen(name) + 1, user, sizeof(user_t));
    // free remaining buffers
    free(name);
    free(user);
}

void return_book(hashtable_t* users, hashtable_t* library)
{
    // read user name
    char* username = malloc(sizeof(char) * MAX_WORD_SIZE);
    DIE(username == NULL, "username buff malloc failed\n");
    scanf("%s", username);
    // read book name
    char* bookname = malloc(sizeof(char) * MAX_NAME_SIZE);
    DIE(bookname == NULL, "bookname buff malloc failed\n");
    get_name(bookname);
    // read time since borrow and rating given by user
    int time_since, user_rating;
    scanf(" %d %d", &time_since, &user_rating);

    // get the user and the book
    // check if the user exists
    if (!ht_has_key(users, username)) {
        printf("You are not registered yet.\n");
        free(username);
        free(bookname);
        return;
    }
    // get the user
    user_t* user = ht_get(users, username);

    // check if the book exists
    if (!ht_has_key(library, bookname)) {
        printf("The book is not in the library.\n");
        free(username);
        free(bookname);
        return;
    }
    // get the book
    book_t* book = ht_get(library, bookname);

    // check to see if the user is banned
    if (user->is_banned == 1) {
        printf("You are banned from this library.\n");
        free(username);
        free(bookname);
        return;
    }
    // check to see if the said book is not the borrowed
    // book or the user has no borrowed book
    if (user->borrowing->has_borrowed == 0 ||
        strcmp(user->borrowing->borrowed_book, book->name)) {
        printf("You didn't borrow this book.\n");
        free(username);
        free(bookname);
        return;
    }

    // apply the score system to the user
    if (time_since > user->borrowing->time) {
        user->points = user->points - 2 * (time_since - user->borrowing->time);
    } else {
        user->points = user->points + (user->borrowing->time - time_since);
    }
    // ban the user if needed
    if (user->points < 0) {
        printf("The user %s has been banned from this library.\n", username);
        user->is_banned = 1;
    }
    // apply the rating to the book
    book->purchases += 1;
    book->rating_sum += user_rating;
    if (book->purchases)
        book->rating = (double)book->rating_sum / book->purchases;

    // make the book available and set the user as not having a borrowed book
    book->unreachable = 0;
    user->borrowing->has_borrowed = 0;
    user->borrowing->time = 0;
    free(user->borrowing->borrowed_book);
    user->borrowing->borrowed_book = NULL;

    // free remaining buffers
    free(username);
    free(bookname);
}

void lost_book(hashtable_t* users, hashtable_t* library)
{
    // read user name
    char* username = malloc(sizeof(char) * MAX_WORD_SIZE);
    DIE(username == NULL, "username buff malloc failed\n");
    scanf("%s", username);
    // read book name
    char* bookname = malloc(sizeof(char) * MAX_NAME_SIZE);
    DIE(bookname == NULL, "bookname buff malloc failed\n");
    get_name(bookname);

    // get the user and the book
    // check if the user exists
    if (!ht_has_key(users, username)) {
        printf("You are not registered yet.\n");
        free(username);
        free(bookname);
        return;
    }
    // get the user
    user_t* user = ht_get(users, username);

    // check if the book exists
    if (!ht_has_key(library, bookname)) {
        printf("The book is not in the library.\n");
        free(username);
        free(bookname);
        return;
    }

    // check to see if the user is banned
    if (user->is_banned == 1) {
        printf("You are banned from this library.\n");
        free(username);
        free(bookname);
        return;
    }

    // apply the scoring system
    user->points -= 50;

    // ban the user if needed
    if (user->points < 0) {
        printf("The user %s has been banned from this library.\n", username);
        user->is_banned = 1;
    }

    // remove the book from the library
    remove_book_at(library, bookname);

    // set the user to not having a borrowed book
    user->borrowing->has_borrowed = 0;
    user->borrowing->time = 0;
    free(user->borrowing->borrowed_book);
    user->borrowing->borrowed_book = NULL;

    // free remaining buffers
    free(username);
    free(bookname);
}

void borrow_book(hashtable_t* users, hashtable_t* library)
{
    // read book user name, book name and number of days to be borrowed
    char* username = malloc(sizeof(char) * MAX_WORD_SIZE);
    DIE(username == NULL, "username buff malloc failed\n");
    scanf("%s", username);
    char* bookname = malloc(sizeof(char) * MAX_NAME_SIZE);
    DIE(bookname == NULL, "bookname buff malloc failed\n");
    get_name(bookname);

    int time;
    scanf("%d", &time);

    // check if the user exists
    if (!ht_has_key(users, username)) {
        printf("You are not registered yet.\n");
        free(username);
        free(bookname);
        return;
    }
    // get the user
    user_t* user = ht_get(users, username);
    // check if the user is banned
    if (user->is_banned == 1) {
        printf("You are banned from this library.\n");
        free(username);
        free(bookname);
        return;
    }
    // check if the user has already borrowed a book
    if (user->borrowing->has_borrowed == 1) {
        printf("You have already borrowed a book.\n");
        free(username);
        free(bookname);
        return;
    }

    // check if the book exists
    if (!ht_has_key(library, bookname)) {
        printf("The book is not in the library.\n");
        free(username);
        free(bookname);
        return;
    }

    // get the book
    book_t* book = ht_get(library, bookname);
    // check if the book is borrowed
    if (book->unreachable == 1) {
        printf("The book is borrowed.\n");
        free(username);
        free(bookname);
        return;
    }
    // mark the borrowing of the book
    user->borrowing->borrowed_book = malloc(sizeof(char) *
                                    strlen(book->name) + 1);
    DIE(user->borrowing->borrowed_book == NULL,
        "borrowing bookname malloc failed\n");
    memcpy(user->borrowing->borrowed_book, book->name, strlen(book->name) + 1);
    user->borrowing->time = time;
    user->borrowing->has_borrowed = 1;
    book->unreachable = 1;

    // free the remaining buffers
    free(username);
    free(bookname);
}
