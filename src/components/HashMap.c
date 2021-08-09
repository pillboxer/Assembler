#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "HashMap.h"

static int hashed(char* arg) {
	// http://www.cse.yorku.ca/~oz/hash.html
	unsigned long hash = 5381;
	int c;
	while ((c = *arg++))
		hash = ((hash << 5) + hash) + c; 
	return hash % NUM_BUCKETS;
}

void print_list(Node* head, int bucket) {
	printf("BUCKET: %d\n\n", bucket);
	while(head != NULL) {
		printf("\t%s\t:\t%d\n", head->key, head->value);
		head = head->next;
	}
	printf("\n**************\n\n");
}

HashMap* hash_map_create() {
    HashMap *hash_map = malloc(sizeof(HashMap));
	for (int i = 0; i < NUM_BUCKETS; i++)
		hash_map->buckets[i] = NULL;
	return hash_map;
}

/** value will always be non-negative. */
void hash_map_put(HashMap* hash_map, char* key, int value) {
	int hashed_key = hashed(key);
	int result = hash_map_get(hash_map, key);
	if (result != -1) {
		Node *current = hash_map->buckets[hashed_key];
		while (current->key != key) {
			current = current->next;
		}
		current->value = value;
	}
	else {
		if (hash_map->buckets[hashed_key] == NULL) {
			hash_map->buckets[hashed_key] = malloc(sizeof(Node));
			hash_map->buckets[hashed_key]->key =  malloc(sizeof(char) * (strlen(key) + 1));
			strcpy(hash_map->buckets[hashed_key]->key, key);
			hash_map->buckets[hashed_key]->value = value;
			hash_map->buckets[hashed_key]->next = NULL;
		}
		else {
			Node *current = hash_map->buckets[hashed_key];
			Node *new = malloc(sizeof(Node));
			new->key = malloc(sizeof(char) * (strlen(key) + 1));
			strcpy(new->key, key);
			new->value = value;
			new->next = NULL;
			while (current->next != NULL) {
				current = current->next;
			}
			current->next = new;
		}
	}
	Node *head = hash_map->buckets[hashed_key];
	print_list(head, hashed_key);
}

/** Returns the value to which the specified key is mapped, or -1 if this map contains no mapping for the key */
int hash_map_get(HashMap* hash_map, char* key) {
  	int hashed_key = hashed(key);
	Node *current = hash_map->buckets[hashed_key];
	int returned = -1;
	while (current != NULL) {
		if (strcmp(current->key, key) == 0) {
			printf("Current value is %d\n", current->value);
			returned = current->value;
			break;
		}
	current = current->next;
	}
	return returned;
	
}

/** Removes the mapping of the specified value key if this map contains a mapping for the key */
void hash_map_remove(HashMap* hash_map, char* key) {
	int hashed_key = hashed(key);
	if (hash_map_get(hash_map, key) != -1) {
		Node *current = hash_map->buckets[hashed_key];
		if (current->key == key) {
			Node *new_current = current->next;
			current = NULL;
			hash_map->buckets[hashed_key] = new_current;
		}
		else {
			while (current->next->key != key) {
				current = current->next;
			}
			Node *old_next = current->next;
			Node *new_next = current->next->next;
			current->next = new_next;
			old_next = NULL;
		}
		print_list(hash_map->buckets[hashed_key], hashed_key);
	}
}

void hash_map_free(HashMap* hash_map) {
    free(hash_map);
}

