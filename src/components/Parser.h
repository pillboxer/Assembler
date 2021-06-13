typedef enum command_type {
	ADD,
	COMP,
	LBL,
	UNKNOWN
} command_type_t;

int num_commands(const char *str);
void commands(command_type_t **ptr, const char *str);
void parse(char **ptr);
command_type_t command_type(const char* str);
