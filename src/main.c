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

void sig_handler(int) {
	printf("\nbye\n");
	exit(0);
}

typedef struct arp_s {
	uint16_t hardware_type;
	uint16_t protocol_type;
	unsigned char vide[38];
} arp_t;

typedef struct frame_s {
	unsigned char destination[6];
	unsigned char source[6];
	uint16_t length_type;
	arp_t arp;
	int checksum;
} frame_t;

int main() {

	printf("%ld\n", sizeof(arp_t));
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
		frame_t *f = (frame_t *)buffer;
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
	return (0);
}