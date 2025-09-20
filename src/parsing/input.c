#include <stdlib.h>

#include "global_utils.h"
#include "structs.h"
#include "def.h"

void bzero_prog_data(prog_data_t *program_data) {
	size_t i = 0;
	char *set = (char *)program_data;

	while (i < sizeof(*program_data)) {
		set[i] = 0;
		i++;
	}
}

int is_ip_valid(char *ip1, char *ip2, prog_data_t *program_data) {

}

int is_mac_valid(char *mac1, char *mac2, prog_data_t *program_data) {

}

int is_valid_input(char **argv, prog_data_t *program_data) {

	if (!is_ip_valid(argv[1], argv[3], program_data))
		return (FAILURE);
	if (!is_mac_valid(argv[2], argv[4], program_data))
		return (FAILURE);
}

int is_valid_opt(char *s) {
	if (ft_strlen(s) != 2)
		return (FAILURE);

	if (s[0] != '-') {
		fprintf(stderr, "invalid option -> {%s}, ignoring\n", s);
		return(FAILURE);
	}

	int i = 0;
	while (VALID_OPTS[i]) {
		if ()
	}
	return(SUCCESS);
}

void add_opt(char *c) {

}

int are_valid_opts(int argc, char **argv, prog_data_t *program_data) {
	if (argc == 5)
		return (SUCCESS);
	
	int i = 5;
	while (i < argc) {
		if (is_valid_opt(argv[i]))
			add_opt(argv[i]);
	}
}

int parse_input(int argc, char **argv, prog_data_t *program_data) {
	if (!are_valid_opts(argc, argv, program_data))
		return (FAILURE);
	if (!is_valid_input(argv, program_data))
		return (FAILURE);
}

int init_program(int argc, char **argv, prog_data_t *program_data) {

	bzero_prog_data(program_data);

	if (!parse_input(argc, argv, program_data))
		return (FAILURE);
	return (SUCCESS);
}