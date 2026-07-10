#include "hashmap.h"
#include <__stdarg_va_list.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define AMOUNT 5

// initialisation
typedef enum { nutral, save, load } keys;
typedef char *(*functions)(int count, ...);
struct fundata {
  functions function;
  keys key;
};

// functions definition
char *initprofile(int count, ...) {
  if (count < 2) {
    perror("error: too few arguments\n");
    return NULL;
  } else if (count > 2) {
    perror("error: too much arguments\n");
    return NULL;
  }
  va_list args;
  va_start(args, count);
  FILE *pdatabase = fopen("../data.json", "a");
  if (!pdatabase) {
    perror("error: failed to save the file\n");
    return NULL;
  }
  fprintf(pdatabase, "%s : {\n%s}", va_arg(args, char *), va_arg(args, char *));

  if (fclose(pdatabase) != 0) {
    perror("error: could not close the file safely\npress enter to continue "
           "...\n");
    getchar();
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
      ;
  }
  char *result = calloc(30, 1);
  sprintf(result, "data store is a success\n");
  return result;
}

// the handler
char *handler(const char *request, size_t size) {
  // initialising the hashmap
  hashmap *funmap = inithash(AMOUNT);
  appendhash(funmap, "initprofile", initprofile, 1);
  // parsing data
  const char *tail = request;
  const char *head = NULL;
  char command[54] = {0};
  char args[255][54] = {0};
  bool ignore = false;
  int offset = -1;
  int length = -1;
  int amountcopied = 0;
  bool copyready = false;
  // parsing loop
  for (size_t i = 0; i < size; i++) {
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
      head = request + i - 1;
      copyready = true;
    }
    if (copyready) {
      copyready = false;
      if (amountcopied == 0) {
        strncpy(command, tail, head - tail);
      } else {
        strncpy(args[amountcopied], tail, head - tail);
      }
      amountcopied++;
    }
  }
  callvalue(funmap, command)(amountcopied, )
}
