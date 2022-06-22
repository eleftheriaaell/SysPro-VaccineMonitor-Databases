#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bloom.h"

/* functions given by instructor*/

unsigned long djb2(unsigned char *str) {
	unsigned long hash = 5381;
	int c; 
	while (c = *str++) {
		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
	}
	return hash;
}

unsigned long sdbm(unsigned char *str) {
	unsigned long hash = 0;
	int c;

	while (c = *str++) {
		hash = c + (hash << 6) + (hash << 16) - hash;
	}

	return hash;
}

unsigned long hash_i(unsigned char *str, unsigned int i) {
	return djb2(str) + i*sdbm(str) + i*i;
}

/* end of functions given by instructor */

void bit_change(int *bit_array, int bloomSize, unsigned char* id, unsigned int i){
	
	int hash = hash_i(id, i) % bloomSize;

	bit_array[hash] = 1;						//changes the bits for the id given with the assistance of the hash function

} 												


void bloom_insert(bloom* bloom_node, char* id, int bloomSize){   //vazi neo vaccinated se iparxon virus
	
	for(int i = 0; i < 16; i++)
		bit_change(bloom_node->bit_array, bloomSize, id, i);

}											//puts new vaccinated id in existant virus in bloom filter and changes its bits

void bloom_push(bloom* new, record* node, int bloomSize){

	new->virus = (char*)malloc(sizeof(char) * 60);
	strcpy(new->virus, node->virus);							
	
	new->bit_array = (int*)malloc(sizeof(int) * bloomSize);
	for(int i = 0; i < bloomSize; i++)
		new->bit_array[i] = 0;										//creates new virus in bloom filter and initializes its bit array with zeros
	
	for(int i = 0; i < 16; i++)
		bit_change(new->bit_array, bloomSize, node->id, i);			//changes its bit array for the id given
	
}  

int bloom_check(bloom* node, int bloomSize, unsigned char* id){
	
	int hash;
	
	for(int i = 0; i < 16; i++){
		hash = hash_i(id, i) % bloomSize;
		if(node->bit_array[hash] == 0){								//check if the bit array has zeros in position suited for the id 
			printf("-----NOT VACCINATED!-----\n");					//if it has at least one zero then its not vaccinated
			return 0;
		}
	}
	printf("-----MAYBE!-----\n");									//else it may be vaccinated
	return 0;

} 

void bloom_free(bloom* bloom_head){

	bloom* temp = bloom_head;
	while(temp != NULL){
		bloom_head = temp->next;
		free(temp->virus);
		free(temp->bit_array);
		free(temp);
		temp = bloom_head;
	}
	
}