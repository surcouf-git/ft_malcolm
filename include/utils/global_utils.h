#pragma once

#include <stdio.h>

#include "structs.h"

int ft_strlen(const char *s);
int is_digit(char c);
void ft_strcpy(char *s1, char *s2);
int ft_strcmp(const char *s1, const char *s2);
int ft_classcmp(const char *s1, const char *s2);

void print_verbose(prog_data_t *program_data, char *message);
void bzero_data(void *program_data, size_t size);
