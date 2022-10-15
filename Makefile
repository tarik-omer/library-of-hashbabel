# Copyright 2021 - 2022 Omer Tarik Ilhan 314CA
CFLAGS=-g -m32 -Wall -Wextra -std=c99
FILES=tema2.c structs.h library_commands.c hashtable_base.c linked_list_base.c utils.h user_commands.c functions.c shut_down.c

build:
	gcc $(CFLAGS) $(FILES) -o main

run:
	valgrind --leak-check=full --show-leak-kinds=all ./main

clean:
	rm main
