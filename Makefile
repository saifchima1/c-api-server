flags = -Wall -Wextra

all: compile

compile: ./src/main.o
	@gcc $(flags) ./src/main.o -o server
main: ./src/ ./src/main.c
	@gcc $(flags) ./src/main.c -c
handler: ./src/handler.c
	@gcc $(flags) ./src/handler.c -c
hashmap: ./src/hashmap.c
	@gcc $(flags) ./src/hashmap.c
clear:
	@rm ./*.o
