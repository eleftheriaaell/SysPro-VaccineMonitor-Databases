#ifndef EXTRA_LISTS_H
#define EXTRA_LISTS_H

#include "skiplist.h"

typedef struct record record;
typedef struct virus virus;
typedef struct country country;

struct record{
    char* id;
    char* firstName;
    char* lastName;
    char* country;
    char* age;
    char* virus;
    char* yesNo; 
    char* date;
    int check;                      
    record* next;
};                                  //struct for keeping the record parts

struct virus{
    char* virus_name;
    skiplist* vaccinated_persons;
    skiplist* not_vaccinated_persons;
    virus* next;
};                                  //struct for saving skiplists for each virus

struct country{
    char* country;
    int total_YES;
    int total_NO;
    int num_YES;                    
    int to_twenty_YES;          
    int to_fourty_YES;
    int to_sixty_YES;
    int sixty_plus_YES;
    country* next;              //struct for saving the population of vaccinated and non vaccinated people in each country
};

void virus_push(virus* head, record* node);

void country_push(country* head, record* node);

void free_virus(virus* v_list);

void free_country(country* c_list);

void free_record(record* r_list);

#endif