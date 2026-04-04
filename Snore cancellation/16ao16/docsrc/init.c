#include <stdio.h>

#include "16ao16_dsl.h"

int ao16_init_dsl(void)
{
	int	errs;
	int	ret;

	ret	= ao16_init();

	if (ret)
		printf("ERROR: ao16_init() returned %d\n", ret);

	errs	= ret ? 1 : 0;
	return(errs);
}