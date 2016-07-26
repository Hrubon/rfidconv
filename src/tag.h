/* GNU licence - copyleft	  */
/* author:		Ondøej Hrubý  */
/* contributor:	David Èepelík */

#ifndef	TAG_H
#define	TAG_H

#include <inttypes.h>
#include <stdbool.h>

struct tag
{
	uint8_t cust_id;	/* 8-bit version ID or customer ID */
	bool has_cust_id;	/* is cust_id present? */
	uint32_t data;		/* 32 data bits */
};

#endif
