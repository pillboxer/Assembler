#define NUM_BUCKETS 1024

typedef struct Node {
	char* key;
	int value;
	struct Node *next;
} Node;

typedef struct {
	Node *buckets[NUM_BUCKETS];
} HashMap;

HashMap* hash_map_create();

void hash_map_put(HashMap *hash_map, char *key, int value);
void hash_map_remove(HashMap *hash_map, char *key);
int hash_map_get(HashMap *hash_map, char* key);
void hash_map_free(HashMap *hash_map);
