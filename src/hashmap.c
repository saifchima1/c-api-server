//=========================[saifchima's hashmap source
// code]================================== including some needed built in
// librarys including the header file:
#include "hashmap.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// global variables:
bool safe_mode = true;

// hashing function:
size_t hashing(size_t size, char *key) {
  size_t result = 1;
  for (size_t i = 1; i <= strlen(key); i++) {
    result =
        (result + i * 67 / (5 * key[i - 1] * 4 + size * 7)) + strlen(key) * 67;
  }
  return result % size;
}

// this is a function to toggle safe_mode which addes a buffer to reduce chanse
// of chaining:
void safemodeswitch(bool choice) { safe_mode = choice; }

// this function initialize the hashmap by allocating it and the nodes:
hashmap *inithash(size_t size) {
  float buffer = 1;
  if (safe_mode) {
    buffer = 1.35;
  }
  hashmap *map = malloc(sizeof(hashmap));
  map->size = size;
  map->nodes = calloc((size_t)(size * buffer), sizeof(hnode *));
  return map;
}

// this function addes whatever value given and save it in a node with key with
// chain handling:
void appendhash(hashmap *dict, char *key, void *value, size_t size_element) {
  float buffer = (safe_mode) ? 1.35 : 1;
  size_t index = hashing((size_t)(dict->size * buffer), key);
  hnode *thenode = dict->nodes[index];
  hnode *appended = malloc(sizeof(hnode));
  appended->element.value = malloc(size_element);
  memcpy(appended->element.value, value, size_element);
  appended->key = malloc(strlen(key) + 1);
  strcpy(appended->key, key);
  appended->next = NULL;
  appended->element.size = size_element;
  if (thenode != NULL) {
    hnode *currentnode = thenode;
    hnode *lastnode = dict->nodes[index];
    bool firstnode = true;
    while (currentnode != NULL) {
      if (strcmp(currentnode->key, key) == 0) {
        appended->next = currentnode->next;
        free(currentnode->element.value);
        free(currentnode->key);
        free(currentnode);
        if (firstnode) {
          dict->nodes[index] = appended;
          return;
        }

        lastnode->next = appended;
        return;
      }
      lastnode = currentnode;
      currentnode = currentnode->next;
      if (firstnode) {
        firstnode = false;
      }
    }
    lastnode->next = appended;
    return;
  }
  dict->nodes[index] = appended;
}

// this function calls a value by it's key:
helement callvalue(hashmap *dict, char *key) {
  float buffer = (safe_mode) ? 1.35 : 1;
  size_t index = hashing((size_t)(dict->size * buffer), key);
  hnode *thenode = dict->nodes[index];
  while (thenode != NULL) {
    if (strcmp(thenode->key, key) == 0) {
      return thenode->element;
    }
    thenode = thenode->next;
  }

  fprintf(stderr,
          "Error: The key \"%s\" doesn't exist in the hashmap. Exiting...\n",
          key);
  exit(EXIT_FAILURE);
}

// this function handles chaining to simplifie the next function:
void handlechain(hnode *destination, hnode *source) {
  if (destination->next != NULL) {
    source->next = destination->next;
  } else {
    source->next = NULL;
  }
  destination->next = source;
}

// remap function handles dynamicly resizing the array and rehashing every node:
void remap(hashmap *dict, size_t size) {
  float buffer = 1;
  if (safe_mode) {
    buffer = 1.35;
  }
  hnode **new_allocation = calloc((size_t)(size * buffer), sizeof(hnode *));
  hnode *sourcenode;
  hnode *backup;
  for (int i = 0; i < (size_t)(dict->size * buffer); i++) {
    sourcenode = dict->nodes[i];
    while (sourcenode != NULL) {
      backup = sourcenode->next;
      sourcenode->next = NULL;
      if (new_allocation[hashing((size_t)(size * buffer), sourcenode->key)] !=
          NULL) {
        handlechain(
            new_allocation[hashing((size_t)(size * buffer), sourcenode->key)],
            sourcenode);
      } else {
        new_allocation[hashing((size_t)(size * buffer), sourcenode->key)] =
            sourcenode;
      }
      sourcenode = backup;
    }
  }
  dict->size = size;
  free(dict->nodes);
  dict->nodes = new_allocation;
}
// this function frees the map and all it's nodes by iterating through the
// absolute root of the data structer:
void freemap(hashmap *dict) {
  float buffer = (safe_mode) ? 1.35 : 1;
  hnode *thenode;
  hnode *lastnode;
  for (int i = 0; i < (size_t)(dict->size * buffer); i++) {
    thenode = dict->nodes[i];
    while (thenode != NULL) {
      lastnode = thenode;
      free(thenode->element.value);
      free(thenode->key);
      thenode = thenode->next;
      free(lastnode);
    }
  }
  free(dict->nodes);
  free(dict);
}
// this function removes a value safly by freeing it and reconnect the rest of
// the nodes if it is chained:
void removevalue(hashmap *dict, char *key) {
  float buffer = (safe_mode) ? 1.35 : 1;
  size_t index = hashing((size_t)dict->size * buffer, key);
  hnode *thenode = dict->nodes[index];
  hnode *lastnode;
  bool firstnode = true;
  while (thenode != NULL) {
    if (firstnode) {
      firstnode = false;
      if (strcmp(thenode->key, key) == 0) {
        dict->nodes[index] = thenode->next;
        free(thenode->element.value);
        free(thenode->key);
        free(thenode);
        return;
      }
      lastnode = thenode;
      thenode = thenode->next;
      continue;
    }
    if (strcmp(thenode->key, key) == 0) {
      if (thenode->next != NULL) {
        lastnode->next = thenode->next;
        free(thenode->element.value);
        free(thenode->key);
        free(thenode);
      } else {
        lastnode->next = NULL;
        free(thenode->element.value);
        free(thenode->key);
        free(thenode);
      }
      return;
    }
    lastnode = thenode;
    thenode = thenode->next;
  }
  printf("the value of key\"%s\" doesn't exist in this hashmap", key);
}

