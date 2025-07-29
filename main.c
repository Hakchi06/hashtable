#include <stdio.h>
#include "hash_table.h"

int main() {
	ht_hash_table* ht = ht_new_table();

	// Insert items into the hash table
	ht_insert(ht, "Alice", "01b9f3ae8d4a4c0bbf98e1b9cd7a3eaf");
	ht_insert(ht, "Max", "73cd4a6f1dc24805ab2de2cc857d317e");
	ht_insert(ht, "Leo", "9a6efb2c7e6342dcb1c5eac88f02b4ee");
	
	ht_insert(ht, "clave29", "this value is entered first");
	ht_insert(ht, "clave51", "this value overwrites the value of clave29");

	// Print the items in the hash table
	printf("Hash Table Items:\n");
	for (int i = 0; i < ht->size; i++) {
		ht_item* item = ht->items[i];
		if (item != NULL) {
			printf("Index: %d, Value: %s\n", i, item->value);
		}
	}

	char keys[][20] = {
		"Alice",
		"Max",
		"Leo",
		"clave29",
		"clave51"
	};
	
	// Search for keys in the hash table
	printf("\nSearching for keys:\n");
	int lenght = sizeof(keys) / sizeof(keys[0]);
	for (int i = 0; i < lenght; i++) {
		char* value = ht_search(ht, keys[i]);
		printf("Key: %s, Value: %s\n", keys[i], value);
		
	}

	ht_delete(ht, "clave29");
	// Print the items in the hash table after deletion
	printf("\nHash Table Items:\n");
	for (int i = 0; i < ht->size; i++) {
		ht_item* item = ht->items[i];
		if (item != NULL) {
			printf("Index: %d, Value: %s\n", i, item->value);
		}
	}

	// delete the hash table
	ht_del_hash_table(ht);

	return 0;
}