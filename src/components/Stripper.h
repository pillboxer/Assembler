#include "HashMap.h"
void strip_spaces(char* dst, const char* src);
void strip_comments(char* dst, const char* src);
void strip_labels(char* dst, const char* src, HashMap* hash_map);
void save_variables(char* dst, HashMap* hash_map); 
