/* GNU licence - copyleft	  */
/* author:		Ondøej Hrubý  */
/* contributor:	David Èepelík */

#include "common.h"

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>


/* Lookup table for nibble bit mirroring, e.g. 1000 0010 -> 0001 0100 */
static const uint8_t mirror_table[] = {
	0, 8, 4, 12, 2, 10, 6, 14, 1, 9, 5,
	13, 3, 11, 7, 15, 128, 136, 132, 140, 130,
	138, 134, 142, 129, 137, 133, 141, 131, 139, 135,
	143, 64, 72, 68, 76, 66, 74, 70, 78, 65,
	73, 69, 77, 67, 75, 71, 79, 192, 200, 196,
	204, 194, 202, 198, 206, 193, 201, 197, 205, 195,
	203, 199, 207, 32, 40, 36, 44, 34, 42, 38,
	46, 33, 41, 37, 45, 35, 43, 39, 47, 160,
	168, 164, 172, 162, 170, 166, 174, 161, 169, 165,
	173, 163, 171, 167, 175, 96, 104, 100, 108, 98,
	106, 102, 110, 97, 105, 101, 109, 99, 107, 103,
	111, 224, 232, 228, 236, 226, 234, 230, 238, 225,
	233, 229, 237, 227, 235, 231, 239, 16, 24, 20,
	28, 18, 26, 22, 30, 17, 25, 21, 29, 19,
	27, 23, 31, 144, 152, 148, 156, 146, 154, 150,
	158, 145, 153, 149, 157, 147, 155, 151, 159, 80,
	88, 84, 92, 82, 90, 86, 94, 81, 89, 85,
	93, 83, 91, 87, 95, 208, 216, 212, 220, 210,
	218, 214, 222, 209, 217, 213, 221, 211, 219, 215,
	223, 48, 56, 52, 60, 50, 58, 54, 62, 49,
	57, 53, 61, 51, 59, 55, 63, 176, 184, 180,
	188, 178, 186, 182, 190, 177, 185, 181, 189, 179,
	187, 183, 191, 112, 120, 116, 124, 114, 122, 118,
	126, 113, 121, 117, 125, 115, 123, 119, 127, 240,
	248, 244, 252, 242, 250, 246, 254, 241, 249, 245,
	253, 243, 251, 247, 255
};


bool is_hexstr(const char *str)
{
	for (unsigned i = 0; str[i] != '\0'; i++)
	{
		if (!isxdigit(str[i]))
			return false;
	}

	return true;
}

bool is_decstr(const char *str)
{
	for (unsigned i = 0; str[i] != '\0'; i++)
	{
		if (!isdigit(str[i]))
			return false;
	}

	return true;
}

char *substring(const char *str, int start, int len)
{
	char *substr = (char*)malloc(len + 1);
	memcpy(substr, &str[start], len);
	substr[len] = '\0';

	return substr;
}

/* Extracts bytes to integer from hexstring */
int64_t extract_bytes(const char *hexstr, int start, int len)
{
	// Get substring from input string
	char *substr = substring(hexstr, start, len);

	// Convert it to integer
	int64_t val = strtoll(substr, NULL, 16);

	free(substr);

	return val;
}

/* Mirrors bits order in all nibbles in the given integer, e.g. 1000 0010 -> 0001 0100 */
uint32_t mirror_nibbles(uint32_t code)
{
	uint8_t *input_bytes = (uint8_t*)&code;
	uint32_t result = 0;
	for (int i = sizeof(uint32_t) - 1; i >= 0; i--)
	{
		result |= mirror_table[input_bytes[i]] << i * 8;
	}

	return result;
}


uint8_t comp_parity(uint64_t code)
{
	uint8_t parity = 0;
	for (int i = 0; i < 4; i++)
	{
		uint8_t bit = 0;
		for (int j = 0; j < 10; j++)
		{
			bit ^= (code >> (4 * j + i)) & 1;
		}
		parity |= bit << i;
	}

	return parity;
}

