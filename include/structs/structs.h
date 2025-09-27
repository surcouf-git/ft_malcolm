#pragma once
#pragma pack(1)

#include <stdint.h>

#include "def.h"

typedef struct arp_s {
	uint16_t hardware_type;
	uint16_t protocol_type;
	uint8_t hardware_len;
	uint8_t protocol_len;
	uint16_t operation;
	uint8_t sender_hw_addr[BYTE_MAC_LEN];
	uint32_t sender_pc_addr;
	uint8_t target_hw_addr[BYTE_MAC_LEN];
	uint32_t target_pc_addr;
} arp_pckt_t;

typedef struct ethframe_s {
	uint8_t destination[6];
	uint8_t source[6];
	uint16_t type;
	arp_pckt_t arp;
} ethframe_t;

typedef struct prog_opts_s {
	unsigned int spam;
	unsigned int verbose;
} prog_opts_t;

typedef struct prog_args_s {
	uint8_t src_ipv4[IP_MAX_LEN];
	uint8_t src_ipv6[IP_MAX_LEN];
	uint32_t dec_src_ip;
	uint8_t src_mac_tab[BYTE_MAC_LEN];
	uint64_t dec_src_mac;
	uint8_t trgt_ipv4[IP_MAX_LEN];
	uint8_t trgt_ipv6[IP_MAX_LEN];
	uint32_t dec_trgt_ip;
	uint8_t trgt_mac_tab[BYTE_MAC_LEN];
	uint64_t dec_trgt_mac;
} prog_args_t;

typedef struct prog_data_s {
	prog_args_t args;
	prog_opts_t options;

	ethframe_t ether_frame;
	ethframe_t reply_frame;
} prog_data_t;

#pragma pack(0)