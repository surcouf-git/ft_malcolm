#include "global_utils.h"

int ft_strlen(char *s) {
	if (!s) return (0);

	int len = 0;
	while (s[len])
		len++;
	return (len);
}