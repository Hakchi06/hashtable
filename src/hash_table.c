#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "hash_table.h"

static ht_item HT_DELETED_ITEM = {NULL, NULL}; // A special item to mark deleted items

static ht_item* ht_new_item(const char* key, const char* value) {
	ht_item* item = malloc(sizeof(ht_item));
	item->key = _strdup(key);
	item->value = _strdup(value);
	return item;
}

ht_hash_table* ht_new_table() {
	ht_hash_table* ht = malloc(sizeof(ht_hash_table));
	ht->size = HT_BASE_SIZE;
	ht->count = 0;
	ht->items = calloc((size_t)ht->size, sizeof(ht_item*));
	return ht;
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
		if(item != NULL) // there is an item in this position
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

// Operations for the hash table
// Function to insert an item into the hash table

void ht_insert(ht_hash_table* ht, const char* key, const char* value) {
	ht_item* item = ht_new_item(key, value);
	int index = ht_hash(item->key, ht->size);
	ht->items[index] = item;
	ht->count++;
}

// Function to delete an item from the hash table
void ht_delete(ht_hash_table* ht, const char* key) {
	int index = ht_hash(key, ht->size);
	ht_item* item = ht->items[index];

	if (item != NULL) {
		ht_del_item(item); // Delete the item
		ht->items[index] = &HT_DELETED_ITEM; // Mark the item as deleted
		ht->count--;
	}
}

// Function to search for an item in the hash table
char* ht_search(ht_hash_table* ht, const char* key) {
	int index = ht_hash(key, ht->size);
	ht_item* item = ht->items[index];
	if (item != NULL) {	// this expression will be used in the next update && strcmp(item->key, key) == 0 
		return item->value;
	}
}
