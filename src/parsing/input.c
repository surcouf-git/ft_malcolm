#include <stdlib.h>
#include <stdio.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "global_utils.h"
#include "structs.h"
#include "def.h"

int is_delimiter_valid(const char *mac, size_t pos) {
	if (mac[pos] != '-' && mac[pos] != ':') {
		fprintf(stderr, EBADMAC, mac);
		return (FAILURE);
	}
	return(SUCCESS);
}

int is_char_valid(const char *mac, size_t pos) {
	char c = mac[pos];

	if (is_digit(c))
		return (SUCCESS);
	else if ((c < 'a' || c > 'f') && (c < 'A' || c > 'F')) {
		fprintf(stderr, EBADMAC, mac);
		return (FAILURE);
	}
	return (SUCCESS);
}

int is_mac_valid(const char *src_mac, const char *trgt_mac) {
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
	if (i != MAC_LEN) {
		if (!trgt_mac[i])
			fprintf(stderr, EBADMAC, trgt_mac);
		else
			fprintf(stderr, EBADMAC, src_mac);
		return (FAILURE);
	}
	return(SUCCESS);
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

void fill_prog_data(const char *raw_src_mac, const char *raw_trgt_mac, prog_data_t *program_data) {
	uint64_t *dec_src_mac = &(program_data->args.dec_src_mac), 
				*dec_trgt_mac = &(program_data->args.dec_trgt_mac);

	uint8_t *src_mac_tab = program_data->args.src_mac_tab,
				*trgt_mac_tab = program_data->args.trgt_mac_tab;

	*dec_src_mac = ascii_to_hex(raw_src_mac, src_mac_tab);
	*dec_trgt_mac = ascii_to_hex(raw_trgt_mac, trgt_mac_tab);
}

int format_mac_addresses(char **argv, prog_data_t *program_data) {
	const char *raw_src_mac = argv[2], 
				*raw_trgt_mac = argv[4];

	if (program_data->options.verbose) 
		printf(STARTMACFORM);

	if (!is_mac_valid(raw_src_mac, raw_trgt_mac))
		return (FAILURE);

	fill_prog_data(raw_src_mac, raw_trgt_mac, program_data);

	if (program_data->options.verbose) {
		printf(MACFORM, 
				program_data->args.dec_src_mac, program_data->args.dec_src_mac,
				program_data->args.dec_trgt_mac, program_data->args.dec_trgt_mac);
	}
	return (SUCCESS);
}

int convert_possible_hostname(char *src_ip, char *trgt_ip, prog_args_t *args) {
	struct addrinfo hints, *src_resp = 0, *trgt_resp = 0, *ptr = 0;
	char buffer[IP_MAX_LEN];
	int src_done = 0, ipv4_done = 0, ipv6_done = 0;

	bzero_data(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;

	if (getaddrinfo(src_ip, 0, &hints, &src_resp)) {
		fprintf(stderr, EBADIP, src_ip);
		return (FAILURE);
	}
	if (getaddrinfo(trgt_ip, 0, &hints, &trgt_resp)) {
		fprintf(stderr, EBADIP, trgt_ip);
		freeaddrinfo(src_resp);
		return (FAILURE);
	}

	ptr = src_resp;
	while (ptr) {

		if (ptr->ai_family == AF_INET && !ipv4_done) {

			struct sockaddr_in *ipv4 = (struct sockaddr_in *)ptr->ai_addr;
			if (!inet_ntop(AF_INET, &(ipv4->sin_addr), buffer, sizeof(buffer))) {
				fprintf (stderr, ENTOP);
				return (FAILURE);
			}
			src_done ? ft_strcpy((char *)args->trgt_ipv4, buffer) : ft_strcpy((char *)args->src_ipv4, buffer);
			bzero_data(buffer, IP_MAX_LEN);
			ipv4_done = 1;

		} else if (ptr->ai_family == AF_INET6 && !ipv6_done) {

			struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)ptr->ai_addr;
			if (!inet_ntop(AF_INET6, &(ipv6->sin6_addr), buffer, sizeof(buffer))) {
				fprintf (stderr, ENTOP);
				return (FAILURE);
			}
			src_done ? ft_strcpy((char *)args->trgt_ipv6, buffer) : ft_strcpy((char *)args->src_ipv6, buffer);
			bzero_data(buffer, IP_MAX_LEN);
			ipv6_done = 1;

		}
		ptr = ptr->ai_next;
		if (!ptr && !src_done) {
			ptr = trgt_resp;
			src_done = 1;
			ipv4_done = 0;
			ipv6_done = 0;
		}
	}
	freeaddrinfo(src_resp);
	freeaddrinfo(trgt_resp);
	return (SUCCESS);
}

int are_private_ips(const char *src_ip, const char *trgt_ip) {
	int src_is_valid = 0, trgt_is_valid = 0, i = 0;
	const int max_classes = 3;
	const char *classes[3] = { CLASS_A, CLASS_B, CLASS_C };

	while (i < max_classes) {
		if (ft_classcmp(classes[i], src_ip))
			src_is_valid = 1;
		if (ft_classcmp(classes[i], trgt_ip))
			trgt_is_valid = 1;
		i++;
	}
	if (!src_is_valid) {
		printf((ft_strcmp(src_ip, LOCALHOST) ? WHYLOCAL : WHYPUBLIC), src_ip);
		return (FAILURE);
	}
	if (!trgt_is_valid) {
		printf((ft_strcmp(trgt_ip, LOCALHOST) ? WHYLOCAL : WHYPUBLIC), trgt_ip);
		return (FAILURE);
	}
	return (SUCCESS);
}

// TODO 172.16 a un masque de 240... donc la deuxieme partie n'est pas forcement '16'

int is_valid_address(char **argv, prog_data_t *program_data) {
	const char *src_ip = (const char *)program_data->args.src_ipv4;
	const char *trgt_ip = (const char *)program_data->args.trgt_ipv4;

	if (program_data->options.verbose) {

		if (!ft_strcmp(src_ip, argv[1]))
			printf(HOSTTOIPSRC, argv[1], src_ip);

		if (!ft_strcmp(trgt_ip, argv[3]))
			printf(HOSTTOIPTRGT, argv[3], trgt_ip);
	}
	if (!are_private_ips(src_ip, trgt_ip)) {
		return (FAILURE);
	}
	return(SUCCESS);
}

int format_ip_addresses(char **argv, prog_data_t *program_data) {
	char *raw_src_ip = argv[1];
	char *raw_trgt_ip = argv[3];

	if (program_data->options.verbose)
		printf(STARTIPFORM);

	if (!convert_possible_hostname(raw_src_ip, raw_trgt_ip, &(program_data->args)))
		return (FAILURE);

	if (!is_valid_address(argv, program_data))
		return (FAILURE);
	
	program_data->args.dec_src_ip = inet_addr((char *)program_data->args.src_ipv4);
	program_data->args.dec_trgt_ip = inet_addr((char *)program_data->args.trgt_ipv4);

	if (program_data->options.verbose) {
		printf(VALID_IPS);
		printf(IPFORM, 
			program_data->args.src_ipv4, program_data->args.dec_src_ip, 
			program_data->args.trgt_ipv4, program_data->args.dec_trgt_ip);
	}
	return (SUCCESS);
}

int parse_input(int argc, char **argv, prog_data_t *program_data) {
	if (!are_valid_opts(argc, argv, program_data))
		return (FAILURE);
	if (!format_mac_addresses(argv, program_data))
		return (FAILURE);
	if (!format_ip_addresses(argv, program_data))
		return (FAILURE);
	return (SUCCESS);
}

int init_program(int argc, char **argv, prog_data_t *program_data) {

	bzero_data(program_data, sizeof(prog_data_t));

	if (!parse_input(argc, argv, program_data))
		return (FAILURE);
	return (SUCCESS);
}