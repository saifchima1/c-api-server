#include "defin.h"
#include "hashmap.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define AMOUNT 5   // amount of functions
#define ARGMAX 255 // max amount of arguments
#define ARGBUFF 54 // arguments max length

// initialisation handy structs
typedef enum { nutral, save, load } tags;
typedef char *(*functions)(int count, char args[][ARGBUFF]);
struct fundata {
  functions function;
  tags tag;
  char *key;
};

// the handler
char *handler(const char *request, size_t size) {
  // initialising the hashmap
  hashmap *funmap = functionmapinit();
  printhashmapstatus(funmap);
  // parsing the request
  //  parsing data
  const char *tail = request;
  const char *head = NULL;
  char command[54] = {0};
  char args[ARGMAX][ARGBUFF] = {0};
  bool ignore = false;
  int offset = -1;
  int length = -1;
  int amountcopied = 0;
  bool copyready = false;
  // parsing loop
  for (size_t i = 0; i < size + 1; i++) {
    char current = request[i];
    if (current == ' ' || current == '\0') {
      ignore = true;
      offset++;
      length = -1;
    } else {
      length++;
      offset = -1;
      ignore = false;
    }
    if (!ignore && length == 0) {
      tail = request + i;
      copyready = false;
    } else if (ignore && offset == 0) {
      head = request + i;
      copyready = true;
    }
    if (copyready) {
      copyready = false;
      if (amountcopied == 0) {
        strncpy(command, tail, head - tail);
      } else {
        strncpy(args[amountcopied - 1], tail, head - tail);
      }
      amountcopied++;
    }
  }
  args[amountcopied - 1][strlen(args[amountcopied - 1]) - 2] = '\0';
  // for debuggin could be removed
  printf("%s:\n", command);
  for (int i = 0; i < amountcopied - 1; i++) {
    printf(" %s,", args[i]);
  }
  printf("\n");
  // end of debuging block
  // here we call the function with the arguments needed
  struct fundata *functiondata = callvalue(funmap, command).value;
  if (!functiondata) {
    printf("the command \"%s\" is unknown\n", command);
    freemap(funmap);
    return NULL;
  }
  functions calledfunction = functiondata->function;
  if (!calledfunction) {
    printf("there is somthing wrong with the function pointer \"%s\"\n",
           functiondata->key);
    freemap(funmap);
    return NULL;
  }
  char *result = calledfunction(amountcopied - 1, args);
  freemap(funmap);
  return result;
}
