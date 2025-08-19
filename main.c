#include <stdio.h>
#include "hash_table.h"


/*You can use the following text to test the hash table implementation (copy&paste into the console):
1
apple
red
1
banana
yellow
1
grape
purple
1
lemon
bright
1
lime
green
1
melon
orange
1
peach
blush
1
pear
fresh
1
plum
deep
1
kiwi
tart
1
mango
juicy
1
berry
sweet
1
cherry
crisp
1
fig
soft
1
date
dry
*/

int main() {
	ht_hash_table* ht = ht_new_table();
	char key[256];
	char value[256];
	int option = 0;

	while (option != 5) {
		printf("Options:\n");
		printf("1. Insert key-value pair\n");
		printf("2. Search for a key\n");
		printf("3. Delete a key\n");
		printf("4. Print hash table\n");
		printf("5. Exit\n");
		printf(">>: ");

		scanf("%d", &option);

		if(option == 1) {
			printf("Enter key: ");
			scanf("%s", key);

			printf("Enter value: ");
			scanf("%s", value);

			ht_insert(ht, key, value);
			printf("Inserted (%s, %s)\n", key, value);

		} else if(option == 2) {
			printf("Enter key to search: ");
			scanf("%s", key);

			char* value = ht_search(ht, key);
			if(value) {
				printf("Value: %s\n", value);
			} else {
				printf("Key not found.\n");
			}

		} else if(option == 3) {
			printf("Enter key to delete: ");
			scanf("%s", key);

			ht_delete(ht, key);
			printf("Deleted key: %s\n", key);

		} else if(option == 4) {
			printf("Hash Table Items:\n");
			for (int i = 0; i < ht->size; i++) {
				ht_item* item = ht->items[i];
				if (item != NULL) {
					printf("Index: %d, Key: %s, Value: %s\n", i, item->key, item->value);
				}
			}
			printf("Hash table size: %d\n", ht->size);
			printf("Hash table count: %d\n", ht->count);

		} else if(option == 5) {
			printf("Exiting...\n");
		} else {
			printf("Invalid option. Please try again.\n");
		}

		// Clear the input buffer
		int c;
		while ((c = getchar()) != '\n' && c != EOF);
	}
	ht_del_hash_table(ht);

	return 0;
}