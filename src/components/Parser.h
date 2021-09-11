typedef enum command_type {
	ADD,
	COMP,
	LBL,
	UNKNOWN
} command_type_t;

void parse(char* dst, const char* src, HashMap* hash_map);
bool is_integral_string(char* str);
