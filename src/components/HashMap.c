#include <stdlib.h>
#include <stdio.h>
#include "HashMap.h"

static int hashed(int arg) {
	return arg % NUM_BUCKETS;
}

void print_list(Node* head, int bucket) {
	printf("BUCKET: %d\n\n", bucket);
	while(head != NULL) {
		printf("\t%d\t:\t%d\n", head->key, head->value);
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

///** value will always be non-negative. */
//void myHashMapPut(MyHashMap* obj, int key, int value) {
//	int hashed_key = hashed(key);
//	int result = myHashMapGet(obj, key);
//	if (result != -1) {
//		Node *current = obj->buckets[hashed_key];
//		while (current->key != key) {
//			current = current->next;
//		}
//		current->value = value;
//	}
//	else {
//		if (obj->buckets[hashed_key] == NULL) {
//			obj->buckets[hashed_key] = malloc(sizeof(Node));
//			obj->buckets[hashed_key]->key = key;
//			obj->buckets[hashed_key]->value = value;
//			obj->buckets[hashed_key]->next = NULL;
//		}
//		else {
//			Node *current = obj->buckets[hashed_key];
//			Node *new = malloc(sizeof(Node));
//			new->key = key;
//			new->value = value;
//			new->next = NULL;
//			while (current->next != NULL) {
//				current = current->next;
//			}
//			current->next = new;
//		}
//	}
//	Node *head = obj->buckets[hashed_key];
//	print_list(head, hashed_key);
//}
//
///** Returns the value to which the specified key is mapped, or -1 if this map contains no mapping for the key */
//int myHashMapGet(MyHashMap* obj, int key) {
//  	int hashed_key = hashed(key);
//	Node *current = obj->buckets[hashed_key];
//	int returned = -1;
//	while (current != NULL) {
//		if (current->key == key) {
//			returned = current->value;
//			break;
//		}
//	current = current->next;
//	}
//	return returned;
//	
//}
//
///** Removes the mapping of the specified value key if this map contains a mapping for the key */
//void myHashMapRemove(MyHashMap* obj, int key) {
//	int hashed_key = hashed(key);
//	if (myHashMapGet(obj, key) != -1) {
//		Node *current = obj->buckets[hashed_key];
//		if (current->key == key) {
//			Node *new_current = current->next;
//			current = NULL;
//			obj->buckets[hashed_key] = new_current;
//		}
//		else {
//			while (current->next->key != key) {
//				current = current->next;
//			}
//			Node *old_next = current->next;
//			Node *new_next = current->next->next;
//			current->next = new_next;
//			old_next = NULL;
//		}
//		print_list(obj->buckets[hashed_key], hashed_key);
//	}
//}
//
//void myHashMapFree(MyHashMap* obj) {
//    free(obj);
//}
//
