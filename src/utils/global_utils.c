#include <stdio.h>
#include <signal.h>
#include <strings.h>
#include <arpa/inet.h>

#include "def.h"
#include "global_utils.h"
#include "structs.h"

void puthex(unsigned char *data, int n) {
	int i = 0;
	while (i < n) {
		printf("%.2X ", data[i]);
		i++;
	}
}

int ft_strlen(const char *s) {
	if (!s) return (0);

	int len = 0;
	while (s[len])
		len++;
	return (len);
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

void ft_ustrcpy(uint8_t *s1, const uint8_t *s2) {
	if (!s2)
		return ;
	for (int i = 0; s2[i]; i++)
		s1[i] = s2[i];
}

void ft_ustrncpy(uint8_t *dest, const uint8_t *src, size_t n) {
	if (!src || !dest)
		return;
		
	size_t i = 0;
	while (i < n && src[i] != '\0') {
		dest[i] = src[i];
		i++;
	}
		
	while (i < n) {
		dest[i] = '\0';
		i++;
	}
}

int ft_umac_cmp(const uint8_t *s1, const uint8_t *s2) {
	if (!s1 || !s2)
		return (0);
		
	uint8_t bufs1[BYTE_MAC_LEN];
	uint8_t bufs2[BYTE_MAC_LEN];
		
	ft_ustrncpy(bufs1, s1, BYTE_MAC_LEN);
	ft_ustrncpy(bufs2, s2, BYTE_MAC_LEN);
		
	for (int i = 0; i < BYTE_MAC_LEN; i++) {
		if (bufs1[i] >= 'a' && bufs1[i] <= 'f')
			bufs1[i] = bufs1[i] - 32;
		if (bufs2[i] >= 'a' && bufs2[i] <= 'f')
			bufs2[i] = bufs2[i] - 32;
	}

	for (int i = 0; i < BYTE_MAC_LEN; i++) {
		if (bufs1[i] != bufs2[i])
			return (0);
	}
	return (1);
}

int ft_strcmp(const char *s1, const char *s2) {
	if (!s1 || !s2)
		return (0);

	for (int i = 0; s1[i] || s2[i]; i++) {
		if (s1[i] != s2[i])
			return (0);
	}
	return (ft_strlen(s1) == ft_strlen(s2));
}

int ft_strncmp(const char *s1, const char *s2, int n) {
	if (!s1 || !s2)
		return (FAILURE);

	int i = 0;
	while (s1[i] || s2[i]) {
		if (i == n)
			return (SUCCESS);
		if (s1[i] != s2[i])
			return (FAILURE);
		i++;
	}
	return (ft_strlen(s1) == ft_strlen(s2));
}

static int is_class_b_range(const char *number) {
	if (ft_strlen(number) < 2)
		return(FAILURE);

	if (number[2] && number[2] != '.')
		return (FAILURE);

	int range = (((number[0] - '0') * 10 ) + (number[1] - '0'));

	if (range < 16 || range > 31)
		return (FAILURE);

	return (SUCCESS);
}

int ft_classcmp(const char *class, const char *arg) {
	int i = 0;

	while (class[i]) {

		if (i == 4 && ft_strncmp(arg, "172", 3)) {
			if (is_class_b_range(arg + i))
				return (SUCCESS);
		}

		if (arg[i] != class[i])
			return (FAILURE);
		i++;
	}
	return (SUCCESS);
}

// == fill mac_tab byte by bytes and return hex to dec value == //TODO clean code
uint64_t ascii_to_hex(const char *mac, uint8_t *mac_tab) {
	unsigned int char_hex_value[256] = {};
	
	char c = 'a';
	int start = MIN_CHAR_VAL;
	for (int i = 0; i < MAX_CHAR_RANGE; i++) {
		char_hex_value[(int)c] = start;
		char_hex_value[(int)(c - 32)] = start;
		c++;
		start++;
	}
	
	uint64_t result = 0, multiplier = 1;
	int is_even = 0, digit = 0, byte = 5;
	for (int i = MAC_LEN - 1; i >= 0; i--) {
		char c = mac[i];

		if (c == ':' || c == '-') {
			byte -= 1;
			is_even = 0;
			continue ;
		}

		if (is_digit(mac[i]))
			digit = mac[i] - '0';
		else
			digit = char_hex_value[(int)mac[i]];

		if (is_even) {
			mac_tab[byte] += digit * BASE_16;
		} else {
			mac_tab[byte] = digit;
			is_even = 1;
		}
		result += digit * multiplier;
		multiplier *= BASE_16;
	}
	return (result);
}

int has_signal = 0;

void sigint_handler(int sig) {
	if (sig == SIGINT) {
		printf("iao !\n");
		has_signal = SIGINT;
	}
}

void set_signal_action(void) {
	struct sigaction act;

	bzero_data(&act, sizeof(act));
	act.sa_handler = &sigint_handler;
	sigaction(SIGINT, &act, NULL);
}