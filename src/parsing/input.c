#include <stdlib.h>
#include <stdio.h>

#include "global_utils.h"
#include "structs.h"
#include "def.h"

int is_ip_valid(char *src_ip, char *trgt_ip, prog_data_t *program_data) {
	(void)src_ip;
	(void)trgt_ip;
	(void)program_data;
	return(SUCCESS);
}

int is_delimiter_valid(char *mac, size_t pos) {
	if (mac[pos] != '-' && mac[pos] != ':') {
		fprintf(stderr, EBADMAC, mac);
		return (FAILURE);
	}
	return(SUCCESS);
}

int is_char_valid(char *mac, size_t pos) {
	char c = mac[pos];

	if (is_digit(c))
		return (SUCCESS);
	else if ((c < 'a' || c > 'f') && (c < 'A' || c > 'F')) {
		fprintf(stderr, EBADMAC, mac);
		return (FAILURE);
	}
	return (SUCCESS);
}

int is_mac_valid(char *src_mac, char *trgt_mac, prog_data_t *program_data) {
	size_t i = 0;

	while (trgt_mac[i] && src_mac[i]) {
		if (IS_DELIMITER) {
			if (!is_delimiter_valid(src_mac, i) || !is_delimiter_valid(trgt_mac, i))
				return (FAILURE);
		} else {
			if (!is_char_valid(src_mac, i) || !is_char_valid(trgt_mac, i))
				return (FAILURE);
		}
		i++;
	}
	if (i != MACLEN) {
		if (!trgt_mac[i])
			fprintf(stderr, EBADMAC, trgt_mac);
		else
			fprintf(stderr, EBADMAC, src_mac);
		return (FAILURE);
	}
	(void)program_data;
	return(SUCCESS);
}

int is_valid_input(char **argv, prog_data_t *program_data) {
	if (!is_ip_valid(argv[1], argv[3], program_data))
		return (FAILURE);
	if (!is_mac_valid(argv[2], argv[4], program_data))
		return (FAILURE);
	return (SUCCESS);
}

int is_valid_opt(char *s) {
	if (ft_strlen(s) != 2) {
		fprintf(stderr, EOPT, s);
		return (FAILURE);
	}

	if (s[0] != '-') {
		fprintf(stderr, EOPT, s);
		return(FAILURE);
	}

	int i = 0;
	while (VALID_OPTS[i]) {
		if (s[1] == VALID_OPTS[i])
			return (SUCCESS);
		i++;
	}
	fprintf(stderr, EOPT, s);
	return(FAILURE);
}

void add_opt(char *c, prog_data_t *program_data) {
	if (c[1] == 'v')
		program_data->options.verbose = 1;
	if (c[1] == 'i')
		program_data->options.indepth = 1;
}

int are_valid_opts(int argc, char **argv, prog_data_t *program_data) {
	if (argc == 5)
		return (SUCCESS);
	
	int i = 5;
	while (i < argc) {
		if (is_valid_opt(argv[i]))
			add_opt(argv[i], program_data);
		i++;
	}
	return (SUCCESS);
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

	for (int i = MACLEN - 1; i >= 0; i--) {
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

void format_mac_addresses(char **argv, prog_data_t *program_data) {
	char *raw_src_mac = argv[2];
	char *raw_trgt_mac = argv[4];

	program_data->args.dec_src_mac = ascii_to_hex(raw_src_mac);
	program_data->args.dec_trgt_mac = ascii_to_hex(raw_trgt_mac);
	if (program_data->options.verbose) {
		printf(MACFORM, 
				program_data->args.dec_src_mac, program_data->args.dec_src_mac,
				program_data->args.dec_trgt_mac, program_data->args.dec_trgt_mac);
	}
}

int parse_input(int argc, char **argv, prog_data_t *program_data) {
	if (!are_valid_opts(argc, argv, program_data))
		return (FAILURE);
	if (!is_valid_input(argv, program_data))
		return (FAILURE);
	format_mac_addresses(argv, program_data);
	return (SUCCESS);
}

int init_program(int argc, char **argv, prog_data_t *program_data) {

	bzero_prog_data(program_data);

	if (!parse_input(argc, argv, program_data))
		return (FAILURE);
	return (SUCCESS);
}