// this function iterates through the hashmap to find if the key exists:
bool doeskeyexist(hashmap *dict, char *key, long int *index, long int *depth) {
  float buffer = (safe_mode) ? 1.35 : 1;
  size_t thisindex = hashing((size_t)dict->size * buffer, key);
  hnode *thenode = dict->nodes[thisindex];
  size_t i = 0;
  while (thenode != NULL) {
    if (strcmp(thenode->key, key) == 0) {
      *index = thisindex;
      *depth = i;
      return true;
    }
    thenode = thenode->next;
    i++;
  }
  *index = -1;
  *depth = -1;
  return false;
}

// this value prints the current shape of the hashmap:
void printhashmapstatus(hashmap *dict) {
  float buffer = (safe_mode) ? 1.35 : 1;
  hnode *thenode;
  size_t thisdepth;
  printf("=========================[hashmap]=========================\n");
  for (size_t i = 0; i < (size_t)dict->size * buffer; i++) {
    thisdepth = 0;
    thenode = dict->nodes[i];
    printf("index[%ld]:", i);
    while (thenode != NULL) {
      printf("node[%s ,%ld]|", thenode->key, thisdepth);
      thenode = thenode->next;
      thisdepth++;
    }
    printf("\n");
  }
}

// this function returns a data structer that has every detail needed of the
// hashmap:
status *hashmapstatus(hashmap *dict) {
  float buffer = (safe_mode) ? 1.35 : 1;
  hnode *thenode;
  size_t thisdepth;
  status *thisstatus = malloc(sizeof(status));
  thisstatus->nodes =
      malloc(sizeof(struct hbucketinfo) * ((size_t)(dict->size * buffer)));
  struct hdepth *temp;
  size_t arraysize = 1;
  thisstatus->size = (size_t)dict->size * buffer;
  for (size_t i = 0; i < (size_t)dict->size * buffer; i++) {
    thisdepth = 0;
    arraysize = 1;
    thenode = dict->nodes[i];
    thisstatus->nodes[i].data = malloc(sizeof(struct hdepth));
    while (thenode != NULL) {
      if (arraysize <= thisdepth + 1) {
        arraysize *= 2;
        temp = realloc(thisstatus->nodes[i].data,
                       arraysize * sizeof(struct hdepth));
        thisstatus->nodes[i].data = temp;
      }
      thisstatus->nodes[i].data[thisdepth] =
          (struct hdepth){thenode->key, thisdepth};
      thenode = thenode->next;
      thisdepth++;
    }
    if (thisdepth == 0) {
      free(thisstatus->nodes[i].data);
      thisstatus->nodes[i].data = NULL;
      thisstatus->nodes[i].size = 0;
    } else {
      temp =
          realloc(thisstatus->nodes[i].data, thisdepth * sizeof(struct hdepth));
      thisstatus->nodes[i].data = temp;
      thisstatus->nodes[i].size = thisdepth;
    }
  }
  return thisstatus;
}

// this function frees the status data type by iterating through the absolute
// root all the way to the status struct:
void freestatus(status *thestatus) {
  struct hbucketinfo thebucket;
  for (size_t i = 0; i < thestatus->size; i++) {
    free(thestatus->nodes[i].data);
    thestatus->nodes[i].data = NULL;
  }
  free(thestatus->nodes);
  thestatus->nodes = NULL;
  free(thestatus);
  thestatus = NULL;
}
// more functions are comming soon :)
