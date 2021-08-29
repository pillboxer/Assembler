#include <stdbool.h>
#define NUM_BUCKETS 1024

typedef struct Node {
	char* key;
	int value;
	struct Node *next;
} Node;

typedef struct {
	Node *buckets[NUM_BUCKETS];
} HashMap;

// Creates and returns a HashMap (caller to free)
HashMap* hash_map_create();
// Insert element(assume non-negative)
void hash_map_put(HashMap *hash_map, char *key, int value);
// Remove value for key 
void hash_map_remove(HashMap *hash_map, char *key);
// Returns the value for the key or -1 if it doesn't exist
int hash_map_get(HashMap *hash_map, char* key);
// Free HashMap
void hash_map_free(HashMap *hash_map);
// Check for existence 
bool hash_map_contains(HashMap* hash_map, char* key);
