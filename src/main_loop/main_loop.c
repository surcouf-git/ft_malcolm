#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <strings.h>
#include <string.h>
#include <signal.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <netpacket/packet.h>
#include <net/ethernet.h>

#include "main_loop.h"
#include "global_utils.h"
#include "structs.h"
#include "def.h"
#include "display.h"

extern int has_signal;

//This one reply to everyone to anyone asking for the source IP (program 1st arg)
//int is_the_target(ethframe_t *source_frame, prog_data_t *program_data) {
//	if (source_frame->arp.target_pc_addr == program_data->args.dec_src_ip) {
//		if (program_data->options.verbose && !has_signal)
//			printf(TRGTFOUND);
//		return (FOUND);
//	}
//	return (NOT_FOUND);
//}

int is_the_target(ethframe_t *source_frame, prog_data_t *program_data) {
	if (source_frame->arp.sender_pc_addr == program_data->args.dec_trgt_ip \
			&& ft_umac_cmp(source_frame->arp.sender_hw_addr, program_data->args.trgt_mac_tab) \
				&& source_frame->arp.target_pc_addr == program_data->args.dec_src_ip) {

		if (program_data->options.verbose && !has_signal)
			printf(TRGTFOUND);
		return (FOUND);
	}
	return (NOT_FOUND);
}

void build_eth_hdr(ethframe_t *reply_frame, ethframe_t *source_frame, prog_data_t *program_data) {
	bzero_data(reply_frame, sizeof(ethframe_t));

	for (int i = 0; i < BYTE_MAC_LEN; i++) {
		reply_frame->destination[i] = source_frame->source[i];
		reply_frame->source[i] = program_data->args.src_mac_tab[i];
	}
	reply_frame->type = htons(ETH_P_ARP);
}

void build_arp_reply(arp_pckt_t *reply_arp_pckt, prog_data_t *program_data) {
	bzero_data(reply_arp_pckt, sizeof(arp_pckt_t));

	reply_arp_pckt->hardware_type = htons(ETH);
	reply_arp_pckt->protocol_type = htons(ETH_P_IP);
	reply_arp_pckt->hardware_len = BYTE_MAC_LEN;
	reply_arp_pckt->protocol_len = IPLEN;
	reply_arp_pckt->operation = htons(REPLY);

	ft_ustrcpy(reply_arp_pckt->sender_hw_addr, program_data->args.src_mac_tab);
	reply_arp_pckt->sender_pc_addr = program_data->args.dec_src_ip;

	ft_ustrcpy(reply_arp_pckt->target_hw_addr, program_data->args.trgt_mac_tab);
	reply_arp_pckt->target_pc_addr = program_data->args.dec_trgt_ip;
}

void build_reply(ethframe_t *source_frame, prog_data_t *program_data) {
	ethframe_t reply_frame;
	arp_pckt_t reply_arp_pckt;

	build_eth_hdr(&reply_frame, source_frame, program_data);
	build_arp_reply(&reply_arp_pckt, program_data);

	reply_frame.arp = reply_arp_pckt;
	program_data->reply_frame = reply_frame;
}

int send_reply(int sock, prog_data_t *program_data) {
	void *buf = &program_data->reply_frame;

	struct sockaddr_ll sockaddr;
	
	bzero_data(&sockaddr, sizeof(struct sockaddr_ll));
	sockaddr.sll_family = AF_PACKET;
	sockaddr.sll_ifindex = if_nametoindex(program_data->args.interface);
	sockaddr.sll_halen = ETH_ALEN;

	errno = 0;
	ssize_t sent = sendto(sock, buf, sizeof(ethframe_t), 0, (struct sockaddr *)&sockaddr, sizeof(sockaddr));
	if (sent < 0) {
		fprintf(stderr, "sendto(): %s\n", strerror(errno));
		return (FAILURE);
	}
	display_eth_frame(&program_data->reply_frame, program_data);
	printf(SSEND, program_data->args.trgt_ipv4);
	return (SUCCESS);
}

int validate_and_build(ethframe_t *source_frame, prog_data_t *program_data) {
	if (!is_the_target(source_frame, program_data))
		return(NOT_FOUND);
	/* else */
	build_reply(source_frame, program_data);
	return (FOUND);
}

int uncap_eht_frame(ethframe_t *source_frame, prog_data_t *program_data) {
	const uint16_t type = ntohs(source_frame->type);

	if (type == ETH_P_ARP) {
		if (program_data->options.verbose)
			display_eth_frame(source_frame, program_data);
		if (validate_and_build(source_frame, program_data))
			return (FOUND);
	}
	return (NOT_FOUND);
}

int is_packet_found(int sock, char *buffer, prog_data_t *program_data) {
	ethframe_t *source_frame;

	int bytes_read = recvfrom(sock, buffer, BUFFER_SIZE - 1, 0, NULL, NULL);
	if (bytes_read <= 0) {
		if (errno == EINTR && has_signal)
			return (NOT_FOUND);
		fprintf (stderr, ERECV, strerror(errno));
		return (1);
	}
	buffer[bytes_read] = 0;

	source_frame = (ethframe_t *)buffer;

	if (!uncap_eht_frame(source_frame, program_data))
		return (NOT_FOUND);
	else if (!send_reply(sock, program_data))
		return (ERROR);
	return (program_data->options.spam ? CONTINUE : FOUND);
}

int bind_socket_to_interface(int sock, prog_data_t *program_data) {

	if (setsockopt(sock, SOL_SOCKET, SO_BINDTODEVICE, 
			program_data->args.interface, ft_strlen(program_data->args.interface))) {
		
		fprintf(stderr, "setsocketopt(): %s", strerror(errno));

		if (errno == 19)
			fprintf (stderr, ": '%s'", program_data->args.interface);

		fprintf(stderr, "\n");
		return (FAILURE);
	}
	return (SUCCESS);
}

int main_loop(prog_data_t *program_data) {
	char buffer[BUFFER_SIZE];

	int sock = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
	if (sock == -1) {
		fprintf(stderr, ESOCK, strerror(errno));
		return (FAILURE);
	}

	// if (!bind_socket_to_interface(sock, program_data))
	// 	return (FAILURE);

	printf(WAITING);
	while (!is_packet_found(sock, buffer, program_data)) {
		if (has_signal)
			break ;
		continue ;
	}

	close(sock);
	return (SUCCESS);
}