// $URL: http://subversion:8080/svn/gsc/trunk/drivers/gsc_common/utils/gsc_util_args.c $
// $Rev: 44781 $
// $Date: 2019-05-02 14:55:16 -0500 (Thu, 02 May 2019) $

// OS & Device Independent: Utility: source file

#include "main.h"



//*****************************************************************************
static int _scan_long(const char* src, long* dst)
{
	char	c;
	int		ret;

	if (src == NULL)
	{
		ret	= 0;
	}
	else if ((src[0] == '0') && (src[1] == 'x') && (src[2]))
	{
		ret	= sscanf(src + 2, "%lX%c", dst, &c);
	}
	else
	{
		ret	= sscanf(src, "%ld%c", dst, &c);
	}

	return(ret);
}



//*****************************************************************************
static int _arg_show_list(const gsc_arg_set_t* set)
{
	#define	MARGIN	2

	char	buf[32];		// No argument should ever be near this length.
	int		errs	= 0;
	int		i;
	int		len;
	int		width	= 1;

	printf("%s - %s\n", set->name, set->desc);
	printf("USAGE: %s", set->name);

	// Show the command line summary.

	for (i = 0; i < set->qty; i++)
	{
		if (set->list[i].arg == NULL)
			continue;

		len	= strlen(set->list[i].arg);

		if ((len + MARGIN + 1) >= sizeof(buf))
			continue;

		switch (set->list[i].type)
		{
			default:

				errs++;
				printf(" <FAIL <---: i %d, type %d>", i, (int) set->list[i].type);
				continue;

			case GSC_ARG_DEV_NOTE:

				continue;

			case GSC_ARG_DEV_INDEX:
			case GSC_ARG_S32_FLAG:

				if (set->list[i].desc == NULL)
					continue;

				sprintf(buf, "%s", set->list[i].arg);
				break;

			case GSC_ARG_S32_MASK:
			case GSC_ARG_S32_MIN:
			case GSC_ARG_S32_RANGE:

				if (set->list[i].desc == NULL)
					continue;

				sprintf(buf, "%s#", set->list[i].arg);
				break;

			case GSC_ARG_STR_PTR:

				if (set->list[i].desc == NULL)
					continue;

				sprintf(buf, "%s=str", set->list[i].arg);
				break;
		}

		printf(" <%s>", buf);
		len	= strlen(buf);

		if (width < len)
			width	= len;
	}

	printf("\n");

	// Show the command line argument descriptions.

	for (i = 0; i < set->qty; i++)
	{
		if (set->list[i].desc == NULL)
			continue;

		if ((set->list[i].arg == NULL) && (set->list[i].type == GSC_ARG_DEV_NOTE))
		{
			printf("  Note: %s\n", set->list[i].desc);
			continue;
		}

		len	= strlen(set->list[i].arg);

		if ((len + 2) >= sizeof(buf))
		{
			errs++;
			printf("  %-*s  %s\n", width, set->list[i].arg, set->list[i].desc);
			printf(	"  %-*s  FAIL <---  (too long, limit is %d bytes))\n",
					width,
					set->list[i].arg,
					(int) sizeof(buf) - MARGIN);
			continue;
		}

		switch (set->list[i].type)
		{
			default:

				errs++;
				printf(	"  %*s  %s\n",
						width,
						set->list[i].arg,
						set->list[i].desc);
				printf(	"  %-*s  FAIL <---  (invalid type: i %d, type %d>\n",
						width,
						set->list[i].arg,
						i,
						(int) set->list[i].type);
				continue;

			case GSC_ARG_DEV_NOTE:

				continue;

			case GSC_ARG_DEV_INDEX:
			case GSC_ARG_S32_FLAG:

				sprintf(buf, "%s", set->list[i].arg);
				break;

			case GSC_ARG_S32_MASK:
			case GSC_ARG_S32_MIN:
			case GSC_ARG_S32_RANGE:

				sprintf(buf, "%s#", set->list[i].arg);
				break;

			case GSC_ARG_STR_PTR:

				sprintf(buf, "%s=str", set->list[i].arg);
				break;
		}

		printf("  %-*s  %s\n", width, buf, set->list[i].desc);
	}

	return(errs);
}



//*****************************************************************************
static int _argc_argv_list(int argc, char** argv)
{
	char	buf[32];
	int		i;

	gsc_label("Arguments");
	printf("(%d argument%s)\n", argc - 1, ((argc - 1) == 1) ? "" : "s");
	gsc_label_level_inc();

	for (i = 0; i < argc; i++)
	{
		sprintf(buf, "Argument %d", i);
		gsc_label(buf);
		printf("%s\n", argv[i]);
	}

	gsc_label_level_dec();
	return(0);
}



