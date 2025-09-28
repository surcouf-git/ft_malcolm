#pragma once

#include <stdio.h>

#include "structs.h"

int ft_strlen(const char *s);
void puthex(unsigned char *data, int n);
int is_digit(char c);
void ft_strcpy(char *s1, char *s2);
int ft_umac_cmp(const uint8_t *s1, const uint8_t *s2);
int ft_strcmp(const char *s1, const char *s2);
int ft_strncmp(const char *s1, const char *s2, int n);
int ft_classcmp(const char *s1, const char *s2);
void ft_ustrcpy(uint8_t *s1, const uint8_t *s2);

void bzero_data(void *program_data, size_t size);

uint64_t ascii_to_hex(const char *mac, uint8_t *mac_tab);

void set_signal_action(void);