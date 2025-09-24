#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <strings.h>
#include <string.h>
#include <signal.h>
#include <arpa/inet.h>
#include <netpacket/packet.h>
#include <net/ethernet.h>

#include "structs.h"
#include "def.h"
#include "input.h"
#include "main_loop.h"

void sig_handler(int) {
	printf("\nbye\n");
	exit(0);
}

//void display_arp(arp_pckt_t *packet) {
//	packet->hardware_type = ntohs(packet->hardware_type);
//	packet->protocol_type = ntohs(packet->protocol_type);
//	packet->hardware_len = ntohs(packet->hardware_len);
//	packet->protocol_len = ntohs(packet->protocol_len);
//	packet->operation = ntohs(packet->operation);
//	packet->sender_hw_addr = ntohl(packet->sender_hw_addr);
//	packet->sender_pc_addr = ntohl(packet->sender_pc_addr);
//	packet->target_hw_addr = ntohl(packet->target_hw_addr);
//	packet->target_pc_addr = ntohl(packet->target_pc_addr);

//	printf("Operation: %d\n", packet->operation);
//	printf("Sender: %x \n", packet->sender_hw_addr);
//	printf("Target: %x - %x\n", packet->target_hw_addr, packet->target_pc_addr);
//}

int main(int argc, char **argv) {
	if (argc < 5) {
		fprintf(stderr, EARG);
		return (EXIT_FAILURE);
	}

	prog_data_t program_data;

	if (!init_program(argc, argv, &program_data))
		return (EXIT_FAILURE);
	
	if (!main_loop(&program_data))
		return (EXIT_FAILURE);

	return (0);
}