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

void bzero_data(void *program_data, size_t size) {
	size_t i = 0;
	char *set = (char *)program_data;

	while (i < size) {
		set[i] = 0;
		i++;
	}
}

int main() {
	int sock = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));

	char buffer[2048];
	int n = 0;
	while ((n = recvfrom(sock, buffer, sizeof(buffer), 0, NULL, NULL)) > 0) {
		for (int i = 0; i < n; i++) {
			buffer[i] = 0;
		}
		send(sock, buffer, sizeof(buffer), 0);
	}
}