typedef enum command_type {
	ADD,
	COMP,
	LBL,
	UNKNOWN
} command_type_t;

int num_commands(const char *str);
const char* parsed_a_command(const char* cmd);
