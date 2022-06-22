#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define max_levels 1000

#include "skiplist.h"

skiplist* skiplist_init(skiplist* list){

    node* header = (node*)malloc(sizeof(node));
    header->key = (char*)malloc(sizeof(char) * 60);
    list->header = header;

    strcpy(header->key, "3000000");                        //initializing the header key with something very big to be used in comparisons 
    header->forward = (node**)malloc(sizeof(node*) * (max_levels + 1));

    for(int i = 0; i < max_levels + 1 ; i++)
        header->forward[i] = list->header;                  //for every level the header node will point in the list header              

    list->level = 0;                                        //set as level 0 the first level

    return list;                                            

}

int random_lvl(){
    
    int lvl = 0;
    
    while(rand() < RAND_MAX/2 && lvl < max_levels)          
        lvl++;
    
    return lvl;

}                                                          //used for calculating a random level for each key thats inserted in skiplist


int skiplist_insert(skiplist* list, char* key){
    
    node *new[max_levels + 1];
    node *temp = list->header;

    for(int i = list->level; i >= 0; i--){
        
        while(atoi(temp->forward[i]->key) < atoi(key))                      //find the key that's smaller than the key we want
            temp = temp->forward[i];                                        //save its postition
        new[i] = temp;                                                      //save the key in the next position
    }
    temp = temp->forward[0];                                                //we take that position of smaller key placed on level 0 and take the next node that must be the key we want

    int level;
    
    if(atoi(temp->key) == atoi(key))                                         //if the key is found return, no need to insert
        return 0;
    else{                                                                   //if key not found then we insert
        level = random_lvl();                                               //calculate random level

        if(level > list->level){
            for(int i = list->level + 1; i <= level; i++){
                new[i] = list->header;                                      //for every new level update the list header 
            }
            list->level = level;
        }

        temp = (node*)malloc(sizeof(node));
        temp->key = (char*)malloc(sizeof(char) * 60);
        strcpy(temp->key, key);
        temp->forward = (node**)malloc(sizeof(node*) * (level + 1));       //create new node with the key we want to insert

        for(int i = 0; i < level + 1; i++){
            temp->forward[i] = new[i]->forward[i];
            new[i]->forward[i] = temp;                                    //put new node in the correct position in every level
        }
    }

    return 0;

} 


int skiplist_search(skiplist* list, char* key){

    node* temp = list->header;
    
    char* key_check = (char*)malloc(sizeof(char) * 60);

    for(int i = list->level; i >= 0; i--){
        while(atoi(temp->forward[i]->key) < atoi(key))                  //find the key that's smaller than the key we want
            temp = temp->forward[i];                                    //save its postition       
        strcpy(key_check, temp->forward[i]->key);                       //save the key in the next position
    }

    if(atoi(key_check) == atoi(key)){                                     //if the key in the next position is equal to the one we want
        free(key_check);
        return 1;                                                       //our key was found and return 1 in main function
    }
    else{ 
        free(key_check);   
        return 0;                                                       //if not then return 0 in main function
    }

}


void skiplist_delete(skiplist* list, char* key){
    
    node *snode[max_levels + 1];
    node *temp = list->header;

    for(int i = list->level; i >= 0; i--){
        while(atoi(temp->forward[i]->key) < atoi(key))
            temp = temp->forward[i];
        snode[i] = temp;
    }                                                         //position of smaller key found
    temp = temp->forward[0];                                  //we take that position of smaller key placed on level 0 and take the next node that must be the key we want

    if(atoi(temp->key) == atoi(key)){                         //if its the key we want continue, but if its not the key, it means it doesn't exist
        for(int i = 0; i <= list->level; i++){               //start from level zero
            if(snode[i]->forward[i] != temp)                 //reach the level where the key doesn't exist and break
                break;
            snode[i]->forward[i] = temp->forward[i];         //fix previous and next pointer of the key
        }

        if(temp != NULL){
            free(temp->key);
            free(temp->forward);
            free(temp);
        }

        while(list->level > 0 && list->header->forward[list->level] == list->header)            //in case we delete a key that its the only key in the level
            list->level--;                                                                      //we decrease the counter of levels
    }

}


void skiplist_free(skiplist* list){   

    node* temp = list->header->forward[0];
    
    while(temp != list->header) {
        node* next = temp->forward[0];
        free(temp->key);
        free(temp->forward);
        free(temp);
        temp = next;
    }
    
    free(list->header->key);
    free(list->header->forward);
    free(list->header);
    free(list);

}


void print_list(skiplist* list){
    
    printf("\n_____Skip List_____\n"); 

    for (int i = list->level; i >= 0; i--){ 
        node* temp = list->header->forward[i]; 
        printf("Level %d: ", i); 
        
        while (temp != list->header){ 
            printf("%s -> ", temp->key); 
            temp = temp->forward[i]; 
        } 
        printf("NIL\n"); 
    } 

}                                                               //printing skiplist with all its levels