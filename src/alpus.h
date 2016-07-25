#ifndef	ALPUS_H
#define	ALPUS_H

#include "tag.h"

int from_alpus(const char *input, struct tag *tag);
int to_alpus(const struct tag *tag, char *output);

int to_alpus_dec(const struct tag *tag, char *output);

#endif
