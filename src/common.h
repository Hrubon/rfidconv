/* GNU licence - copyleft	  */
/* author:		Ondøej Hrubý  */
/* contributor:	David Èepelík */

#ifndef COMMON_H
#define COMMON_H

#include <stdbool.h>
#include <inttypes.h>
#include <string.h>

#ifndef	DEBUG
#define	DEBUG	0
#endif

#define	DEBUG_MSG(fmt, ...) do { \
	if (DEBUG) fprintf(stderr, "*** DEBUG *** " fmt " at %s:%d\n", \
		__VA_ARGS__, __FILE__, __LINE__); \
} while (0);

#define	ARRAY_SIZE(arr)	(sizeof (arr) / sizeof (*(arr)))

#define	OUTPUT_MAX_LEN	32


bool is_hexstr(const char *str);
bool is_decstr(const char *str);
char *substring(const char *str, int start, int len);
int64_t extract_bytes(const char *hexstr, int start, int len);
uint32_t mirror_nibbles(uint32_t code);
uint8_t comp_parity(uint64_t code);

#endif
