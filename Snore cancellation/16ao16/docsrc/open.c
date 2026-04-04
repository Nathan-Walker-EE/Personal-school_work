#include <stdio.h>

#include "16ao16_dsl.h"

int ao16_open_dsl(int index, int share, int* fd)
{
	int	errs;
	int	ret;

	ret	= ao16_open(index, share, fd);

	if (ret)
		printf("ERROR: ao16_open() returned %d\n", ret);

	errs	= ret ? 1 : 0;
	return(errs);
}