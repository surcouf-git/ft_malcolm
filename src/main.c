#include <stdio.h>
#include <stdlib.h>

#include "structs.h"
#include "def.h"
#include "input.h"
#include "main_loop.h"
#include "global_utils.h"

int main(int argc, char **argv) {
	if (argc < 5) {
		fprintf(stderr, EARG);
		return (EXIT_FAILURE);
	}

	prog_data_t program_data;

	set_signal_action();

	if (!init_program(argc, argv, &program_data))
		return (EXIT_FAILURE);
	
	if (!main_loop(&program_data))
		return (EXIT_FAILURE);

	return (0);
}