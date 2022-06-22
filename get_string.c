#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "get_string.h"

void get_record(record* head, char* fileName){
    
    FILE *fp;

    if((fp = fopen(fileName, "r")) == NULL)                                     //fp has the address of input file 
        printf("Error opening file %s\n", fileName);

    char* buffer = NULL;
    char* Buff;
    size_t length = 0;
    char* id;
    int flag;

    head->id = (char*)malloc(sizeof(char) * 60);
    head->firstName = (char*)malloc(sizeof(char) * 60);
    head->lastName = (char*)malloc(sizeof(char) * 60);
    head->country = (char*)malloc(sizeof(char) * 60);
    head->age = (char*)malloc(sizeof(char) * 60);
    head->virus = (char*)malloc(sizeof(char) * 60);
    head->yesNo = (char*)malloc(sizeof(char) * 60);
    head->date = (char*)malloc(sizeof(char) * 60);

    strcpy(head->id, "-1");
    strcpy(head->firstName, "-1");
    strcpy(head->lastName, "-1");
    strcpy(head->country, "-1");
    strcpy(head->age, "-1");
    strcpy(head->virus, "-1");
    strcpy(head->yesNo, "-1");
    strcpy(head->date, "-1");
    head->check = 0;

    head->next = NULL;
    record* node = head;                                                        //create head of record list that its gonna be empty
    
    while((getline(&buffer, &length, fp)) != -1){                               //while to get all lines of file
        
        record* new = (record*)malloc(sizeof(record));                          //create new node
        
        Buff = buffer;                        
        
        id = (char*)malloc(sizeof(char) * 60);
        strcpy(id, "");

        while(*buffer != ' '){
            strncat(id, buffer, 1);
            *buffer++;
        }                                                                      //find the id of the line

        buffer = Buff;

        flag = 0;
        record* temp = head;
        while(temp != NULL){
            if(strcmp(temp->id, id) == 0){                                  //check if id is found again in list until now
                char* virus = (char*)malloc(sizeof(char) * 60);
                strcpy(virus, "");

                int counter = 0;
                char *buff = buffer;

                while(1){ 
                    if(*buff == ' ')
                        counter++;
                    if(counter == 5){
                        *buff++;
                        while(*buff != ' '){
                            strncat(virus, buff, 1);
                            *buff++;                                                
                        }
                        break;
                    }
                    *buff++;  
                }                                                       //find the virus for this id
                
                if(strcmp(temp->virus, virus) == 0){                    //inconsistancy meaning both id and virus are same for more than one record
                    printf("ERROR IN RECORD %s", buffer);
                    free(id); free(new); free(virus);               
                    flag = 1;
                    break;
                }

                free(virus);
            }
            temp = temp->next;
        }                                                              

        if(flag == 1)               //if inconsistansy found go to the next line without creating a new node for this id
            continue;
                
        temp = head;
        while(temp != NULL){
            char* yn = (char*)malloc(sizeof(char) * 60);
            strcpy(yn, "");

            int counter = 0;
            char* buff = buffer;
           
            while(1){ 
                if(*buff == ' ')
                    counter++;
                if(counter == 6){
                    *buff++;

                    strncat(yn, buff, 2);                           //find yes or no of record in the line
                    
                    break;
                }
                *buff++;  
            }
            
            if(strcmp(yn, "NO") == 0){
                *buff++;
                *buff++;
                *buff++;
                
                if(*buff >= '0' && *buff <= '9'){
                    printf("ERROR IN RECORD %s", buffer);           //inconsistency for no followed by date
                    free(yn); free(id); free(new);
                    flag = 2;
                    break;
                }
            }
            free(yn);
            temp = temp->next;
        }

        if(flag == 2)                       //if no is followed by date go to the next line without creating a new node for this id
            continue;

        temp = head;
        while(temp != NULL){
            if(strcmp(temp->id, id) == 0){                      //check if id is found again in list until now
                char* name = (char*)malloc(sizeof(char) * 60);
                char* surname = (char*)malloc(sizeof(char) * 60);
                strcpy(name, "");
                strcpy(surname, "");

                int counter = 0;
                char *buff = buffer;

                while(1){ 
                    if(*buff == ' ')
                        counter++;
                    if(counter == 1){
                        *buff++;
                        while(*buff != ' '){
                            strncat(name, buff, 1);             //find name of record in line
                            *buff++;
                        }
                        break;
                    }
                    *buff++;  
                }

                buff = buffer;

                counter = 0;
                while(1){ 
                    if(*buff == ' ')
                        counter++;
                    if(counter == 2){
                        *buff++;
                        while(*buff != ' '){
                            strncat(surname, buff, 1);          //find surname of record in line
                            *buff++;
                        }
                        break;
                    }
                    *buff++;  
                }
                
                if(strcmp(temp->firstName, name) != 0 || strcmp(temp->lastName, surname) != 0){                    //inconsistancy meaning same id with different name or surname
                    printf("ERROR IN RECORD %s", buffer);
                    free(name); free(surname); free(id); free(new);
                    flag = 3;
                    break;
                }

                free(name); free(surname);
            }
            temp = temp->next;
        }

        if(flag == 3)           //if the same id is found with different name or surname go to the next line without creating new node with this id
            continue;

        /* end of error checking */

        new->check = 0;

        temp = head;
        while(temp != NULL){
            if(strcmp(temp->id, id) == 0){              //if id already in list but with a consistant record
                new->check = 1;                         //put one to check so that when I calculate the population it wont be counted more than once
                break;
            }
            temp = temp->next;
        }
       
        new->id = (char*)malloc(sizeof(char) * 60); 
        strcpy(new->id, id);                           //put the id in the new node  

        free(id);                      

        new->firstName = (char*)malloc(sizeof(char) * 60);
        strcpy(new->firstName, "");

        while(*buffer != ' ')
            *buffer++;
        *buffer++;     

        while(*buffer != ' '){
            strncat(new->firstName, buffer, 1);
            *buffer++;
        }                                               //find the name and put it in the new node
          
        new->lastName = (char*)malloc(sizeof(char) * 60);
        strcpy(new->lastName, "");

        *buffer++;
        while(*buffer != ' '){
            strncat(new->lastName, buffer, 1);
            *buffer++;
        }                                               //find the surname and put it in the new node
        
        new->country = (char*)malloc(sizeof(char) * 60);
        strcpy(new->country, "");

        *buffer++;
        while(*buffer != ' '){
            strncat(new->country, buffer, 1);
            *buffer++;
        }                                               //find the country and put it in the new node

        new->age = (char*)malloc(sizeof(char) * 60);
        strcpy(new->age, "");

        *buffer++;
        while(*buffer != ' '){
            strncat(new->age, buffer, 1);
            *buffer++;
        }                                               //find the age and put it in the new node
        
        new->virus = (char*)malloc(sizeof(char) * 60);
        strcpy(new->virus, "");

        *buffer++;
        while(*buffer != ' '){
            strncat(new->virus, buffer, 1);
            *buffer++;
        }                                               //find the virus and put it in the new node
        
        new->yesNo = (char*)malloc(sizeof(char) * 60);
        strcpy(new->yesNo, "");

        *buffer++;
        strncat(new->yesNo, buffer, 2);
        
        if(strcmp(new->yesNo, "YE") == 0)
            strcpy(new->yesNo, "YES");

        new->date = (char*)malloc(sizeof(char) * 60);
        strcpy(new->date, "");

        if(strcmp(new->yesNo, "NO") == 0)
            strcpy(new->date, "-");       
        else{
            while(*buffer != ' ')
                *buffer++;          
            *buffer++;
            strncat(new->date, buffer, 10);
        }                                               //find yes or no and put it in the new node         
        
        node->next = new;
        new->next = NULL;
        node = new;                                     //put node in the correct position in list
        
        buffer = Buff;                                  //clear buffer
    }   
    
    buffer = Buff;                                  //clear buffer                              
    
    free(buffer);                                                                   
    fclose(fp); 

}


