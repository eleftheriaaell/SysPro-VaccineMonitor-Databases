#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "get_string.h"
#include "bloom.h"

int main(int argc, char *argv[]){

    int bloomSize = atoi(argv[4]);                              //gets bloomSize from command line
    
    record* head = (record*)malloc(sizeof(record));

    get_record(head, argv[2]);                                 //reads the record's file given from the command line

    bloom* bloom_head = (bloom*)malloc(sizeof(bloom));
    
    record* temp = head->next;                                  //initializes with head->next because the head is empty
    while(temp != NULL){
        if(strcmp(temp->yesNo, "YES") == 0){
            bloom_push(bloom_head, temp, bloomSize);
            bloom_head->next = NULL;
            break;
        }
        temp = temp->next;
    }                                                           //creates the head of bloom filter

    temp = head->next;
    bloom* temp_b;

    int flag;

    while(temp != NULL){
        if(strcmp(temp->yesNo, "YES") == 0){
            
            flag = 0;
            temp_b = bloom_head;    

            while(temp_b != NULL){
                if(strcmp(temp_b->virus, temp->virus) == 0){        //if the virus exists in bloom filter
                    bloom_insert(temp_b, temp->id, bloomSize);      //just insert the id in the correct bloom filter
                    flag = 1;
                    break; 
                }
                else
                    temp_b = temp_b->next;
                
            }                                               

            if(flag == 0){                                          //if virus not existant in bloom filter yet
                bloom* node_b = (bloom*)malloc(sizeof(bloom));
                bloom_push(node_b, temp, bloomSize);                //create new bloom node with the virus
                temp_b = bloom_head;
                while(temp_b->next != NULL)
                    temp_b = temp_b->next;
                
                temp_b->next = node_b;
                node_b->next = NULL;                                //place new bloom node in the correct position
            }
        }
        temp = temp->next;
    }

    virus* virus_head = (virus*)malloc(sizeof(virus));
    
    temp = head->next;

    virus_push(virus_head, temp);                                  //creates the head of the virus list that includes the skiplists for each virus
    virus_head->next = NULL;
    
    virus* temp_v;
    
    while(temp != NULL){
            
        flag = 0;
        temp_v = virus_head;    

        while(temp_v != NULL){
            if(strcmp(temp_v->virus_name, temp->virus) == 0){                   //if virus exists in virus list 
                if(strcmp(temp->yesNo, "YES") == 0)                             //check if the id is vaccinated
                    skiplist_insert(temp_v->vaccinated_persons, temp->id);      //if yes insert it in the vaccinated skiplist of the virus
                else
                    skiplist_insert(temp_v->not_vaccinated_persons, temp->id);  //if not insert it in the not vaccinated skiplist of the virus
                
                flag = 1;
                break;
            }
            else
                temp_v = temp_v->next;
        }

        if(flag == 0){                                                          //if virus non existant in virus list yet       
            virus* node_v = (virus*)malloc(sizeof(virus));
            virus_push(node_v, temp);                                           //create new virus node for the virus
            temp_v = virus_head;
            while(temp_v->next != NULL)
                temp_v = temp_v->next;
            
            temp_v->next = node_v;
            node_v->next = NULL;                                                //place the virus node on the correct position

            if(strcmp(temp->yesNo, "YES") == 0)                                 //check if the id is vaccinated
                skiplist_insert(node_v->vaccinated_persons, temp->id);          //if yes insert it in the vaccinated skiplist of the virus
            else
                skiplist_insert(node_v->not_vaccinated_persons, temp->id);      //if not insert it in the not vaccinated skiplist of the virus
        }
    
        temp = temp->next;
    }

    temp = head->next;

    country* country_head = (country*)malloc(sizeof(country));                  //creates country head
    country_head->country = (char*)malloc(sizeof(char) * 60);

    strcpy(country_head->country, temp->country);                               //sets name of country in head node
    country_push(country_head, temp);                                           //calculates the totals of vaccinated and not vaccinated people in country of the head
    country_head->next = NULL;
    
    country* temp_c;
    
    while(temp != NULL){
        flag = 0;
        temp_c = country_head;    

        while(temp_c != NULL){                                                  
            if(strcmp(temp_c->country, temp->country) == 0){                    //if country exists in country list
                country_push(temp_c, head->next);                               //calculates the totals for the country
                flag = 1;
                break;
            }
            else
                temp_c = temp_c->next;
        }

        if(flag == 0){                                                          //creates new country node      
            country* node_c = (country*)malloc(sizeof(country));   
            node_c->country = (char*)malloc(sizeof(char) * 60);

            strcpy(node_c->country, temp->country);                            //sets name of country in new node
            country_push(node_c, head->next);                                  //calculates the totals of vaccinated and not vaccinated people in country of the new node
            temp_c = country_head;
            while(temp_c->next != NULL)
                temp_c = temp_c->next;

            temp_c->next = node_c;
            node_c->next = NULL;                                                //places new country node in the correct position
        }
    
        temp = temp->next;
    }    
    
    /* done with saving data */

    char buffer[BUFSIZ];

    int check = 0, counter, i;

    printf("Enter command: ");
    fgets(buffer, BUFSIZ, stdin);               //reads command

    while(strcmp(buffer, "exit\n") != 0){           //when command is exit the program stops

        if(check != 0){                             //to check if it's the first time reading command
            printf("Enter command: ");
            fgets(buffer, BUFSIZ, stdin);               //reads command
        }

        char** Command;
        int num; 
        flag = 0;

        check = 1;

        counter = 0;
        i = 0;

        /* vaccineStatusBloom */
        if(strstr(buffer, "vaccineStatusBloom") != 0){
            
            while(buffer[i] != '\n'){
                if(buffer[i] == ' ')
                    counter++;
                i++;
            }

            if(counter == 2)
                num = 3;   
            else
                num = 0;                                       //used to check many cases of command
            
            if(num != 0)
                Command = get_command(buffer, num);             //splits command in array for comparisons below

            if(num == 3){
                bloom* temp_b = bloom_head;
                while(temp_b != NULL){
                    if(strcmp(temp_b->virus, Command[2]) == 0){                 //if virus is compatible with command
                        bloom_check(temp_b, bloomSize, Command[1]);             //check in bloom filter for id
                        break;
                    }
                    temp_b = temp_b->next;
                }
                for(int i = 0; i < num; i++)
                    free(Command[i]);
                free(Command);
                continue;
            }
            else{
                printf("ERROR!\n");
                continue;
            }
        }

        /* vaccineStatus */
        if(strstr(buffer, "vaccineStatus") != 0){
           
            while(buffer[i] != '\n'){
                if(buffer[i] == ' ')
                    counter++;
                i++;
            }

            if(counter == 2)
                num = 3;
            else if(counter == 1) 
                num = 2;    
            else
                num = 0;                                       //used to check many cases of command
            
            if(num != 0)
                Command = get_command(buffer, num);            //splits command in array to be used in comparisons

            if(num == 3){               //case: citizenID virus
                temp_v = virus_head;
                while(temp_v != NULL){
                    if(strcmp(temp_v->virus_name, Command[2]) == 0){                            //when virus compatible with command
                        if(skiplist_search(temp_v->vaccinated_persons, Command[1]) == 1){       //check if citizen is in vaccinated skiplist
                            temp = head->next;
                            while(temp != NULL){
                                if(strcmp(temp->id, Command[1]) == 0){
                                    printf("VACCINATED ON %s \n", temp->date);                  //if yes print out when he was vaccinated
                                    break;
                                }
                                temp = temp->next;
                            }
                        }
                        else
                            printf("NOT VACCINATED\n");                                         //if not he's not vaccinated
                    }
                    temp_v = temp_v->next;
                }
            }

            else if(num == 2){       //case: citizenID -all viruses-
                temp_v = virus_head;
                while(temp_v != NULL){                                                      //check for all viruses
                    if(skiplist_search(temp_v->vaccinated_persons, Command[1]) == 1){       //if citizenId is found in vaccinated skiplist
                        temp = head->next;
                        while(temp != NULL){
                            if(strcmp(temp->id, Command[1]) == 0 && strcmp(temp->virus, temp_v->virus_name) == 0){
                                printf("%s %s %s \n", temp_v->virus_name, temp->yesNo, temp->date);         //if yes print out virus yes and the date of vaccination
                                break;
                            }
                            temp = temp->next;
                        }
                    }
                    else
                        printf("%s NO\n", temp_v->virus_name);              //if not then print virus and no
                    
                    temp_v = temp_v->next;
                }
            }

            else                            //for any incompatible format not checked above
                printf("ERROR!\n");         //give an error

            if(num != 0){
                for(int i = 0; i < num; i++)
                    free(Command[i]);
                free(Command);
            }
        }

        /* populationStatus */
        if(strstr(buffer, "populationStatus") != 0){
            
            while(buffer[i] != '\n'){
                if(buffer[i] == ' ')
                    counter++;
                i++;
            }

            if(counter == 4)
                num = 5;
            else if(counter == 3)    
                num = 4;
            else if(counter == 2)
                num = 3;
            else if(counter == 1)
                num = 2;
            else
                num = 0;                                            //used to check many cases of command

            if(num != 0)
                Command = get_command(buffer, num);                 //splits command in array to be used in comparisons

            counter = 0;

            float percent = 0.0;

            if(num == 5){                                                   //case: country virus date1 date2
                temp = head->next;
                while(temp != NULL){
                    
                    if(strcmp(temp->country, Command[1]) == 0 && strcmp(temp->virus, Command[2]) == 0 && strcmp(temp->yesNo, "YES") == 0){  //when country virus and yes is compatible to command
                        char** date1 = get_date(temp->date);
                        char** date2 = get_date(Command[3]);
                        char** date3 = get_date(Command[4]);                //get the date limits from the command and the current date we are about to examine

                        int flag1 = 0, flag2 = 0;

                        if(strcmp(temp->date, Command[3]) == 0 || strcmp(temp->date, Command[4]) == 0)
                            counter++;
                        else{
                            if(strcmp(date2[2], date1[2]) < 0)
                                flag1 = 1;
                            else if(strcmp(date2[2], date1[2]) == 0 && strcmp(date2[1], date1[1]) < 0)
                                flag1 = 1;
                            else if(strcmp(date2[2], date1[2]) == 0 && strcmp(date2[1], date1[1]) == 0 && strcmp(date2[0], date1[0]) < 0)
                                flag1 = 1;
                            
                            if(strcmp(date1[2], date3[2]) < 0)
                                flag2 = 1;
                            else if(strcmp(date1[2], date3[2]) == 0 && strcmp(date1[1], date3[1]) < 0)
                                flag2 = 1;
                            else if(strcmp(date1[2], date3[2]) == 0 && strcmp(date1[1], date3[1]) == 0 && strcmp(date1[0], date3[0]) < 0)
                                flag2 = 1;

                            if(flag1 == 1 && flag2 == 1)
                                counter++;
                        }                                                   //checking if the current date is inbetween the dates given by user and increasing counter if positive

                        country* node_c = country_head;
                        while(node_c != NULL){
                            if(strcmp(node_c->country, Command[1]) == 0){                                           //finds correct country to take the totals 
                                percent = (float)counter / (node_c->total_YES + node_c->total_NO) * 100.0;          //calculates the percantage of vaccinated people for this virus in country given by the user
                                break;
                            }
                            node_c = node_c->next;
                        }

                        for(int i = 0; i < 3; i++){
                            free(date1[i]); free(date2[i]); free(date3[i]);
                        }
                        free(date1); free(date2); free(date3);
            
                    }
                    temp = temp->next;
                }
                printf("%s %s %d %.2f%%\n", Command[1], Command[2], counter, percent);                             //prints out the information asked
            }

            else if(num == 4){                                              //case: virus date1 date2 -all countries-
                temp = head->next;
                while(temp != NULL){
                    if(strcmp(temp->country, Command[1]) == 0){             //checking for error in command's format
                        printf("ERROR!\n");
                        flag = 1;
                        break;
                    }
                    temp = temp->next;
                }

                if(flag == 0){                                  //if there's no error
                    temp = head->next;
                    while(temp != NULL){
                        if(strcmp(temp->virus, Command[1]) == 0 && strcmp(temp->yesNo, "YES") == 0){          //when virus and yes is compatible to command 
                            char** date1 = get_date(temp->date);
                            char** date2 = get_date(Command[2]);
                            char** date3 = get_date(Command[3]);                                             //get the date limits from the command and the current date we are about to examine

                            int flag1 = 0, flag2 = 0;

                            country* node_c = country_head;

                            if(strcmp(temp->date, Command[2]) == 0 || strcmp(temp->date, Command[3]) == 0){         
                                while(node_c != NULL){
                                    if(strcmp(temp->country, node_c->country) == 0){
                                        node_c->num_YES++;
                                        break;
                                    }
                                    node_c = node_c->next;
                                }
                            }
                            else{
                                if(strcmp(date2[2], date1[2]) < 0)
                                    flag1 = 1;
                                else if(strcmp(date2[2], date1[2]) == 0 && strcmp(date2[1], date1[1]) < 0)
                                    flag1 = 1;
                                else if(strcmp(date2[2], date1[2]) == 0 && strcmp(date2[1], date1[1]) == 0 && strcmp(date2[0], date1[0]) < 0)
                                    flag1 = 1;
                                
                                if(strcmp(date1[2], date3[2]) < 0)
                                    flag2 = 1;
                                else if(strcmp(date1[2], date3[2]) == 0 && strcmp(date1[1], date3[1]) < 0)
                                    flag2 = 1;
                                else if(strcmp(date1[2], date3[2]) == 0 && strcmp(date1[1], date3[1]) == 0 && strcmp(date1[0], date3[0]) < 0)
                                    flag2 = 1;

                                if(flag1 == 1 && flag2 == 1){
                                    while(node_c != NULL){
                                        if(strcmp(temp->country, node_c->country) == 0){
                                            node_c->num_YES++;
                                            break;
                                        }
                                        node_c = node_c->next;
                                    }
                                }
                                    
                            }                                                       //checking if the current date is inbetween the dates given by user
                            for(int i = 0; i < 3; i++){
                                free(date1[i]); free(date2[i]); free(date3[i]);
                            }
                            free(date1); free(date2); free(date3);
                        }
                        temp = temp->next;                                          //checking for all countries and increasining the number of yes for each country if positive
                    }

                    country* node_c = country_head;
                    while(node_c != NULL){
                        percent = (float)node_c->num_YES / (node_c->total_YES + node_c->total_NO) * 100.0;          //calculating the percentage of vaccinated people for this virus for every country seperately
                        printf("%s %d %.2f%%\n", node_c->country, node_c->num_YES, percent);                        //prints out the information asked
                        node_c->num_YES = 0;                                                                        //sets number of yes to zero for each country for next request
                        node_c = node_c->next;
                    }
                }
                
            } 

            else if(num == 3){                                                      //case: country virus -all dates-
                temp = head->next;
                while(temp != NULL){
                    if(atoi(Command[2]) < 32 && atoi(Command[2]) > 0){              //checking for error in command's format
                        printf("ERROR!\n");
                        flag = 1;
                        break;
                    }
                    temp = temp->next;
                }

                if(flag == 0){                          //if there's no error
                    temp = head->next;
                    while(temp != NULL){
                        if(strcmp(temp->country, Command[1]) == 0 && strcmp(temp->virus, Command[2]) == 0 && strcmp(temp->yesNo, "YES") == 0)       //when country virus and yes is compatible to command
                            counter++;

                        country* node_c = country_head;
                        while(node_c != NULL){
                            if(strcmp(node_c->country, Command[1]) == 0){                                                                           //find country and get the totals
                                percent = (float)counter / (node_c->total_YES + node_c->total_NO) * 100.0;                                          //calculate the percentage for virus in this country
                                break;
                            }
                            node_c = node_c->next;
                        }
                        
                        temp = temp->next;                                                                                                          //for every date available in records
                    }
                    printf("%s %s %d %.2f%%\n", Command[1], Command[2], counter, percent);                                                          //print out information asked
                }
            }

            else if(num == 2){                                                  //case: virus -all countries all dates-
                temp = head->next;
                while(temp != NULL){
                    if((atoi(Command[1]) < 32 && atoi(Command[1]) > 0) || strcmp(Command[1], temp->country) == 0){                  //checking for error in command's format
                        printf("ERROR!\n");
                        flag = 1;
                        break;
                    }
                    temp = temp->next;
                }

                if(flag == 0){                              //if there's no error
                    temp = head->next;

                    while(temp != NULL){
                        country* node_c = country_head;
                        if(strcmp(temp->virus, Command[1]) == 0 && strcmp(temp->yesNo, "YES") == 0){                        //when virus and yes is compatible to command
                            while(node_c != NULL){
                                if(strcmp(temp->country, node_c->country) == 0){                                
                                    node_c->num_YES++;                                                                     //increase number of yes for every country who has this virus
                                    break;
                                }
                                node_c = node_c->next;
                            }
                        }

                        temp = temp->next;
                    }

                    country* node_c = country_head;
                    while(node_c != NULL){
                        percent = (float)node_c->num_YES / (node_c->total_YES + node_c->total_NO) * 100.0;              //calculates percantage for this virus in every country seperately
                        printf("%s %d %.2f%%\n", node_c->country, node_c->num_YES, percent);                            //prints out information asked
                        node_c->num_YES = 0;                                                                            //sets number of yes to zero for each country for next request
                        node_c = node_c->next;
                    }
                }
            }

            else                                   //for any incompatible format not checked above
                printf("ERROR!\n");                //give an error

            if(num != 0){
                for(int i = 0; i < num; i++)
                    free(Command[i]);
                free(Command);
            } 
        }

        /* popStatusByAge */
        if(strstr(buffer, "popStatusByAge") != 0){
                
            while(buffer[i] != '\n'){
                if(buffer[i] == ' ')
                    counter++;
                i++;
            }

            if(counter == 4)
                num = 5;
            else if(counter == 3)    
                num = 4;
            else if(counter == 2)
                num = 3;
            else if(counter == 1)
                num = 2;
            else
                num = 0;                                              //used to check many cases of command

            if(num != 0)
                Command = get_command(buffer, num);                  //splits command to array to be used in comparisons

            counter = 0;

            float percent0 = 0.0, percent20 = 0.0, percent40 = 0.0, percent60 = 0.0;        //percentages for all age categories

            if(num == 5){                                           //case: country virus date1 date2
                temp = head->next;
                while(temp != NULL){
                    
                    if(strcmp(temp->country, Command[1]) == 0 && strcmp(temp->virus, Command[2]) == 0 && strcmp(temp->yesNo, "YES") == 0){  //when country virus and yes are compatible with command
                        char** date1 = get_date(temp->date);
                        char** date2 = get_date(Command[3]);
                        char** date3 = get_date(Command[4]);                //get date limits and the current date

                        int flag1 = 0, flag2 = 0;
                        
                        country* node_c = country_head;

                        if(strcmp(temp->date, Command[3]) == 0 || strcmp(temp->date, Command[4]) == 0){
                            if(strcmp(temp->age, "0") >= 0 && strcmp(temp->age, "20") <= 0){
                                while(node_c != NULL){
                                    if(strcmp(node_c->country, temp->country) == 0){
                                        node_c->to_twenty_YES++;
                                        break;
                                    }
                                    node_c = node_c->next;
                                }
                            }

                            else if(strcmp(temp->age, "20") > 0 && strcmp(temp->age, "40") <= 0){
                                while(node_c != NULL){
                                    if(strcmp(node_c->country, temp->country) == 0){
                                        node_c->to_fourty_YES++;
                                        break;
                                    }
                                    node_c = node_c->next;
                                }
                            }

                            else if(strcmp(temp->age, "40") > 0 && strcmp(temp->age, "60") <= 0){
                                while(node_c != NULL){
                                    if(strcmp(node_c->country, temp->country) == 0){
                                        node_c->to_sixty_YES++;
                                        break;
                                    }
                                    node_c = node_c->next;
                                }
                            }

                            else{
                                while(node_c != NULL){
                                    if(strcmp(node_c->country, temp->country) == 0){
                                        node_c->sixty_plus_YES++;
                                        break;
                                    }
                                    node_c = node_c->next;
                                }
                            }
                        }
                            
                        else{
                            if(strcmp(date2[2], date1[2]) < 0)
                                flag1 = 1;
                            else if(strcmp(date2[2], date1[2]) == 0 && strcmp(date2[1], date1[1]) < 0)
                                flag1 = 1;
                            else if(strcmp(date2[2], date1[2]) == 0 && strcmp(date2[1], date1[1]) == 0 && strcmp(date2[0], date1[0]) < 0)
                                flag1 = 1;
                            
                            if(strcmp(date1[2], date3[2]) < 0)
                                flag2 = 1;
                            else if(strcmp(date1[2], date3[2]) == 0 && strcmp(date1[1], date3[1]) < 0)
                                flag2 = 1;
                            else if(strcmp(date1[2], date3[2]) == 0 && strcmp(date1[1], date3[1]) == 0 && strcmp(date1[0], date3[0]) < 0)
                                flag2 = 1;

                            if(flag1 == 1 && flag2 == 1){
                                if(strcmp(temp->age, "0") >= 0 && strcmp(temp->age, "20") <= 0){
                                    while(node_c != NULL){
                                        if(strcmp(node_c->country, temp->country) == 0){
                                            node_c->to_twenty_YES++;
                                            break;
                                        }
                                        node_c = node_c->next;
                                    }
                                }
                                
                                else if(strcmp(temp->age, "20") > 0 && strcmp(temp->age, "40") <= 0){
                                    while(node_c != NULL){
                                        if(strcmp(node_c->country, temp->country) == 0){
                                            node_c->to_fourty_YES++;
                                            break;
                                        }
                                        node_c = node_c->next;
                                    }
                                }

                                else if(strcmp(temp->age, "40") > 0 && strcmp(temp->age, "60") <= 0){
                                    while(node_c != NULL){
                                        if(strcmp(node_c->country, temp->country) == 0){
                                            node_c->to_sixty_YES++;
                                            break;
                                        }
                                        node_c = node_c->next;
                                    }
                                }

                                else{
                                    while(node_c != NULL){
                                        if(strcmp(node_c->country, temp->country) == 0){
                                            node_c->sixty_plus_YES++;
                                            break;
                                        }
                                        node_c = node_c->next;
                                    }
                                }
                            }   
                        }       //if current date is inbetween dates given find in which category the person belongs to and increase its counter in the country given by user
                        for(int i = 0; i < 3; i++){
                            free(date1[i]); free(date2[i]); free(date3[i]);
                        }
                        free(date1); free(date2); free(date3);
                    }
                    temp = temp->next;
                }

                country* node_c = country_head;
                while(node_c != NULL){
                    if(strcmp(node_c->country, Command[1]) == 0){
                        percent0 = (float) node_c->to_twenty_YES / (node_c->total_YES + node_c->total_NO) * 100.0;
                        percent20 = (float) node_c->to_fourty_YES / (node_c->total_YES + node_c->total_NO) * 100.0;
                        percent40 = (float) node_c->to_sixty_YES / (node_c->total_YES + node_c->total_NO) * 100.0;
                        percent60 = (float) node_c->sixty_plus_YES / (node_c->total_YES + node_c->total_NO) * 100.0;                                    //calculate percentage for every age category for this virus in country given by user
                        printf("%s\n0-20 %d %.2f%%\n20-40 %d %.2f%%\n40-60 %d %.2f%%\n60+ %d %.2f%%\n", Command[1], node_c->to_twenty_YES, percent0,
                        node_c->to_fourty_YES, percent20, node_c->to_sixty_YES, percent40, node_c->sixty_plus_YES, percent60);                          //print out the information asked
                        break;
                    }
                    node_c->to_twenty_YES = 0;
                    node_c->to_fourty_YES = 0;
                    node_c->to_sixty_YES = 0;
                    node_c->sixty_plus_YES = 0;         //set counters of each category to zero for next calculation
                    
                    node_c = node_c->next;
                }
            }

            else if(num == 4){                      //case: virus date1 date2 -all countries-
                temp = head->next;
                while(temp != NULL){
                    if(strcmp(temp->country, Command[1]) == 0){             //checking for error in command's format
                        printf("ERROR!\n");
                        flag = 1;
                        break;
                    }
                    temp = temp->next;
                }

                if(flag == 0){
                    temp = head->next;
                    while(temp != NULL){
                        
                        if(strcmp(temp->virus, Command[1]) == 0 && strcmp(temp->yesNo, "YES") == 0){            //when virus and yes are compatible with command
                            char** date1 = get_date(temp->date);
                            char** date2 = get_date(Command[2]);
                            char** date3 = get_date(Command[3]);                    //get date limits given and current date

                            int flag1 = 0, flag2 = 0;
                            
                            country* node_c = country_head;

                            if(strcmp(temp->date, Command[2]) == 0 || strcmp(temp->date, Command[3]) == 0){
                                if(strcmp(temp->age, "0") >= 0 && strcmp(temp->age, "20") <= 0){
                                    while(node_c != NULL){
                                        if(strcmp(node_c->country, temp->country) == 0){
                                            node_c->to_twenty_YES++;
                                            break;
                                        }
                                        node_c = node_c->next;
                                    }
                                }

                                else if(strcmp(temp->age, "20") > 0 && strcmp(temp->age, "40") <= 0){
                                    while(node_c != NULL){
                                        if(strcmp(node_c->country, temp->country) == 0){
                                            node_c->to_fourty_YES++;
                                            break;
                                        }
                                        node_c = node_c->next;
                                    }
                                }

                                else if(strcmp(temp->age, "40") > 0 && strcmp(temp->age, "60") <= 0){
                                    while(node_c != NULL){
                                        if(strcmp(node_c->country, temp->country) == 0){
                                            node_c->to_sixty_YES++;
                                            break;
                                        }
                                        node_c = node_c->next;
                                    }
                                }

                                else{
                                    while(node_c != NULL){
                                        if(strcmp(node_c->country, temp->country) == 0){
                                            node_c->sixty_plus_YES++;
                                            break;
                                        }
                                        node_c = node_c->next;
                                    }
                                }
                            }
                                
                            else{
                                if(strcmp(date2[2], date1[2]) < 0)
                                    flag1 = 1;
                                else if(strcmp(date2[2], date1[2]) == 0 && strcmp(date2[1], date1[1]) < 0)
                                    flag1 = 1;
                                else if(strcmp(date2[2], date1[2]) == 0 && strcmp(date2[1], date1[1]) == 0 && strcmp(date2[0], date1[0]) < 0)
                                    flag1 = 1;
                                
                                if(strcmp(date1[2], date3[2]) < 0)
                                    flag2 = 1;
                                else if(strcmp(date1[2], date3[2]) == 0 && strcmp(date1[1], date3[1]) < 0)
                                    flag2 = 1;
                                else if(strcmp(date1[2], date3[2]) == 0 && strcmp(date1[1], date3[1]) == 0 && strcmp(date1[0], date3[0]) < 0)
                                    flag2 = 1;

                                if(flag1 == 1 && flag2 == 1){
                                    if(strcmp(temp->age, "0") >= 0 && strcmp(temp->age, "20") <= 0){
                                        while(node_c != NULL){
                                            if(strcmp(node_c->country, temp->country) == 0){
                                                node_c->to_twenty_YES++;
                                                break;
                                            }
                                            node_c = node_c->next;
                                        }
                                    }
                                    
                                    else if(strcmp(temp->age, "20") > 0 && strcmp(temp->age, "40") <= 0){
                                        while(node_c != NULL){
                                            if(strcmp(node_c->country, temp->country) == 0){
                                                node_c->to_fourty_YES++;
                                                break;
                                            }
                                            node_c = node_c->next;
                                        }
                                    }

                                    else if(strcmp(temp->age, "40") > 0 && strcmp(temp->age, "60") <= 0){
                                        while(node_c != NULL){
                                            if(strcmp(node_c->country, temp->country) == 0){
                                                node_c->to_sixty_YES++;
                                                break;
                                            }
                                            node_c = node_c->next;
                                        }
                                    }

                                    else{
                                        while(node_c != NULL){
                                            if(strcmp(node_c->country, temp->country) == 0){
                                                node_c->sixty_plus_YES++;
                                                break;
                                            }
                                            node_c = node_c->next;
                                        }
                                    }
                                }    
                            }   //if current date is inbetween dates given find in which category the person belongs to and increase its counter for all countries
                            for(int i = 0; i < 3; i++){
                                free(date1[i]); free(date2[i]); free(date3[i]);
                            }
                            free(date1); free(date2); free(date3);
                        }
                        temp = temp->next;
                    }

                    country *node_c = country_head;
                    while(node_c != NULL){
                        
                        percent0 = (float) node_c->to_twenty_YES / (node_c->total_YES + node_c->total_NO) * 100.0;
                        percent20 = (float) node_c->to_fourty_YES / (node_c->total_YES + node_c->total_NO) * 100.0;
                        percent40 = (float) node_c->to_sixty_YES / (node_c->total_YES + node_c->total_NO) * 100.0;
                        percent60 = (float) node_c->sixty_plus_YES / (node_c->total_YES + node_c->total_NO) * 100.0;                //calculates percantage of yes for virus in every country
                        printf("%s\n0-20 %d %.2f%%\n20-40 %d %.2f%%\n40-60 %d %.2f%%\n60+ %d %.2f%%\n\n", node_c->country, node_c->to_twenty_YES, percent0,
                        node_c->to_fourty_YES, percent20, node_c->to_sixty_YES, percent40, node_c->sixty_plus_YES, percent60);      //prints out the information asked
                            
                        node_c->to_twenty_YES = 0;
                        node_c->to_fourty_YES = 0;
                        node_c->to_sixty_YES = 0;
                        node_c->sixty_plus_YES = 0;         //sets counter of yes for all age categories to zero for next calculation
                        
                        node_c = node_c->next;
                    }
                }
            } 

            else if(num == 3){                                                   //case: country virus -all dates-
                temp = head->next;
                while(temp != NULL){
                    if(atoi(Command[2]) < 32 && atoi(Command[2]) > 0){          //checking for error in command's format
                        printf("ERROR!\n");
                        flag = 1;
                        break;
                    }
                    temp = temp->next;
                }

                if(flag == 0){                      //if there's no error
                    temp = head->next;
                    while(temp != NULL){
                        
                        if(strcmp(temp->country, Command[1]) == 0 && strcmp(temp->virus, Command[2]) == 0 && strcmp(temp->yesNo, "YES") == 0){        //when country virus and yes are compatible with command              
                            country* node_c = country_head;

                            if(strcmp(temp->age, "0") >= 0 && strcmp(temp->age, "20") <= 0){
                                while(node_c != NULL){
                                    if(strcmp(node_c->country, temp->country) == 0){
                                        node_c->to_twenty_YES++;
                                        break;
                                    }
                                    node_c = node_c->next;
                                }
                            }

                            else if(strcmp(temp->age, "20") > 0 && strcmp(temp->age, "40") <= 0){
                                while(node_c != NULL){
                                    if(strcmp(node_c->country, temp->country) == 0){
                                        node_c->to_fourty_YES++;
                                        break;
                                    }
                                    node_c = node_c->next;
                                }
                            }

                            else if(strcmp(temp->age, "40") > 0 && strcmp(temp->age, "60") <= 0){
                                while(node_c != NULL){
                                    if(strcmp(node_c->country, temp->country) == 0){
                                        node_c->to_sixty_YES++;
                                        break;
                                    }
                                    node_c = node_c->next;
                                }
                            }

                            else{
                                while(node_c != NULL){
                                    if(strcmp(node_c->country, temp->country) == 0){
                                        node_c->sixty_plus_YES++;
                                        break;
                                    }
                                    node_c = node_c->next;
                                }
                            }
                        }       //find in which category the person belongs to and increase its counter in the country given by user
                        temp = temp->next;
                    }

                    country* node_c = country_head;
                    while(node_c != NULL){
                        if(strcmp(node_c->country, Command[1]) == 0){
                            percent0 = (float) node_c->to_twenty_YES / (node_c->total_YES + node_c->total_NO) * 100.0;
                            percent20 = (float) node_c->to_fourty_YES / (node_c->total_YES + node_c->total_NO) * 100.0;
                            percent40 = (float) node_c->to_sixty_YES / (node_c->total_YES + node_c->total_NO) * 100.0;
                            percent60 = (float) node_c->sixty_plus_YES / (node_c->total_YES + node_c->total_NO) * 100.0;                    //calculates the percentages for virus for all age categories in country given
                            printf("%s\n0-20 %d %.2f%%\n20-40 %d %.2f%%\n40-60 %d %.2f%%\n60+ %d %.2f%%\n", Command[1], node_c->to_twenty_YES, percent0,
                            node_c->to_fourty_YES, percent20, node_c->to_sixty_YES, percent40, node_c->sixty_plus_YES, percent60);          //prints out the information asked
                            break;
                        }
                        node_c->to_twenty_YES = 0;
                        node_c->to_fourty_YES = 0;
                        node_c->to_sixty_YES = 0;
                        node_c->sixty_plus_YES = 0;      //sets counter of yes for all age categories to zero for next calculation

                        node_c = node_c->next;
                    }
                }
            }

            else if(num == 2){                  //case: virus -all countries all dates-
                temp = head->next;
                while(temp != NULL){
                    if((atoi(Command[1]) < 32 && atoi(Command[1]) > 0) || strcmp(Command[1], temp->country) == 0){          //checking for error in command's format
                        printf("ERROR!\n");
                        flag = 1;
                        break;
                    }
                    temp = temp->next;
                }

                if(flag == 0){          //if there's no error
                    temp = head->next;
                    while(temp != NULL){
                        
                        if(strcmp(temp->virus, Command[1]) == 0 && strcmp(temp->yesNo, "YES") == 0){         //when virus and yes are compatible to command                  
                            country* node_c = country_head;

                            if(strcmp(temp->age, "0") >= 0 && strcmp(temp->age, "20") <= 0){
                                while(node_c != NULL){
                                    if(strcmp(node_c->country, temp->country) == 0){
                                        node_c->to_twenty_YES++;
                                        break;
                                    }
                                    node_c = node_c->next;
                                }
                            }

                            else if(strcmp(temp->age, "20") > 0 && strcmp(temp->age, "40") <= 0){
                                while(node_c != NULL){
                                    if(strcmp(node_c->country, temp->country) == 0){
                                        node_c->to_fourty_YES++;
                                        break;
                                    }
                                    node_c = node_c->next;
                                }
                            }

                            else if(strcmp(temp->age, "40") > 0 && strcmp(temp->age, "60") <= 0){
                                while(node_c != NULL){
                                    if(strcmp(node_c->country, temp->country) == 0){
                                        node_c->to_sixty_YES++;
                                        break;
                                    }
                                    node_c = node_c->next;
                                }
                            }

                            else{
                                while(node_c != NULL){
                                    if(strcmp(node_c->country, temp->country) == 0){
                                        node_c->sixty_plus_YES++;
                                        break;
                                    }
                                    node_c = node_c->next;
                                }
                            }
                        }   //find in which category the person belongs to and increase its counter 
                        temp = temp->next;
                    }

                    country *node_c = country_head;
                    while(node_c != NULL){
                        
                        percent0 = (float) node_c->to_twenty_YES / (node_c->total_YES + node_c->total_NO) * 100.0;
                        percent20 = (float) node_c->to_fourty_YES / (node_c->total_YES + node_c->total_NO) * 100.0;
                        percent40 = (float) node_c->to_sixty_YES / (node_c->total_YES + node_c->total_NO) * 100.0;
                        percent60 = (float) node_c->sixty_plus_YES / (node_c->total_YES + node_c->total_NO) * 100.0;                //calculates percentage for virus for all age categories in all countries
                        printf("%s\n0-20 %d %.2f%%\n20-40 %d %.2f%%\n40-60 %d %.2f%%\n60+ %d %.2f%%\n\n", node_c->country, node_c->to_twenty_YES, percent0,
                        node_c->to_fourty_YES, percent20, node_c->to_sixty_YES, percent40, node_c->sixty_plus_YES, percent60);      //prints out the information asked
                            
                        
                        node_c->to_twenty_YES = 0;
                        node_c->to_fourty_YES = 0;
                        node_c->to_sixty_YES = 0;
                        node_c->sixty_plus_YES = 0;          //sets counter of yes for all age categories to zero for next calculation
                        
                        node_c = node_c->next;
                    }
                }
            }

            else                        //for any incompatible format not checked above
                printf("ERROR!\n");     //give an error

            if(num != 0){
                for(int i = 0; i < num; i++)
                    free(Command[i]);
                free(Command);
            }
        }

        /* insertCitizenRecord */
        if(strstr(buffer, "insertCitizenRecord") != 0){
            
            while(buffer[i] != '\n'){
                if(buffer[i] == ' ')
                    counter++;
                i++;
            }

            if(counter == 8)
                num = 9;
            else if(counter == 7)
                num = 8;
            else
                num = 0;                                //used to check many cases of command

            if(num != 0)
                Command = get_command(buffer, num);     //splits command in array to be used for comparisons

            int flag1 = 0;

            if(num == 9){                               //case: citizenID firstName lastName country age virusName YES date
                if(strcmp(Command[7], "NO") == 0)       //checking for error in command's format
                    printf("Error!\n");

                else{             
                    temp_v = virus_head;
                    while(temp_v != NULL){
                        if(strcmp(temp_v->virus_name, Command[6]) == 0 && skiplist_search(temp_v->vaccinated_persons, Command[1]) == 1){            //check for already vaccinated person
                            temp = head->next;
                            while(temp != NULL){
                                if(strcmp(temp->id, Command[1]) == 0){
                                    printf("ERROR: CITIZEN %s ALREADY VACCINATED ON %s \n", temp->id, temp->date);
                                    flag = 1;
                                    break;
                                }
                                else
                                    temp = temp->next; 
                            }
                        }
                        temp_v = temp_v->next; 
                    }

                    if(flag == 0){
                        temp_v = virus_head;
                        while(temp_v != NULL){
                            if(strcmp(temp_v->virus_name, Command[6]) == 0){ 
                                flag = 2;
                                break;
                            }
                            temp_v = temp_v->next; 
                        }

                        if(flag == 2){
                            temp_v = virus_head;
                            while(temp_v != NULL){             
                                if(strcmp(temp_v->virus_name, Command[6]) == 0 && strcmp(Command[7], "YES") == 0 && skiplist_search(temp_v->not_vaccinated_persons, Command[1]) == 1){  //check for existand record who's not vaccinated and it's about to get inserted as vaccinated
                                    temp = head->next;
                                    while(temp != NULL){
                                        if(strcmp(temp->id, Command[1]) == 0){
                                            strcpy(temp->yesNo, "YES");
                                            strcpy(temp->date, Command[8]);                          //find the existant record and change the information of vaccination
                                        }
                                        temp = temp->next;
                                    }
                                
                                    temp_b = bloom_head;
                                    while(temp_b != NULL){
                                        if(strcmp(temp_b->virus, Command[6]) == 0){
                                            bloom_insert(temp_b, Command[1], bloomSize);            //insert record in bloom filter
                                            break;
                                        }
                                        else
                                            temp_b = temp_b->next;
                                    }

                                    
                                    skiplist_insert(temp_v->vaccinated_persons, Command[1]);        //insert record in vaccinated skiplist
                                    skiplist_delete(temp_v->not_vaccinated_persons, Command[1]);    //delete record from non vaccinated skiplist

                                    flag1 = 1;
                                        
                                    printf("RECORD SUCCESFULLY INSERTED\n");                        //inform user for the succesful insertion

                                    break;
                                }
                                temp_v = temp_v->next;
                            }
                        

                            if(flag1 == 0){       //check for non existant record who's get inserted as vaccinated
                                temp = head->next;
                                while(temp->next != NULL)
                                    temp = temp->next;
                                
                                record* new = (record*)malloc(sizeof(record));
                                push_record(head->next, new, Command);

                                new->yesNo = (char*)malloc(sizeof(char) * 60);
                                new->date = (char*)malloc(sizeof(char) * 60);
                            
                                strcpy(new->yesNo, "YES");
                                strcpy(new->date, Command[8]);
                        
                                temp->next = new;
                                new->next = NULL;                       //creates new record and insert information of vaccination

                                temp_b = bloom_head;
                                while(temp_b != NULL){
                                    if(strcmp(temp_b->virus, Command[6]) == 0){                 
                                        bloom_insert(temp_b, Command[1], bloomSize);            //insert record in bloom filter
                                        break;
                                    }
                                    else
                                        temp_b = temp_b->next;
                                }

                                temp_v = virus_head;
                                while(temp_v != NULL){
                                    if(strcmp(temp_v->virus_name, Command[6]) == 0){
                                        skiplist_insert(temp_v->vaccinated_persons, Command[1]);        //insert record in vaccinated skiplist
                                        break;
                                    }
                                    else    
                                        temp_v = temp_v->next;
                                }
                                printf("RECORD SUCCESFULLY INSERTED\n");                                //inform user for successful insertion
                            }

                        }

                        else if(flag != 2){
                            temp = head->next;
                            while(temp->next != NULL)
                                temp = temp->next;
                            
                            record* new = (record*)malloc(sizeof(record));
                            push_record(head->next, new, Command);

                            new->yesNo = (char*)malloc(sizeof(char) * 60);
                            new->date = (char*)malloc(sizeof(char) * 60);
                        
                            strcpy(new->yesNo, "YES");
                            strcpy(new->date, Command[8]);
                    
                            temp->next = new;
                            new->next = NULL;                       //creates new record and insert information of vaccination

                            bloom* node_b = (bloom*)malloc(sizeof(bloom));
                            bloom_push(node_b, new, bloomSize);                //create new bloom node with the virus
                            temp_b = bloom_head;
                            while(temp_b->next != NULL)
                                temp_b = temp_b->next;
                            
                            temp_b->next = node_b;
                            node_b->next = NULL;                                //place new bloom node in the correct position

                            virus* node_v = (virus*)malloc(sizeof(virus));
                            virus_push(node_v, new);                                           //create new virus node for the virus
                            temp_v = virus_head;
                            while(temp_v->next != NULL)
                                temp_v = temp_v->next;
                            
                            temp_v->next = node_v;
                            node_v->next = NULL;                                                //place the virus node on the correct position

                            skiplist_insert(node_v->vaccinated_persons, new->id);          //if yes insert it in the vaccinated skiplist of the virus
                            
                            printf("RECORD SUCCESFULLY INSERTED\n");
                        }

                        temp_c = country_head;
                        while(temp_c != NULL){
                            if(strcmp(temp_c->country, Command[4]) == 0){ 
                                flag = 3;
                                break;
                            }
                            temp_c = temp_c->next; 
                        }

                        if(flag != 3){
                            country* node_c = (country*)malloc(sizeof(country));   
                            node_c->country = (char*)malloc(sizeof(char) * 60);

                            strcpy(node_c->country, Command[4]);                                //sets name of country in new node
                            country_push(node_c, head->next);                                  //calculates the totals of vaccinated and not vaccinated people in country of the new node
                            temp_c = country_head;
                            while(temp_c->next != NULL)
                                temp_c = temp_c->next;

                            temp_c->next = node_c;
                            node_c->next = NULL;                                                //places new country node in the correct position
                        }
                    }
                }
            }

            else if(num == 8){                                                      //case: citizenID firstName lastName country age virusName NO
                if(strcmp(Command[7], "YES") == 0 || atoi(Command[7]) > 0)          //checking for error in command's format
                    printf("ERROR!\n");

                else{
                    temp_v = virus_head;
                    while(temp_v != NULL){
                        if(strcmp(temp_v->virus_name, Command[6]) == 0){
                            if(skiplist_search(temp_v->vaccinated_persons, Command[1]) == 1){ //check for existance in vaccinated or not vaccinated skiplist
                                temp = head->next;
                                while(temp != NULL){
                                    if(strcmp(temp->id, Command[1]) == 0){
                                        printf("ERROR: CITIZEN %s ALREADY VACCINATED ON %s \n", temp->id, temp->date);
                                        flag = 1;
                                        break;
                                    }
                                    else
                                        temp = temp->next; 
                                }
                            }
                            if(skiplist_search(temp_v->not_vaccinated_persons, Command[1]) == 1){
                                printf("ALREADY EXISTS IN NOT VACCINATED PEOPLE!\n");         //prints error for existance
                                flag = 1;
                                break;
                            }
                        }
                        temp_v = temp_v->next; 
                    }

                    if(flag == 0){              //check for new record who's about to get inserted as vaccinated
                        temp_v = virus_head;
                        while(temp_v != NULL){
                            if(strcmp(temp_v->virus_name, Command[6]) == 0){ 
                                flag = 2;
                                break;
                            }
                            temp_v = temp_v->next; 
                        }

                        temp = head->next;
                        while(temp->next != NULL)
                            temp = temp->next;
                        
                        record* new = (record*)malloc(sizeof(record));
                        push_record(head->next, new, Command);

                        new->yesNo = (char*)malloc(sizeof(char) * 60);
                        new->date = (char*)malloc(sizeof(char) * 60);
                        
                        strcpy(new->yesNo, "NO");
                        strcpy(new->date, "-");
                        
                        temp->next = new;
                        new->next = NULL;               //creates new record and gets negative vaccination information

                        if(flag == 2){
                            temp_v = virus_head;
                            while(temp_v != NULL){
                                if(strcmp(temp_v->virus_name, Command[6]) == 0){
                                    skiplist_insert(temp_v->not_vaccinated_persons, Command[1]);    //inserts record in not vaccinated list of virus
                                    printf("RECORD SUCCESFULLY INSERTED!\n");                       //informs user for successful insertion
                                    break;
                                }
                                else    
                                    temp_v = temp_v->next;
                            }
                        }

                        else{
                            virus* node_v = (virus*)malloc(sizeof(virus));
                            virus_push(node_v, new);                                           //create new virus node for the virus
                            temp_v = virus_head;
                            while(temp_v->next != NULL)
                                temp_v = temp_v->next;
                            
                            temp_v->next = node_v;
                            node_v->next = NULL;                                                //place the virus node on the correct position

                            skiplist_insert(node_v->not_vaccinated_persons, new->id);      //if not insert it in the not vaccinated skiplist of the virus
                            
                            printf("RECORD SUCCESFULLY INSERTED\n");
                        }
                        
                        temp_c = country_head;
                        while(temp_c != NULL){
                            if(strcmp(temp_c->country, Command[4]) == 0){ 
                                flag = 3;
                                break;
                            }
                            temp_c = temp_c->next; 
                        }
                        
                        if(flag != 3){
                            country* node_c = (country*)malloc(sizeof(country));   
                            node_c->country = (char*)malloc(sizeof(char) * 60);

                            strcpy(node_c->country, Command[4]);                                //sets name of country in new node
                            country_push(node_c, head->next);                                  //calculates the totals of vaccinated and not vaccinated people in country of the new node
                            temp_c = country_head;
                            while(temp_c->next != NULL)
                                temp_c = temp_c->next;

                            temp_c->next = node_c;
                            node_c->next = NULL;                                                //places new country node in the correct position
                        }

                    }
                }
            }

            else                            //for any incompatible format not checked above
                printf("ERROR!\n");         //give an error

            if(num != 0){
                for(int i = 0; i < num; i++)
                    free(Command[i]);
                free(Command);
            }
        }
        
        /* vaccinateNow */
        if(strstr(buffer, "vaccinateNow") != 0){
            while(buffer[i] != '\n'){
                if(buffer[i] == ' ')
                    counter++;
                i++;
            }

            if(counter == 6)
                num = 7;
            else    
                num = 0;                                    //used to check many cases of command
            
            if(num != 0)
                Command = get_command(buffer, num);         //splits command in array to be used in comparisons

            char* todays_date = (char*)malloc(sizeof(char) * 60);
            
            time_t current = time(NULL);
            struct tm* local = localtime(&current); 
            strftime(todays_date, 20, "%d-%m-%Y", local);       //gets today's date

            int flag1 = 0;
            
            if(num == 7){               //case: citizenID firstName lastName country age virusName
                temp_v = virus_head;
                while(temp_v != NULL){
                    if(strcmp(temp_v->virus_name, Command[6]) == 0 && skiplist_search(temp_v->vaccinated_persons, Command[1]) == 1){  //checks for existance in vaccinated skiplist of virus
                        temp = head->next;
                        while(temp != NULL){
                            if(strcmp(temp->id, Command[1]) == 0){
                                printf("ERROR: CITIZEN %s ALREADY VACCINATED ON %s \n", temp->id, temp->date);
                                flag = 1;
                                break;
                            }
                            else
                                temp = temp->next; 
                        }
                    }
                    temp_v = temp_v->next; 
                }

                if(flag == 0){              //check for non existant record who's about to get inserted as vaccinated on today's date
                    temp_v = virus_head;
                    while(temp_v != NULL){
                        if(strcmp(temp_v->virus_name, Command[6]) == 0){ 
                            flag = 2;
                            break;
                        }
                        temp_v = temp_v->next; 
                    }

                    if(flag == 2){
                        temp_v = virus_head;
                        while(temp_v != NULL){
                            if(strcmp(temp_v->virus_name, Command[6]) == 0 && skiplist_search(temp_v->not_vaccinated_persons, Command[1]) == 1){            //check for existant record who's not vaccinated
                                temp = head->next;
                                while(temp != NULL){
                                    if(strcmp(temp->id, Command[1]) == 0){
                                        strcpy(temp->yesNo, "YES");
                                        strcpy(temp->date, todays_date);                //changes record's vaccination information as yes with the today's date
                                    }
                                    temp = temp->next;
                                }
                            
                                temp_b = bloom_head;
                                while(temp_b != NULL){
                                    if(strcmp(temp_b->virus, Command[6]) == 0){
                                        bloom_insert(temp_b, Command[1], bloomSize);        //inserts record to bloom filter
                                        break;
                                    }
                                    else
                                        temp_b = temp_b->next;
                                }

                                skiplist_insert(temp_v->vaccinated_persons, Command[1]);        //inserts record to vaccinated skiplist
                                skiplist_delete(temp_v->not_vaccinated_persons, Command[1]);    //deletes record from not vaccinated skiplist

                                flag1 = 1;

                                printf("RECORD SUCCESFULLY INSERTED\n");                        //informs user for successful insertion

                                break;
                            }
                            temp_v = temp_v->next;
                        }

                        if(flag1 == 0){             //check for new record who's about to be inserted as vaccinated on today's date
                            temp = head->next;
                            while(temp->next != NULL)
                                temp = temp->next;
                            
                            record* new = (record*)malloc(sizeof(record));
                            push_record(head->next, new, Command);

                            new->yesNo = (char*)malloc(sizeof(char) * 60);
                            strcpy(new->yesNo, "YES");

                            new->date = (char*)malloc(sizeof(char) * 60);
                            strcpy(new->date, todays_date);

                            temp->next = new;
                            new->next = NULL;       //creates new record with positive vaccination information

                            temp_v = virus_head;
                            while(temp_v != NULL){
                                if(strcmp(temp_v->virus_name, Command[6]) == 0){
                                    skiplist_insert(temp_v->vaccinated_persons, Command[1]);        //inserts record in vaccinated skiplist
                                    printf("RECORD SUCCESFULLY INSERTED!\n");                       //informs user for successful insertion
                                    break;
                                }
                                else    
                                    temp_v = temp_v->next;
                            }
                        }
                    }

                    else if(flag != 2){
                        temp = head->next;
                        while(temp->next != NULL)
                            temp = temp->next;
                        
                        record* new = (record*)malloc(sizeof(record));
                        push_record(head->next, new, Command);

                        new->yesNo = (char*)malloc(sizeof(char) * 60);
                        new->date = (char*)malloc(sizeof(char) * 60);
                    
                        strcpy(new->yesNo, "YES");
                        strcpy(new->date, todays_date);
                
                        temp->next = new;
                        new->next = NULL;                       //creates new record and insert information of vaccination

                        bloom* node_b = (bloom*)malloc(sizeof(bloom));
                        bloom_push(node_b, new, bloomSize);                //create new bloom node with the virus
                        temp_b = bloom_head;
                        while(temp_b->next != NULL)
                            temp_b = temp_b->next;
                        
                        temp_b->next = node_b;
                        node_b->next = NULL;                                //place new bloom node in the correct position

                        virus* node_v = (virus*)malloc(sizeof(virus));
                        virus_push(node_v, new);                                           //create new virus node for the virus
                        temp_v = virus_head;
                        while(temp_v->next != NULL)
                            temp_v = temp_v->next;
                        
                        temp_v->next = node_v;
                        node_v->next = NULL;                                                //place the virus node on the correct position

                        skiplist_insert(node_v->vaccinated_persons, new->id);          //if yes insert it in the vaccinated skiplist of the virus
                        
                        printf("RECORD SUCCESFULLY INSERTED\n");
                    }
                    
                    temp_c = country_head;
                    while(temp_c != NULL){
                        if(strcmp(temp_c->country, Command[4]) == 0){ 
                            flag = 3;
                            break;
                        }
                        temp_c = temp_c->next; 
                    }

                    if(flag != 3){
                        country* node_c = (country*)malloc(sizeof(country));   
                        node_c->country = (char*)malloc(sizeof(char) * 60);

                        strcpy(node_c->country, Command[4]);                                //sets name of country in new node
                        country_push(node_c, head->next);                                  //calculates the totals of vaccinated and not vaccinated people in country of the new node
                        temp_c = country_head;
                        while(temp_c->next != NULL)
                            temp_c = temp_c->next;

                        temp_c->next = node_c;
                        node_c->next = NULL;                                                //places new country node in the correct position
                    }
                }
            }

            else                        //for any incompatible format not checked above 
                printf("ERROR!\n");     //give an error

            if(num != 0){
                for(int i = 0; i < num; i++)
                    free(Command[i]);
                free(Command);
            }

            free(todays_date);
        }

        /* list-nonVaccinated-Persons */
        if(strstr(buffer, "list-nonVaccinated-Persons") != 0){
            
            while(buffer[i] != '\n'){
                if(buffer[i] == ' ')
                    counter++;
                i++;
            }

            if(counter == 1)
                num = 2;
            else    
                num = 0;                                //used to check many cases of command
            
            if(num != 0)
                Command = get_command(buffer, num);     //splits command in array to be used in comparisons

            if(num == 2){                   //case: virus
                temp_v = virus_head;
                while(temp_v != NULL){
                    if(strcmp(temp_v->virus_name, Command[1]) == 0){                                    //find compatible virus
                        temp = head->next;
                        while(temp != NULL){
                            if(skiplist_search(temp_v->not_vaccinated_persons, temp->id) == 1 && strcmp(temp->virus, Command[1]) == 0){                                             //if id in vaccinated skiplist of virus
                                printf("%s %s %s %s %s\n", temp->id, temp->firstName, temp->lastName, temp->country, temp->age);            //print out the information asked
                                flag = 1;
                            }
                            temp = temp->next;
                        }
                    }
                    temp_v = temp_v->next;
                }

                if(flag == 0)              //if record doesn't exist or is in non vaccinated skiplist
                    printf("ERROR! NON EXISTANT VIRUS OR NO NON-VACCINATED PEOPLE!\n");
            }
            
            else                        //for any incompatible format not checked above
                printf("ERROR!\n");     //give an error

            if(num != 0){
                for(int i = 0; i < num; i++)
                    free(Command[i]);
                free(Command);
            }
        }

    } 
    
    bloom_free(bloom_head);
    free_country(country_head);
    free_virus(virus_head);
    free_record(head);

    return 0;
}