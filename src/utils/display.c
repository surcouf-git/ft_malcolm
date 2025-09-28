/*Sorry for your eyes*/

#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "display.h"
#include "global_utils.h"
#include "structs.h"

#define MAX_SIZE 60
#define MAX_BUF_SIZE 4096

void fill_empty_space(size_t size) {
	for (size_t i = 0; i < size; i++) {
		printf(" ");
	}
	printf("+\n");
}

void build_first_line(uint16_t type) {
	static char request[] = "+----------------------ARP REQUEST", reply[] = "+----------------------ARP REPLY";
	char *ptr = NULL;
	size_t line_size = 0, to_fill = 0;
		
	if (type == REQUEST)
		ptr = request;
	else
		ptr = reply;
		
	line_size = ft_strlen(ptr);
	to_fill = MAX_SIZE - line_size;
	printf("%s", ptr);
		
	for (size_t i = 0; i < to_fill - 1; i++) {
		printf("-");
	}
	printf("+\n+");

	for (size_t i = 0; i < MAX_SIZE - 2; i++) {
		printf(" ");
	}
	printf("+\n");
}

void build_body(ethframe_t *frame, prog_data_t *program_data, uint16_t type) {
	uint8_t *target = NULL, *destination = NULL, *ptr = NULL;
	char buffer[1024] = {};

	
	if (type == REQUEST) {

		char targ[IP_MAX_LEN], dest[IP_MAX_LEN];
		inet_ntop(AF_INET, &frame->arp.target_pc_addr, targ, IP_MAX_LEN);
		inet_ntop(AF_INET, &frame->arp.sender_pc_addr, dest, IP_MAX_LEN);

		sprintf (buffer, "+ Who has %s ? Tell %s ", targ, dest);
	} else {
		target = program_data->args.src_ipv4;
		destination = program_data->args.src_mac_tab;
		sprintf (buffer, "+ %s is at %2X:%2X:%2X:%2X:%2X:%2X ", target,
					destination[0], destination[1], destination[2], destination[3], destination[4], destination[5]);
	}

	printf("%s", buffer);
	fill_empty_space((MAX_SIZE - 1) - ft_strlen(buffer));

	printf("+");
	fill_empty_space(MAX_SIZE - 2);

	bzero_data(buffer, 1024);
	ptr = frame->arp.sender_hw_addr;
	sprintf(buffer, "+ Sender MAC addess: %2X:%2X:%2X:%2X:%2X:%2X ", ptr[0], ptr[1], ptr[2], ptr[3], ptr[4], ptr[5]);
	printf("%s", buffer);

	fill_empty_space((MAX_SIZE - 1) - ft_strlen(buffer));

	bzero_data(buffer, 1024);
	struct in_addr addr;
	bzero_data(&addr, sizeof(struct in_addr));
	addr.s_addr = frame->arp.sender_pc_addr;
	sprintf(buffer, "+ Sender IP address: %s ", inet_ntoa(addr));
	printf("%s", buffer);

	fill_empty_space((MAX_SIZE - 1) - ft_strlen(buffer));

	bzero_data(buffer, 1024);
	ptr = frame->arp.target_hw_addr;
	sprintf(buffer, "+ Target MAC addess: %2X:%2X:%2X:%2X:%2X:%2X ", ptr[0], ptr[1], ptr[2], ptr[3], ptr[4], ptr[5]);
	printf("%s", buffer);

	fill_empty_space((MAX_SIZE - 1) - ft_strlen(buffer));

	bzero_data(buffer, 1024);
	bzero_data(&addr, sizeof(struct in_addr));
	addr.s_addr = frame->arp.target_pc_addr;
	sprintf(buffer, "+ Target IP address: %s ", inet_ntoa(addr));
	printf("%s", buffer);

	fill_empty_space((MAX_SIZE - 1) - ft_strlen(buffer));

	printf("+");
	fill_empty_space(MAX_SIZE - 2);
}

void build_last_line() {
	printf("+");
	for (size_t i = 0; i < MAX_SIZE - 2; i++) {
		printf("-");
	}
	printf("+\n\n");
}

void build_eth_header(ethframe_t *frame) {
	static char name[] = "+--------------------ETHERNET HEADER";
	char buffer[1024] = {};

	printf("%s", name);
	size_t to_fill = MAX_SIZE - ft_strlen(name);
	for (size_t i = 0; i < to_fill - 1; i++) {
		printf("-");
	}
	printf("+\n+");

	for (size_t i = 0; i < MAX_SIZE - 2; i++) {
		printf(" ");
	}
	printf("+\n");

	bzero_data(buffer, 1024);
	uint8_t *ptr = frame->destination;
	sprintf(buffer, "+ Destination: %2X:%2X:%2X:%2X:%2X:%2X ", ptr[0], ptr[1], ptr[2], ptr[3], ptr[4], ptr[5]);
	printf("%s", buffer);
	fill_empty_space((MAX_SIZE - 1) - ft_strlen(buffer));

	bzero_data(buffer, 1024);
	ptr = frame->source;
	sprintf(buffer, "+ Source: %2X:%2X:%2X:%2X:%2X:%2X ", ptr[0], ptr[1], ptr[2], ptr[3], ptr[4], ptr[5]);
	printf("%s", buffer);
	fill_empty_space((MAX_SIZE - 1) - ft_strlen(buffer));

	printf("+");
	for (size_t i = 0; i < MAX_SIZE - 2; i++) {
		printf(" ");
	}
	printf("+\n");
}

void display_eth_frame(ethframe_t *frame, prog_data_t *program_data) {
	uint16_t type = ntohs(frame->arp.operation);

	build_eth_header(frame);
	build_first_line(type);
	build_body(frame, program_data, type);
	build_last_line();
}