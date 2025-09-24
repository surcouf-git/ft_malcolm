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

void decimal_to_ip(long ip) {
	uint8_t addr[4] = {};

	printf("DEC TO IP (%ld)\n", ip);
	int o = 4;
	for (int i = 0; i < 4; i++) {
		addr[i] = ip >> o & 0xFF;
		o += 4;
		printf("%d\n", addr[i]);
	}
	printf ("DONE\n");
}

int is_the_target(ethframe_t *frame, prog_data_t *program_data) {
	//const unsigned char *value = frame->
	decimal_to_ip(program_data->args.dec_src_mac);
}

int is_arp_found(ethframe_t *frame, prog_data_t *program_data) {
	if (is_the_target(frame, program_data))
	return(NOT_FOUND);
}

int uncap_eht_frame(ethframe_t *frame, prog_data_t *program_data) {
	const uint16_t type = ntohs(frame->type);

	if (type == ETH_P_ARP) {
		display_eth_frame(frame);

		if (is_arp_found(frame, program_data))
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