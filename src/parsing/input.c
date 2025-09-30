#include <stdlib.h>
#include <stdio.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <ifaddrs.h>
#include <net/if.h>

#include "global_utils.h"
#include "structs.h"
#include "def.h"

extern int has_signal;

static int is_delimiter_valid(const char *mac, size_t pos) {
	if (mac[pos] != '-' && mac[pos] != ':') {
		fprintf(stderr, EBADMAC, mac);
		return (FAILURE);
	}
	return(SUCCESS);
}

static int is_char_valid(const char *mac, size_t pos) {
	char c = mac[pos];

	if (is_digit(c))
		return (SUCCESS);
	else if ((c < 'a' || c > 'f') && (c < 'A' || c > 'F')) {
		fprintf(stderr, EBADMAC, mac);
		return (FAILURE);
	}
	return (SUCCESS);
}

static int is_mac_valid(const char *src_mac, const char *trgt_mac) {
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
	if (ft_strlen(trgt_mac) != 17) {
		fprintf(stderr, EBADMAC, trgt_mac);
		return(FAILURE);
	} else if (ft_strlen(src_mac) != 17) {
		fprintf(stderr, EBADMAC, src_mac);
		return(FAILURE);
	}
	return(SUCCESS);
}

static int is_valid_opt(char *s) {
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

static int add_opt(char *c, prog_data_t *program_data) {
	if (c[1] == 'v')
		program_data->options.verbose = 1;
	if (c[1] == 's')
		program_data->options.spam = 1;
	if (c[1] == 'i') {
		printf("-i detected, wich interface do you want to use ?\n->");
		if (scanf("%15s", program_data->args.interface) <= 0) {
			if (!has_signal) fprintf (stderr, EBADINPT);
			return (FAILURE);
		}
		printf("\n");
	}
	return (SUCCESS);
}

static int are_valid_opts(int argc, char **argv, prog_data_t *program_data) {
	if (argc == 5)
		return (SUCCESS);
	
	int i = 5;
	while (i < argc) {
		if (is_valid_opt(argv[i]))
			if (!add_opt(argv[i], program_data)) return (FAILURE);
		i++;
	}
	return (SUCCESS);
}

static void fill_prog_data(const char *raw_src_mac, const char *raw_trgt_mac, prog_data_t *program_data) {
	uint64_t *dec_src_mac = &(program_data->args.dec_src_mac), 
				*dec_trgt_mac = &(program_data->args.dec_trgt_mac);

	uint8_t *src_mac_tab = program_data->args.src_mac_tab,
				*trgt_mac_tab = program_data->args.trgt_mac_tab;

	*dec_src_mac = ascii_to_hex(raw_src_mac, src_mac_tab);
	*dec_trgt_mac = ascii_to_hex(raw_trgt_mac, trgt_mac_tab);
}

static int format_mac_addresses(char **argv, prog_data_t *program_data) {
	const char *raw_src_mac = argv[2], 
				*raw_trgt_mac = argv[4];

	if (program_data->options.verbose && !has_signal) 
		printf(STARTMACFORM);

	if (!is_mac_valid(raw_src_mac, raw_trgt_mac))
		return (FAILURE);

	fill_prog_data(raw_src_mac, raw_trgt_mac, program_data);

	if (program_data->options.verbose && !has_signal) {
		printf(MACFORM, 
				program_data->args.dec_src_mac, program_data->args.dec_src_mac,
				program_data->args.dec_trgt_mac, program_data->args.dec_trgt_mac);
	}
	return (SUCCESS);
}

static int get_addr_infos(char *src_ip, char *trgt_ip,
							struct addrinfo *hints, 
								struct addrinfo **src_info, 
									struct addrinfo **trgt_info) {

	bzero_data(hints, sizeof(struct addrinfo));
	hints->ai_family = AF_INET;

	if (getaddrinfo(src_ip, 0, hints, src_info)) {
		fprintf(stderr, EBADIP, src_ip);
		return (FAILURE);
	}
	
	if (getaddrinfo(trgt_ip, 0, hints, trgt_info)) {
		fprintf(stderr, EBADIP, trgt_ip);
		freeaddrinfo(*src_info);
		return (FAILURE);
	}
	return (SUCCESS);
}

static int convert_possible_hostname(char *src_ip, char *trgt_ip, prog_args_t *args) {
	struct addrinfo hints, *src_info = NULL, *trgt_info = NULL, *ptr = NULL;
	char buffer[IP_MAX_LEN];
	
	if (!get_addr_infos(src_ip, trgt_ip, &hints, &src_info, &trgt_info))
		return (FAILURE);
	
	int src_done = 0;
	ptr = src_info;
	while (ptr) {

		if (ptr->ai_family == AF_INET) {
			
			struct sockaddr_in *ipv4 = (struct sockaddr_in *)ptr->ai_addr;
			if (!inet_ntop(AF_INET, &(ipv4->sin_addr), buffer, sizeof(buffer))) {
				fprintf (stderr, ENTOP);
				return (FAILURE);
			}
			src_done ? ft_strcpy((char *)args->trgt_ipv4, buffer) : ft_strcpy((char *)args->src_ipv4, buffer);
			bzero_data(buffer, IP_MAX_LEN);

		}
		if (!src_done) {
			ptr = trgt_info;
			src_done = 1;
			continue ;
		}
		break ;
	}

	freeaddrinfo(src_info);
	freeaddrinfo(trgt_info);

	return (SUCCESS);
}

static int are_private_ips(const char *src_ip, const char *trgt_ip) {
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
		printf((ft_strcmp(src_ip, LOCALHOST) ? WHYLOCAL : WHY), src_ip);
		return (FAILURE);
	}
	if (!trgt_is_valid) {
		printf((ft_strcmp(trgt_ip, LOCALHOST) ? WHYLOCAL : WHY), trgt_ip);
		return (FAILURE);
	}
	return (SUCCESS);
}

