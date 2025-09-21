#include <stdio.h>

#include "def.h"
#include "global_utils.h"
#include "structs.h"

int ft_strlen(char *s) {
	if (!s) return (0);

	int len = 0;
	while (s[len])
		len++;
	return (len);
}

void print_verbose(prog_data_t *program_data, char *message) {
	if (program_data->options.verbose)
		fprintf(stderr, "%s", message);
}

void bzero_prog_data(prog_data_t *program_data) {
	size_t i = 0;
	char *set = (char *)program_data;

	while (i < sizeof(*program_data)) {
		set[i] = 0;
		i++;
	}
}

int is_digit(char c) {
	if (c >= '0' && c <= '9')
		return (SUCCESS);
	return (FAILURE);
}