#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <strings.h>
#include <string.h>
#include <signal.h>
#include <arpa/inet.h>
#include <netpacket/packet.h>
#include <net/ethernet.h>

#include "main_loop.h"
#include "global_utils.h"
#include "structs.h"
#include "def.h"

void puthex(unsigned char *data, int n) {
	int i = 0;
	while (i < n) {
		printf("%X ", data[i]);
		i++;
	}
}

void display_eth_frame(ethframe_t *frame) {
	printf("[====================]\n");
	printf("[ETH FRAME]\n");
	printf("DESTINATION: ");
	puthex((unsigned char *)&frame->destination, 6);
	printf("\n");
	printf("SOURCE: ");
	puthex((unsigned char *)&frame->source, 6);
	printf("\n[END ETH FRAME]\n");
	printf("[ARP PACKET]\n");
	printf("Hardware type: %X\n", ntohs(frame->arp.hardware_type));
	printf("Protocol type: %X\n", ntohs(frame->arp.protocol_type));
	printf("Operation: %X\n", ntohs(frame->arp.operation));
	printf("Sender HW addr: \n");
	puthex(frame->arp.sender_hw_addr, 6);
	//printf("Sender protocol addr: %X\n", )
	printf("\n");
	printf("[END ARP PACKET]\n");
}

//uint32_t uint8_tab_to_uint32(uint8_t tab[8]) {
//	uint8_t addr[4] = {};

//	printf("DEC TO IP (%ld)\n", ip);
//	int o = 0;
//	for (int i = 0; i < 4; i++) {
//		addr[i] = ip >> o & 0xFF;
//		o += 8;
//		printf("%d\n", addr[i]);
//	}
//	printf ("DONE\n");
//}

int is_the_target(ethframe_t *frame, prog_data_t *program_data) {
	if (frame->arp.sender_pc_addr == program_data->args.dec_trgt_ip) {
		if (program_data->options.verbose)
			printf(ARPFOUND, frame->arp.sender_pc_addr, program_data->args.dec_trgt_ip);
		return (FOUND);
	}
	return (NOT_FOUND);
}

void build_eth_hdr(ethframe_t *reply_frame, ethframe_t *source_frame, prog_data_t *program_data) {
	bzero_data(reply_frame, sizeof(ethframe_t));

	int o = 0;
	uint64_t src_mac = program_data->args.dec_src_mac;
	for (int i = 0; i < 6; i++) {
		reply_frame->destination[i] = source_frame->source[i];
		reply_frame->source[i] = (src_mac >> o) & 0xFF;
		printf("TKT: -> [%d]\n", reply_frame->destination[i]);
		printf("SRC_REPLY -> [%lX]\n", src_mac);
		o += 8;
	}
}

void build_reply(ethframe_t *frame, prog_data_t *program_data) {
	ethframe_t reply_frame;

	build_eth_hdr(&reply_frame, frame, program_data);
	//build_arp_reply(&reply_frame, frame, program_data);
}

int validate_and_reply(ethframe_t *frame, prog_data_t *program_data) {
	if (!is_the_target(frame, program_data))
		return(NOT_FOUND);
	/* else */
	build_reply(frame, program_data);
	//send_reply(frame, program_data);
	return (SUCCESS);
}

int uncap_eht_frame(ethframe_t *frame, prog_data_t *program_data) {
	const uint16_t type = ntohs(frame->type);

	if (type == ETH_P_ARP) {
		display_eth_frame(frame);
		if (validate_and_reply(frame, program_data))
			return (FOUND);
	}
	return (NOT_FOUND);
}

int is_packet_found(int sock, char *buffer, prog_data_t *program_data) {
	ethframe_t *frame;

	int bytes_read = recvfrom(sock, buffer, BUFFER_SIZE - 1, 0, NULL, NULL);
	if (bytes_read <= 0) {
		fprintf (stderr, ERECV, strerror(errno));
		return (1);
	}
	buffer[bytes_read] = 0;

	frame = (ethframe_t *)buffer;

	if (!uncap_eht_frame(frame, program_data))
		return (NOT_FOUND);
	return (FOUND);
}

int main_loop(prog_data_t *program_data) {
	char buffer[BUFFER_SIZE];

	int sock = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
	if (sock == -1) {
		fprintf(stderr, ESOCK, strerror(errno));
		return (FAILURE);
	}

	while (!is_packet_found(sock, buffer, program_data))
		continue ;

	return (SUCCESS);
}