#ifndef BLOOM_H
#define BLOOM_H

#include "extra_lists.h"

typedef struct bloom bloom;

struct bloom{
    char* virus;
    int *bit_array;
    bloom* next;
};

unsigned long sdbm(unsigned char *str);

unsigned long djb2(unsigned char *str);

unsigned long hash_i(unsigned char *str, unsigned int i);

void bit_change(int *bit_array, int bloomSize, unsigned char* id, unsigned int i);

void bloom_insert(bloom* bloom_head, char* id, int bloomSize);

int bloom_check(bloom* node, int bloomSize, unsigned char* id);

void bloom_push(bloom* new, record* node, int bloomSize);

void bloom_free(bloom* bloom_head);

#endif