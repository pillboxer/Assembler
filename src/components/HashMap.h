typedef struct {
	char* key;
	int value;
	Node *next;
}

typedef struct {
	Node *buckets[NUM_BUCKETS];
} HashMap;

HashMap hash_map_create();

void hash_map_put(HashMap *hash_map, char *key, int value);
void hash_map_remove(HashMap *hash_map, char *key);
void hash_map_get(HashMap *hash_map, char* key);
void hash_map_free(HashMap *hash_map);
