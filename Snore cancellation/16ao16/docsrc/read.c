#include <stdio.h>

#include "16ao16_dsl.h"

int ao16_read_dsl(int fd, void* dst, size_t bytes)
{
	int	ret;

	ret	= ao16_read(fd, dst, bytes);

	if (ret < 0)
		printf("ERROR: ao16_read() returned %d\n", ret);

	return(ret);
}