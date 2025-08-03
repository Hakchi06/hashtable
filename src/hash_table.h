#define HT_BASE_SIZE 64

// the key-value items
typedef struct {
	char* key;	// ASCI string
	char* value;
} ht_item;

// the hash table
typedef struct {
	int size;
	int count; // number of items in the hash table
	ht_item** items; // array of pointers to ht_item
} ht_hash_table;

ht_hash_table* ht_new_table(void);
void ht_insert(ht_hash_table* ht, const char* key, const char* value);
char* ht_search(ht_hash_table* ht, const char* key);
void ht_delete(ht_hash_table* ht, const char* key);