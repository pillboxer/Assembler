typedef enum command_type {
	ADD,
	COMP,
	LBL
} command_type_t;

void commands(command_type_t **ptr, const char *str);
void parse(char **ptr);
