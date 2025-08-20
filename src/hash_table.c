#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "hash_table.h"

static ht_item HT_DELETED_ITEM = {NULL, NULL}; // A special item to mark deleted items

static ht_item* ht_new_item(const char* key, const char* value) {
	ht_item* item = malloc(sizeof(ht_item));
	item->key = strdup(key);
	item->value = strdup(value);
	return item;
}

ht_hash_table* ht_new_sized_table(const int size) {
	ht_hash_table* ht = malloc(sizeof(ht_hash_table));
	ht->size = size;
	ht->count = 0;
	ht->items = calloc((size_t)ht->size, sizeof(ht_item*));
	return ht;
}

ht_hash_table* ht_new_table(){
	return ht_new_sized_table(HT_BASE_SIZE);
}

// Functions to delete an item from the hash table and a hash table

static void ht_del_item(ht_item* item) {
	free(item->key);
	free(item->value);
	free(item);
}

void ht_del_hash_table(ht_hash_table* ht) {
	for (int i = 0; i < ht->size; i++) {
		ht_item* item = ht->items[i];
		if(item != NULL && item != &HT_DELETED_ITEM)
			ht_del_item(item);
	}
	free(ht->items);
	free(ht);
}

// Function to calculate the hash of a key

static int ht_hash(const char* key, const int m) {
	long hash = 0;
	for(int i = 0; i < strlen(key); i++) {
		hash = key[i] + (hash << 6) + (hash << 16) - hash;
	}
	int index = (int)(hash % m);
	return (int)(index < 0) ? -index : index; // Ensure the index is non-negative
}

// Functions to fix collisions in the hash table

int get_max(int a, int b) {
	return (a > b) ? a : b;
}

static int ht_get_next_index(int index, const int i, const int m, const char* key) {
	/*
	* This function calculates the next index in the hash table
	* It's important that the size of the hash table is a power of two
	* and step (get_max(1, sumK/m)) is an odd number
	*/

	int sumK = 0; // Sum of ASCII values of the key characters
	for(int k = 0; k < strlen(key); k++) {
		sumK += key[k];
	}
	int step = get_max(1, sumK/m);
	
	if(step % 2 == 0) step++; // Ensure step is odd
	
	index = (index + step * i) % m;
	return index;
}


// Functions to resize the hash table

static void ht_resize(const int new_size, ht_hash_table* ht){
	ht_hash_table* new_ht = ht_new_sized_table(new_size);

	for (int i = 0; i < ht->size; i++) {
		ht_item* item = ht->items[i];
		if (item != NULL && item != &HT_DELETED_ITEM) {
			ht_insert(new_ht, item->key, item->value);
		}
    }

	// now, we need to switch the content of the old hash table with the new one
	const int tmp_size = ht->size;
	ht->size = new_ht->size;
	new_ht->size = tmp_size;
	
	ht->count = new_ht->count;

	ht_item** tmp_items = ht->items;
	ht->items = new_ht->items;
	new_ht->items = tmp_items;

	ht_del_hash_table(new_ht); // Delete the new hash table
}

void ht_resize_up(ht_hash_table* ht){
	const int new_size = pow(2, log(ht->size)/log(2) + 1);  // 2^(actual_size + 1)
	ht_resize(new_size, ht);
}

void ht_resize_down(ht_hash_table* ht){
	const int new_size = pow(2, log(ht->size)/log(2) - 1);  // 2^(actual_size - 1)
	ht_resize(new_size, ht);
}

// Operations for the hash table
// Function to insert an item into the hash table

void ht_insert(ht_hash_table* ht, const char* key, const char* value) {
	ht_item* item = ht_new_item(key, value);
	int index = ht_hash(item->key, ht->size);
	int i = 0; // count of collisions
	int load = ht->count*100/ht->size;
	while (ht->items[index] != NULL) {
		i++;
		index = ht_get_next_index(index, i, ht->size, key);
	}
	ht->items[index] = item;
	ht->count++;

	if(load > 70){
		ht_resize_up(ht);
	}
}

// Function to delete an item from the hash table
void ht_delete(ht_hash_table* ht, const char* key) {
	int index = ht_hash(key, ht->size);
	int i = 0; // count of collisions
	int load = ht->count*100/ht->size;
	while(ht->items[index] != NULL) {
		ht_item* item = ht->items[index];
		if(strcmp(item->key, key) == 0 && item != &HT_DELETED_ITEM) {
			ht_del_item(item); // Delete the item
			ht->items[index] = &HT_DELETED_ITEM; // Mark the item as deleted
			ht->count--;
		}
		i++;
		index = ht_get_next_index(index, i, ht->size, key);
	}

	if(load < 30){
		ht_resize_down(ht);
	}
}

// Function to search for an item in the hash table
char* ht_search(ht_hash_table* ht, const char* key) {
	int index = ht_hash(key, ht->size);
	int i = 0; // count of collisions
	while(ht->items[index] != NULL) {
		ht_item* item = ht->items[index];
		if (strcmp(item->key, key) == 0 && item != &HT_DELETED_ITEM) {
			return item->value; // Return the value if the key matches
		}
		i++;
		index = ht_get_next_index(index, i, ht->size, key);
	}
	return NULL; // Return NULL if the key is not found
}
