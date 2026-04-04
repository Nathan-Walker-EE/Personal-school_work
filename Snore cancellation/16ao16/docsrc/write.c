#include <stdio.h>

#include "16ao16_dsl.h"

int ao16_write_dsl(int fd, const void* src, size_t bytes)
{
	int	ret;

	ret	= ao16_write(fd, src, bytes);

	if (ret < 0)
		printf("ERROR: ao16_write() returned %d\n", ret);

	return(ret);
}