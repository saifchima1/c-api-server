flags = -Wall -Wextra

all: compile

compile: ./src/main.o
	@gcc $(flags) ./src/main.o -o server
main: ./src/ ./src/main.c
	@gcc $(flags) ./src/main.c -c

clear:
	@rm ./*.o
