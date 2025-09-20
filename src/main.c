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

void sig_handler(int) {
	printf("\nbye\n");
	exit(0);
}

int test_main() {
	struct sigaction action;
	action.sa_handler = &sig_handler;

	sigaction(SIGINT, &action, NULL);

	uint16_t protocol = htons(ETH_P_ALL);
	printf("Prototcol: %x(%x)\n", protocol, ntohs(ETH_P_ALL));
	int sock = socket(AF_PACKET, SOCK_RAW, protocol);
	if (sock == -1) {
		perror("socket()");
		return (EXIT_FAILURE);
	}
	printf("Socket:[%d]\n", sock);

	char *buffer = calloc(10000, 1);
	int n = 0;
	while ((n = recvfrom(sock, buffer, 10000, 0, NULL, NULL)) > 0) {
		buffer[n] = 0;
		ethframe_t *f = (ethframe_t *)buffer;
		for (int i = 0; i < 6; i++) {
			printf("%X ", f->destination[i]);
		}
		f->length_type = ntohs(f->length_type);
		if (f->length_type == ETH_P_ARP) {
			arp_t *arp = &f->arp;
			printf("HW Type: %d | %d", arp->hardware_type, arp->protocol_type);
		}
		printf("\n");
		memset(buffer, 0, 10000);
	}
	printf("Process exited with: %d bytes reads\n", n);
	free(buffer);
}

int main(int argc, char **argv) {
	//if (argc < 5) {
	//	fprintf(stderr, EARG);
	//	return (EXIT_FAILURE);
	//}

	prog_data_t program_data;

	if (!init_program(argc, argv, &program_data))
		return (EXIT_FAILURE);

	test_main();
	return (0);
}