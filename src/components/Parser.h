typedef enum command_type {
	ADD,
	COMP,
	LBL,
	UNKNOWN
} command_type_t;

// Parse a source file that has been stripped of comments and spaces
void parse(char* dst, char* src, HashMap* hash_map);
bool is_integral_string(char* str);
