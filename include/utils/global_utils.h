#pragma once

#include <stdio.h>

#include "structs.h"

int ft_strlen(char *s);

void print_verbose(prog_data_t *program_data, char *message);
void bzero_prog_data(prog_data_t *program_data);