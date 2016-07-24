#include <string.h>
#include <stdio.h>

#include "safeq.h"
#include "common.h"


int
from_safeq(const char *input, struct tag *tag)
{
	if (strlen(input) != 10)
		return 1;

	if (!is_hexstr(input))
		return 2;

	tag->has_cust_id = true;
	tag->cust_id = extract_bytes(input, 0, 2); // first byte is cust_id
	tag->data = extract_bytes(input, 2, 8); // the rest is tag data

	return 0;
}

int
to_safeq(const struct tag *tag, char *output)
{
	char cust[2 + 1], data[8 + 1];

	snprintf(cust, 2 + 1, "%02x", tag->cust_id);
	snprintf(data, 8 + 1, "%08x", tag->data);

	strncpy(output, cust, 2 + 1);
	strncat(output, data, 8 + 1);

	return 0;
}
