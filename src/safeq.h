/* GNU licence - copyleft	  */
/* author:		Ond�ej Hrub�  */
/* contributor:	David �epel�k */

#ifndef	SAFEQ_H
#define	SAFEQ_H

#include "tag.h"

int from_safeq(const char *input, struct tag *tag);
int to_safeq(const struct tag *tag, char *output);

#endif