//*****************************************************************************
static int _parse_arg(int index, char* argv, const gsc_arg_set_t* set, int* index_qty)
{
	int		errs	= 0;
	int		i;
	int		j;
	int		len;
	long	l;
	int		qty		= index_qty[0];
	char**	ppsz;

	for (i = 0;; i++)
	{
		if (i > set->qty)
		{
			// The argument was successfully parsed.
			break;
		}

		if (i == set->qty)
		{
			// The argument doesn't match any in the list.
			errs	= 1;
			printf(	"%d. ERROR: invalid argument: #%d = '%s'\n",
					__LINE__,
					index,
					argv);
			break;
		}

		switch (set->list[i].type)
		{
			default:

				// The "type" is not recognized.
				errs++;
				printf(	"%d. i %d, %s, %s\n",
						__LINE__,
						i,
						set->list[i].arg,
						set->list[i].desc);
				printf(	"%d. i %d, %s  FAIL <---  (invalid type: %d)\n",
						__LINE__,
						i,
						set->list[i].arg,
						(int) set->list[i].type);
				continue;

			case GSC_ARG_DEV_NOTE:

				continue;

			case GSC_ARG_DEV_INDEX:

				if (qty)
				{
					// One or more device index have already been specified.
					qty--;
					continue;
				}

				j	= _scan_long(argv, &l);

				if ((j == 1) && (l >= set->list[i].values[0]))
				{
					index_qty[0]++;
					set->list[i].var[0]	= l;
					i	= set->qty;
					continue;
				}

				// The device index specifies
				break;

			case GSC_ARG_S32_FLAG:

				if (strcmp(argv, set->list[i].arg))
					continue;

				set->list[i].var[0]	= set->list[i].values[0];

				if (set->list[i].desc)
					i	= set->qty;

				continue;

			case GSC_ARG_S32_MASK:

				len	= strlen(set->list[i].arg);

				if (memcmp(argv, set->list[i].arg, len))
					continue;

				j	= _scan_long(argv + len, &l);

				if ((j == 1) &&
					(((((u32) l) & (~(u32) set->list[i].values[0])) == 0)))
				{
					set->list[i].var[0]	= l;

					if (set->list[i].desc)
						i	= set->qty;

					continue;
				}

				break;

			case GSC_ARG_S32_MIN:

				len	= strlen(set->list[i].arg);

				if (memcmp(argv, set->list[i].arg, len))
					continue;

				j	= _scan_long(argv + len, &l);

				if ((j == 1) && (l >= set->list[i].values[0]))
				{
					set->list[i].var[0]	= l;

					if (set->list[i].desc)
						i	= set->qty;

					continue;
				}

				break;

			case GSC_ARG_S32_RANGE:

				len	= strlen(set->list[i].arg);

				if (memcmp(argv, set->list[i].arg, len))
					continue;

				j	= _scan_long(argv + len, &l);

				if ((j == 1) &&
					(l >= set->list[i].values[0]) &&
					(l <= set->list[i].values[1]))
				{
					set->list[i].var[0]	= l;

					if (set->list[i].desc)
						i	= set->qty;

					continue;
				}

				break;

			case GSC_ARG_STR_PTR:

				len	= strlen(set->list[i].arg);

				if (memcmp(argv, set->list[i].arg, len))
					continue;

				if ((argv[len] != '=') || (argv[len + 1] == 0))
					continue;

				ppsz	= (void*) set->list[i].var;
				ppsz[0]	= argv + len + 1;

				if (set->list[i].desc)
					i	= set->qty;

				continue;
		}

		errs	= 1;
		printf("%d. ERROR: invalid argument: #%d = '%s'\n", __LINE__, index, argv);
		fflush(stdout);
		break;
	}

	return(errs);
}



//*****************************************************************************
static int _args_parse(const gsc_arg_set_t* set, int argc, char** argv)
{
	int	errs		= 0;
	int	i;
	int	index_qty	= 0;

	for (i = 1; i < argc; i++)
	{
		errs	+=  _parse_arg(i, argv[i], set, &index_qty);
	}

	return(errs);
}



//*****************************************************************************
int gsc_args_parse(int argc, char** argv, const gsc_arg_set_t* set)
{
	int	errs	= 0;

	errs	+= _arg_show_list(set);
	errs	+= _argc_argv_list(argc, argv);
	errs	+= _args_parse(set, argc, argv);
	return(errs);
}


