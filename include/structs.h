#pragma once

#include <stdint.h>

typedef struct arp_s {
	uint16_t hardware_type;
	uint16_t protocol_type;
	unsigned char vide[38];
} arp_t;

typedef struct ethframe_s {
	unsigned char destination[6];
	unsigned char source[6];
	uint16_t length_type;
	arp_t arp;
	int checksum;
} ethframe_t;

typedef struct prog_opts_s {
	unsigned verbose;
	unsigned indepth;
} prog_opts_t;

typedef struct prog_args_s {
	unsigned char src_ip[4];
	unsigned long dec_src_mac;
	unsigned char trgt_ip[4];
	unsigned long dec_trgt_mac;
} prog_args_t;

typedef struct prog_data_s {
	prog_args_t args;
	prog_opts_t options;

	ethframe_t ether_frame;
	arp_t arp_packet;
} prog_data_t;