static int is_valid_address(char **argv, prog_data_t *program_data) {
	const char *src_ip = (const char *)program_data->args.src_ipv4;
	const char *trgt_ip = (const char *)program_data->args.trgt_ipv4;

	if (program_data->options.verbose && !has_signal) {

		if (!ft_strcmp(src_ip, argv[1]))
			printf(RESOLVIPSRC, argv[1], src_ip);

		if (!ft_strcmp(trgt_ip, argv[3]))
			printf(RESOLVIPTRGT, argv[3], trgt_ip);
	}
	if (!are_private_ips(src_ip, trgt_ip)) {
		return (FAILURE);
	}
	return(SUCCESS);
}

static int format_ip_addresses(char **argv, prog_data_t *program_data) {
	char *raw_src_ip = argv[1];
	char *raw_trgt_ip = argv[3];

	if (program_data->options.verbose && !has_signal)
		printf(STARTIPFORM);

	if (!convert_possible_hostname(raw_src_ip, raw_trgt_ip, &(program_data->args)))
		return (FAILURE);

	if (!is_valid_address(argv, program_data))
		return (FAILURE);
	
	program_data->args.dec_src_ip = inet_addr((char *)program_data->args.src_ipv4);
	program_data->args.dec_trgt_ip = inet_addr((char *)program_data->args.trgt_ipv4);

	if (program_data->options.verbose && !has_signal) {
		printf(VALID_IPS);
		printf(IPFORM, 
			program_data->args.src_ipv4, program_data->args.dec_src_ip, 
			program_data->args.trgt_ipv4, program_data->args.dec_trgt_ip);
	}
	return (SUCCESS);
}

static int parse_input(int argc, char **argv, prog_data_t *program_data) {
	if (!are_valid_opts(argc, argv, program_data))
		return (FAILURE);
	if (!format_mac_addresses(argv, program_data))
		return (FAILURE);
	printf(SMACADDR);
	if (!format_ip_addresses(argv, program_data))
		return (FAILURE);
	printf(SIPADDR);
	return (SUCCESS);
}

int get_interface_priority(struct ifaddrs *ifa) {
	if (ft_strncmp(ifa->ifa_name, "eth", 3) || ft_strncmp(ifa->ifa_name, "enp", 3)) return 3;
	if (ft_strncmp(ifa->ifa_name, "wlan", 4) || ft_strncmp(ifa->ifa_name, "wlp", 3)) return 2;

	return 1;
}

int is_basic_valid(struct ifaddrs *ifa) {	
	if (!ifa || !ifa->ifa_name) return (FAILURE);
		
	
	if (!(ifa->ifa_flags & IFF_UP)) return (FAILURE);		
	if (!(ifa->ifa_flags & IFF_RUNNING)) return (FAILURE);   
	if (ifa->ifa_flags & IFF_LOOPBACK) return (FAILURE);	 

	if (!ifa->ifa_addr) return (FAILURE);
	if (ifa->ifa_addr->sa_family != AF_INET) return (FAILURE);

	struct sockaddr_in *addr_in = (struct sockaddr_in*)ifa->ifa_addr;
	if (addr_in->sin_addr.s_addr == 0) return (FAILURE);
	
	if (ft_strncmp(ifa->ifa_name, "docker", 6) == 1) return (FAILURE);
	if (ft_strncmp(ifa->ifa_name, "veth", 4) == 1) return (FAILURE);
	if (ft_strncmp(ifa->ifa_name, "br-", 3) == 1) return (FAILURE);
	if (ft_strncmp(ifa->ifa_name, "virbr", 5) == 1) return (FAILURE);
	if (ft_strcmp(ifa->ifa_name, "lo") == 1) return (FAILURE);
		
	return (SUCCESS); 
}

int find_valid_interface(prog_data_t *program_data) {
	struct ifaddrs *ifaddr, *ifa;
	int priority = 0;

	if (ft_strlen(program_data->args.interface))
		return (SUCCESS);
	if (getifaddrs(&ifaddr) == -1) {
		fprintf(stderr, EIFADDR);
		return (FAILURE);
	}
		
	for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
		if (!is_basic_valid(ifa)) continue;
		
		int current_priority = get_interface_priority(ifa);
		if (current_priority > priority) {
			priority = current_priority;
			ft_strcpy(program_data->args.interface, ifa->ifa_name);
		}
	}
	freeifaddrs(ifaddr);
	if (!ft_strlen(program_data->args.interface)) {
		fprintf(stderr, EINOTFND);
		return (FAILURE);
	} else {
		printf(SINFND, program_data->args.interface);
	}
	return (SUCCESS);
}

int init_program(int argc, char **argv, prog_data_t *program_data) {

	bzero_data(program_data, sizeof(prog_data_t));
	bzero_data(&program_data->reply_frame, sizeof(arp_pckt_t));

	if (!parse_input(argc, argv, program_data))
		return (FAILURE);
	if (!find_valid_interface(program_data))
		return (FAILURE);
	return (SUCCESS);
}