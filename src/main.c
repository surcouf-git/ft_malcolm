#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <strings.h>
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

	int protocol = htons(ETHERTYPE_IP);
	int sock = socket(PF_PACKET, SOCK_RAW, protocol);
	if (sock == -1) {
		perror("socket()");
		return (EXIT_FAILURE);
	}
	printf("Socket:[%d]\n", sock);

	
	char *buffer = calloc(1001, 1);
	int n = 0;
	while ((n = recvfrom(sock, buffer, 1000, 0, NULL, NULL)) > 0) {
		buffer[n] = 0;
		printf("Raw packet:%s\n", buffer);
		for (int i = 0; buffer[i]; i++)
			buffer[i] = 0;
	}
	printf("Process exited with: %d bytes reads\n", n);
	return (0);
}