/* GNU licence - copyleft	  */
/* author:		Ondøej Hrubý  */
/* contributor:	David Èepelík */

#ifndef	SAFEQ_H
#define	SAFEQ_H

#include "tag.h"

int from_safeq(const char *input, struct tag *tag);
int to_safeq(const struct tag *tag, char *output);

#endif
