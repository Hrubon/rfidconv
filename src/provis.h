/* GNU licence - copyleft	  */
/* author:		Ondøej Hrubý  */
/* contributor:	David Èepelík */

#ifndef	PROVIS_H
#define	PROVIS_H

#include "tag.h"

int from_provis(const char *input, struct tag *tag);
int to_provis(const struct tag *tag, char *output);

#endif
