#include "global_utils.h"
#include "structs.h"
#include <stdio.h>

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