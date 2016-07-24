#include "provis.h"
#include "common.h"

#include <stdlib.h>
#include <stdio.h>

extern int default_cust_id;
#define	OUTPUT_MAX_LEN	32 // TODO


int
from_provis(const char *input, struct tag *tag)
{
	if (strlen(input) != 9) {
		DEBUG_MSG("%s", "from_provis: input string too long");
		return (-1);
	}

	if (!is_decstr(input)) {
		DEBUG_MSG("%s", "from_provis: input is not a decimal string");
		return (-1);
	}

	tag->has_cust_id = false;
	tag->cust_id = default_cust_id; // for formats that use it TODO

	char code[8 + 1];
	long val = strtol(input, NULL, 10);
	snprintf(code, 8 + 1, "%lx", val);

	tag->data = extract_bytes(code, 0, 8);
	return (0);
}

int
to_provis(const struct tag *tag, char *output)
{
	snprintf(output, OUTPUT_MAX_LEN, "%i", tag->data); // TODO
	return (0);
}
