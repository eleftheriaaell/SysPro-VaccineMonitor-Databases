#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "extra_lists.h"

void virus_push(virus* head, record* node){

    head->virus_name = (char*)malloc(sizeof(char) * 60);
    strcpy(head->virus_name, node->virus);                                          //sets name for the virus of the node
    
    head->vaccinated_persons = (skiplist*)malloc(sizeof(skiplist));
    head->vaccinated_persons = skiplist_init(head->vaccinated_persons);             //initializes the vaccinated skiplist for the virus of the node

    head->not_vaccinated_persons = (skiplist*)malloc(sizeof(skiplist));
    head->not_vaccinated_persons = skiplist_init(head->not_vaccinated_persons);     //initializes the non vaccinated skiplist for the virus of the node
    
}

void country_push(country* head, record* node){

    int sum_YES = 0, sum_NO = 0;
    
    while(node != NULL){
        if(strcmp(head->country, node->country) == 0){                      //if the country is the country given in main 
            if(strcmp(node->yesNo, "YES") == 0 && node->check == 0)         //and if the person of node given is vaccinated 
                sum_YES++;                                                  //increase total of vaccinated people in country
            else if(strcmp(node->yesNo, "NO") == 0 && node->check == 0)     //or if the person of node given is not vaccinated
                sum_NO++;                                                   //increase total of not vaccinated people in country
        }
        node = node->next;
    }

    head->total_YES = sum_YES;
    head->total_NO = sum_NO;                                                //set the totals
    head->num_YES = 0;                          
    head->to_twenty_YES = 0;
    head->to_fourty_YES = 0;
    head->to_sixty_YES = 0;
    head->sixty_plus_YES = 0;                                               //initialize counters that are gonna change in the main function and then reset back to zero for next calculation

}

void free_virus(virus* v_list){

    virus* temp = v_list;
    while(temp != NULL){
        v_list = temp->next;
        free(temp->virus_name);
        skiplist_free(temp->vaccinated_persons);
        skiplist_free(temp->not_vaccinated_persons);
        free(temp);
        temp = v_list;
    }

}

void free_country(country* c_list){

    country* temp = c_list;
    while(temp != NULL){
        c_list = temp->next;
        free(temp->country);
        free(temp);
        temp = c_list;
    }

}

void free_record(record* r_list){

    record* temp = r_list;
    while(temp != NULL){
        r_list = temp->next;
        free(temp->id);
        free(temp->firstName);
        free(temp->lastName);
        free(temp->country);
        free(temp->age);
        free(temp->virus);
        free(temp->yesNo);
        free(temp->date);
        free(temp);
        temp = r_list;
    }

}