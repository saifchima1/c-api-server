flags = -Wall -Wextra -g

all: compile

compile: ./src/main.o
	@gcc $(flags) ./*.o -o server
main: ./src/ ./src/main.c
	@gcc $(flags) ./src/main.c -c
handler: ./src/handler.c
	@gcc $(flags) ./src/handler.c -c
hashmap: ./src/hashmap.c
	@gcc $(flags) ./src/hashmap.c -c
functions: ./src/functions.c
	@gcc $(flags) ./src/functions.c -c
fullsurce: ./src/main.c
	@gcc $(flags) ./src/*.c -c
clear:
	@rm ./*.o
