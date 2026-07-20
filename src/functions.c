#include "hashmap.h"
#include "sqlite3.h"
#include "sqlite3ext.h"
#include "defin.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define AMOUNT 5 // amount of functions "change it"
#define ARGMAX 255 // max amount of arguments "reccomended not to change only if needed"
#define ARGBUFF 54 // arguments max length "reccomended not to change only if needed"

//handy structs initialisation
typedef enum { nutral, save, load } tags;
typedef char *(*functions)(int count, char args[][ARGBUFF]);
struct fundata {
  functions function;
  tags tag;
  char* key;
};

// creat ur functions here expected format char * foo(int count,char args[][ARGBUFF])
char *initprofile(int count, char args[][ARGBUFF]) {
  if (count < 2) {
    perror("error: too few arguments\n");
    return NULL;
  } else if (count > 2) {
    perror("error: too much arguments\n");
    return NULL;
  }
  FILE *pdatabase = fopen("data.json", "a");
  if (!pdatabase) {
    perror("database now found");
    return NULL;
  }
  fprintf(pdatabase, "%s : {\n%s}", args[0], args[1]);
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

//here we put the function pointers and meta data
hashmap *functionmapinit() {
  //prepare teh data
  struct fundata initprofiledata = {initprofile,save,"initprofile"};

  //creat the hashmap
  hashmap* funmap = inithash(AMOUNT);
  //put data in the hashmap
  appendhash(funmap, initprofiledata.key, &initprofiledata, sizeof initprofiledata);
  
  //we return the hashmap
  return funmap;
}
