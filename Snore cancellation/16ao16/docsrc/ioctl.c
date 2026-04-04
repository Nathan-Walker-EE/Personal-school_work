#include <stdio.h>

#include "16ao16_dsl.h"

int ao16_ioctl_dsl(int fd, int request, void *arg)
{
	int	errs;
	int	ret;

	ret	= ao16_ioctl(fd, request, arg);

	if (ret)
		printf("ERROR: ao16_ioctl() returned %d\n", ret);

	errs	= ret ? 1 : 0;
	return(errs);
}