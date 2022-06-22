#ifndef SKIPLIST_H
#define SKIPLIST_H

typedef struct node node;
typedef struct skiplist skiplist;

struct node{
    char* key;
    node** forward;
};

struct skiplist{
    int level;
    node* header;
};

skiplist* skiplist_init(skiplist* list);

int random_lvl();

int skiplist_insert(skiplist* list, char* key);

int skiplist_search(skiplist* list, char* key);

void skiplist_delete(skiplist* list, char* key);

void skiplist_free(skiplist* list);

void print_list(skiplist* list);

#endif