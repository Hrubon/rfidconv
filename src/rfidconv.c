#define _CRT_SECURE_NO_WARNINGS

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "common.h"

#include "alpus.h"
#include "provis.h"
#include "safeq.h"


#define	KEYWORD_MAX_LEN	16
#define DEFAULT_CUST_ID 4


int default_cust_id;


struct format
{
	char keyword[KEYWORD_MAX_LEN + 1];
	int (*from)(const char *input, struct tag *tag);
	int (*to)(const struct tag *tag, char *output);
};


static int not_implemented(const char *input, struct tag *tag)
{
	(void) input;
	(void) tag;

	DEBUG_MSG("%s", "conversion not implemented");

	return (-1);
}

static const struct format formats[] = {
	{ "safeq",	from_safeq,		to_safeq },
	{ "provis",	from_provis,		to_provis },
	{ "alpus",	from_alpus,		to_alpus },
	{ "alpus-dec",	not_implemented,	to_alpus_dec }
};


// _Noreturn removed - cannot compile under Windows...
static void die(char *format, ...) {
	va_list args;

	va_start(args, format);
	vfprintf(stderr, format, args);
	va_end(args);

	exit(EXIT_FAILURE);
}

static const struct format *select_format(char *keyword)
{
	for (unsigned i = 0; i < ARRAY_SIZE(formats); i++)
	{
		if (strcmp(formats[i].keyword, keyword) == 0)
			return &formats[i];
	}

	return (NULL);
}

static void usage(const char *argv0)
{
	fprintf(stderr, "Usage: %s <src_format> <dst_format> <rfid> [cust_id]\n", argv0);
	fprintf(stderr, "Supported formats:");

	for (unsigned i = 0; i < ARRAY_SIZE(formats); i++)
	{
		fprintf(stderr, " %s", formats[i].keyword);
	}
	fprintf(stderr, ".\n");
}

int main(int argc, char *argv[])
{
	char *from, *to, *what;
	const struct format *input_format, *output_format;
	struct tag tag;
	char output[OUTPUT_MAX_LEN + 1];

	default_cust_id = DEFAULT_CUST_ID;

	if (argc != 4 && argc != 5)
	{
		usage(argv[0]);
		return (EXIT_FAILURE);
	}

	from = argv[1];
	to = argv[2];
	what = argv[3];

	// optional cust_id argument
	if (argc == 5)
	{
		if (!is_hexstr(argv[4]))
			die("%s: cust_id is expected to be a hex byte\n", argv[0]);

		default_cust_id = strtol(argv[4], NULL, 16);

		if (default_cust_id < 0 || default_cust_id > 255)
			die("%s: cust_id must be non-negative and less than 255\n", argv[0]);
	}

	if ((input_format = select_format(from)) == NULL)
		die("%s: unknown input format: %s.\n", argv[0], from);

	if ((output_format = select_format(to)) == NULL)
		die("%s: unknown output format: %s.\n", argv[0], to);

	if (input_format->from(what, &tag) != 0)
		die("%s: error when parsing RFID of format %s.\n", argv[0], input_format->keyword);

	if (output_format->to(&tag, output) != 0)
		die("%s: error when converting RFID to format %s.\n", argv[0], output_format->keyword);

	//printf("%s\n", output);
	//return (EXIT_SUCCESS);
}
