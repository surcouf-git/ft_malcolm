#include <stdio.h>
#include <stdlib.h>

unsigned long ipv4_en_decimal(const char *ipv4) {
	unsigned int o1, o2, o3, o4;

	if (sscanf(ipv4, "%u.%u.%u.%u", &o1, &o2, &o3, &o4) != 4) {
		fprintf(stderr, "invalid address : %s\n", ipv4);
		exit(EXIT_FAILURE);
	}

	if (o1 > 255 || o2 > 255 || o3 > 255 || o4 > 255) {
		fprintf(stderr, "why > 255 ? \n");
		exit(EXIT_FAILURE);
	}

	return ((unsigned long)o1 << 24) |
			((unsigned long)o2 << 16) |
			((unsigned long)o3 << 8)  |
			((unsigned long)o4);
}

int main(int argc, char *argv[]) {
	if (argc < 2) {
		fprintf(stderr, "usage: %s ipv4 addr\n", argv[0]);
		return EXIT_FAILURE;
	}

	unsigned long decimal = ipv4_en_decimal(argv[1]);

	printf("decimal: %lu\n", decimal);

	return (EXIT_SUCCESS);
}
