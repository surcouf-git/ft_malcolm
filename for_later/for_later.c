
//Extract ipv4 AND ipv6 - just need to change the const IP_MAX_LEN to 46
static int convert_possible_hostname(char *src_ip, char *trgt_ip, prog_args_t *args) {
	struct addrinfo hints, *src_resp = 0, *trgt_resp = 0, *ptr = 0;
	char buffer[IP_MAX_LEN];
	int src_done = 0, ipv4_done = 0, ipv6_done = 0;

	bzero_data(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;

	if (getaddrinfo(src_ip, 0, &hints, &src_resp)) {
		fprintf(stderr, EBADIP, src_ip);
		return (FAILURE);
	}
	if (getaddrinfo(trgt_ip, 0, &hints, &trgt_resp)) {
		fprintf(stderr, EBADIP, trgt_ip);
		freeaddrinfo(src_resp);
		return (FAILURE);
	}

	ptr = src_resp;
	while (ptr) {

		if (ptr->ai_family == AF_INET && !ipv4_done) {

			struct sockaddr_in *ipv4 = (struct sockaddr_in *)ptr->ai_addr;
			if (!inet_ntop(AF_INET, &(ipv4->sin_addr), buffer, sizeof(buffer))) {
				fprintf (stderr, ENTOP);
				return (FAILURE);
			}
			src_done ? ft_strcpy((char *)args->trgt_ipv4, buffer) : ft_strcpy((char *)args->src_ipv4, buffer);
			bzero_data(buffer, IP_MAX_LEN);
			ipv4_done = 1;

		} else if (ptr->ai_family == AF_INET6 && !ipv6_done) {

			struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)ptr->ai_addr;
			if (!inet_ntop(AF_INET6, &(ipv6->sin6_addr), buffer, sizeof(buffer))) {
				fprintf (stderr, ENTOP);
				return (FAILURE);
			}
			src_done ? ft_strcpy((char *)args->trgt_ipv6, buffer) : ft_strcpy((char *)args->src_ipv6, buffer);
			bzero_data(buffer, IP_MAX_LEN);
			ipv6_done = 1;

		}
		ptr = ptr->ai_next;
		if (!ptr && !src_done) {
			ptr = trgt_resp;
			src_done = 1;
			ipv4_done = 0;
			ipv6_done = 0;
		}
	}
	freeaddrinfo(src_resp);
	freeaddrinfo(trgt_resp);
	return (SUCCESS);
}