# The library of HashBabel - Homework 2

**Name: Omer Tarik Ilhan**

**Group: 314CA**

## Description:

*This project represent the implementation of a library using hashmaps.*

The aspects that need to be implemented consist of a book database (library),
a user database (users), librarian commands (addition & removal of books, etc)
and user commands (user registration, book borrowing, return and the unfornate
losing of a book).

For the implementation of this project, I used several structures:
* linked_list_t - generic linked list
* ll_node_t - generic linked list node
* info - generic key - value pair (used to hashtables)
* hashtable_t - generic hashmap
* book_t - structure containing details about a book: name, rating, purchases, availability and content (implemented using another hashtable)
* user_t - structure containing user details: name, points, whether he is banned, borrowing information
* borrow_t - structure containing details about a borrowing - borrowed book name and time length

The main function consists of the creation of the userbase and library. After that,
commands are read from stdin. Also, everytime a command is introduced, the load factor
of both the userbase and library are checked and if needed, the hashtable(s) are resized.

The resize function is also used whenever a book's content hashtable has a load factor
greater than 1.

A librarian can initiate several commands:
    
    * ADD_BOOK <book_name> <number_of_definitions> - inserts a book into the library with the given name, containing a given number of definitions (also read from stdin, as key - value pairs)
    
    * RMV_BOOK <book_name> - removes a book from the library
    
    * GET_BOOK <book_name> - displays information about the given book
    
    * ADD_DEF <book_name> - adds a key - value pair (word - definition) to the desired book
    
    * RMV_DEF <book_name> <def_key> - removes a definition for said book, based on the definition key (defined word)
    
    * GET_DEF <book_name> <def_key> - prints to stdin definition of given word from said book

A user can also initiate several commands:
    
    * ADD_USER <username> - adds a user to the userbase
    
    * BORROW <username> <book_name> <borrow_length> - said user borrows said book for said days - book is marked as borrowed and user is marked as having borrowed a book
    
    * LOST <username> <book_name> - said user loses a book - book is removed from the library and user loses many points
    
    * RETURN <username> <book_name> <time_since_borrow> <rating> - said user returns said book after said days, giving it a rating - user's score is modified based on whether he returned the book late / early

There is also, of course, the EXIT command that exits the program, freeing the memory, but also displaying a top of books, based on rating and purchases, and also a top of users, based on user points.

The userbase has implemented a points system, based on early / late book returnal, but also a banning system: if a user's points drop below 0, the user is banned from the library and cannot borrow any more books.

All commands' name are typed in CAPS.

## Possible improvements:
* I could have used less memory / better memory management.
* I could have made some algorithms work faster. (Top Books / Top Users)
* Better function genericity.
* Better modularization - many functions are 50% - 70% checking (user exists, book exists, etc.)

## Comments:

* Improved my C programming skills and usage of data genericity.
* Improved my pointer usage.
* Improved my understanding of how data structures incorporate or link with
other data structures. (Hashmap (userbase) linked with other hashmap (library)
which contains hashmaps (books))
* Helped me understand how a hashmap resize and rehash works in practice.

## Resources:

1. [DIE Macro] (https://ocw.cs.pub.ro/courses/so/laboratoare/resurse/die)

2. [String Hash Function] (http://www.cse.yorku.ca/~oz/hash.html)
