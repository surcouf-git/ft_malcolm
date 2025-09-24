#include <stdio.h>

#include "def.h"
#include "global_utils.h"
#include "structs.h"

int ft_strlen(const char *s) {
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

void bzero_data(void *program_data, size_t size) {
	size_t i = 0;
	char *set = (char *)program_data;

	while (i < size) {
		set[i] = 0;
		i++;
	}
}

int is_digit(char c) {
	if (c >= '0' && c <= '9')
		return (SUCCESS);
	return (FAILURE);
}

// == Copy s2 in s1 ==
void ft_strcpy(char *s1, char *s2) {
	if (!s2)
		return ;
	while (*s2) {
		*s1 = *s2;
		s1++;
		s2++;
	}
}

int ft_strcmp(const char *s1, const char *s2) {
	if (!s1 || !s2)
		return (0);

	for (int i = 0; s1[i] && s2[i]; i++) {
		if (s1[i] != s2[i])
			return (0);
	}
	return (ft_strlen(s1) == ft_strlen(s2));
}

int ft_classcmp(const char *s1, const char *s2) {
	int i = 0;

	while (s1[i]) {
		if (s2[i] != s1[i])
			return (0);
		i++;
	}
	return (1);
}

unsigned long ascii_to_hex(char *mac) {
	unsigned long result = 0, digit = 0, multiplier = 1;

	char c = 'a';
	int start = MIN_CHAR_VAL;
	unsigned int char_hex_value[256] = {};
	for (int i = 0; i < MAX_CHAR_RANGE; i++) {
		char_hex_value[(int)c] = start;
		char_hex_value[(int)(c - 32)] = start;
		c++;
		start++;
	}

	for (int i = MAC_LEN - 1; i >= 0; i--) {
		char c = mac[i];

		if (c == ':' || c == '-')
			continue ;

		if (is_digit(mac[i]))
			digit = mac[i] - '0';
		else
			digit = char_hex_value[(int)mac[i]];

		result += digit * multiplier;
		multiplier *= BASE_16;
	}
	return (result);
}