#include <stdio.h>

#include "16ao16_dsl.h"

int ao16_close_dsl(int fd)
{
	int	errs;
	int	ret;

	ret	= ao16_close(fd);

	if (ret)
		printf("ERROR: ao16_close() returned %d\n", ret);

	errs	= ret ? 1 : 0;
	return(errs);
}