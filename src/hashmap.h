#ifndef HASHMAP_H_
#define HASHMAP_H_

#include <stdbool.h>
#include <stddef.h>

typedef struct {
  void *value;
  size_t size;
} helement;

typedef struct hnode {
  helement element;
  char *key;
  struct hnode *next;
} hnode;

typedef struct {
  hnode **nodes;
  size_t size;
} hashmap;

struct hdepth {
  const char *key;
  size_t index;
};

struct hbucketinfo {
  size_t size;
  struct hdepth *data;
};

typedef struct {
  size_t size;
  struct hbucketinfo *nodes;
} status;

void safemodeswitch(bool choice);

hashmap *inithash(size_t size);

void appendhash(hashmap *dict, char *key, void *value, size_t size_element);

helement callvalue(hashmap *dict, char *key);

void remap(hashmap *dict, size_t size);

void freemap(hashmap *dict);

void removevalue(hashmap *dict, char *key);

bool doeskeyexist(hashmap *dict, char *key, long int *index, long int *depth);

void printhashmapstatus(hashmap *dict);

status *hashmapstatus(hashmap *dict);

void freestatus(status *thestatus);

#endif