void push_record(record* head, record* node, char** Command){

    node->id = (char*)malloc(sizeof(char) * 60);
    node->firstName = (char*)malloc(sizeof(char) * 60);
    node->lastName = (char*)malloc(sizeof(char) * 60);
    node->country = (char*)malloc(sizeof(char) * 60);
    node->age = (char*)malloc(sizeof(char) * 60);
    node->virus = (char*)malloc(sizeof(char) * 60);

    strcpy(node->id, Command[1]);
    strcpy(node->firstName, Command[2]);
    strcpy(node->lastName, Command[3]);
    strcpy(node->country, Command[4]);
    strcpy(node->age, Command[5]);
    strcpy(node->virus, Command[6]);                    //creates new record when the user asks for it
    node->check = 0;
    
    record* temp = head;
    while(temp != NULL){
        if(strcmp(temp->id, node->id) == 0){
            node->check = 1;    //the first time the id is inserted is marked with zero and all the other records with the same id but different virus are marked as 1
            break;
        }
        temp = temp->next;
    }
        
}


char** get_command(char* command, int num){     

    char** array = (char**)malloc(sizeof(char*) * (num + 1) * 60);

    for(int i = 0; i < num; i++)
        array[i] = (char*)malloc(sizeof(char) * 60);

    char* word;

    for(int i = 0; i < num - 1; i++){
        word = (char*)malloc(sizeof(command) * 60);
        strcpy(word, "");

        while(*command != ' '){
            strncat(word, command, 1);
            *command++;
        }
        *command++;
        strcpy(array[i], word);                         //saves all the parts of the command in the array
        
        free(word);
    }                                                               

    word = (char*)malloc(sizeof(command) * 60);
    strcpy(word, "");

    while(*command != '\n'){
        strncat(word, command, 1);
        *command++;
    }
    strcpy(array[num - 1], word);                   //saves the last part of the command in the last position of the array                                

    free(word);
    
    return array;

}


char** get_date(char* date){

    char** array = (char**)malloc(sizeof(char*) * 60);

    for(int i = 0; i < 3; i++)
        array[i] = (char*)malloc(sizeof(char) * 60);

    char* word;

    for(int i = 0; i < 2; i++){
        word = (char*)malloc(sizeof(date) * 60);
        strcpy(word, "");

        while(*date != '-'){
            strncat(word, date, 1);
            *date++;
        }
        *date++;
        strcpy(array[i], word);                                 //saves day and month in array
        
        free(word);
    }

    strcpy(array[2], "");
    strncat(array[2], date, 4);                                 //then saves the year
        
    return array;

}