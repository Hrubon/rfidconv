/* GNU licence - copyleft	  */
/* author:		Ond�ej Hrub�  */
/* contributor:	David �epel�k */

#ifndef	PROVIS_H
#define	PROVIS_H

#include "tag.h"

int from_provis(const char *input, struct tag *tag);
int to_provis(const struct tag *tag, char *output);

#endif
