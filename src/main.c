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

int main() {

	struct sigaction action;
	action.sa_handler = &sig_handler;

	sigaction(SIGINT, &action, NULL);

	uint16_t protocol = htons(ETH_P_ALL);
	printf("Prototcol: %x(%x)\n", protocol, ntohs(ETH_P_ALL));
	//int sock = socket(PF_PACKET, SOCK_RAW, protocol);
	int sock = socket(PF_INET, SOCK_RAW, IPPROTO_UDP);
	if (sock == -1) {
		perror("socket()");
		return (EXIT_FAILURE);
	}
	printf("Socket:[%d]\n", sock);

	char *buffer = calloc(1001, 1);
	int n = 0;
	while ((n = recvfrom(sock, buffer, 1000, 0, NULL, NULL)) > 0) {
		buffer[n] = 0;
		struct ethhdr *eth = (struct ethhdr *)(buffer);
		printf("\t|Source adress: %.2X-%.2X-%.2X-%.2X-%.2X-%.2X\n", eth->h_source[0], eth->h_source[1], eth->h_source[2], eth->h_source[3], eth->h_source[4], eth->h_source[5]);
		printf("Raw packet (size:%d):%s\n", n, buffer);
		memset(buffer, 0, 1001);
		//break ;
	}
	printf("Process exited with: %d bytes reads\n", n);
	free(buffer);
	return (0);
}