#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdarg.h>
#include <ctype.h>


#define	KEYWORD_MAX_LEN	10
#define	OUTPUT_MAX_LEN	32
#define DEFAULT_CUST_ID 4


#ifndef	DEBUG
#define	DEBUG	0
#endif

#define	DEBUG_MSG(fmt, ...) do { \
	if (DEBUG) fprintf(stderr, "*** DEBUG *** " fmt " at %s:%d\n", \
		__VA_ARGS__, __FILE__, __LINE__); \
} while (0);


#define	ARRAY_SIZE(arr)	(sizeof (arr) / sizeof (*(arr)))



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

/* Lookup table for computing CRC8 */
static const uint8_t crc8_table[] =
{
	0, 94, 188, 226, 97, 63, 221, 131, 194, 156, 126,
	32, 163, 253, 31, 65, 157, 195, 33, 127, 252, 162,
	64, 30, 95, 1, 227, 189, 62, 96, 130, 220, 35,
	125, 159, 193, 66, 28, 254, 160, 225, 191, 93, 3,
	128, 222, 60, 98, 190, 224, 2, 92, 223, 129, 99,
	61, 124, 34, 192, 158, 29, 67, 161, 255, 70, 24,
	250, 164, 39, 121, 155, 197, 132, 218, 56, 102,
	229, 187, 89, 7, 219, 133, 103, 57, 186, 228, 6,
	88, 25, 71, 165, 251, 120, 38, 196, 154, 101, 59,
	217, 135, 4, 90, 184, 230, 167, 249, 27, 69, 198,
	152, 122, 36, 248, 166, 68, 26, 153, 199, 37, 123,
	58, 100, 134, 216, 91, 5, 231, 185, 140, 210, 48,
	110, 237, 179, 81, 15, 78, 16, 242, 172, 47, 113,
	147, 205, 17, 79, 173, 243, 112, 46, 204, 146,
	211, 141, 111, 49, 178, 236, 14, 80, 175, 241, 19,
	77, 206, 144, 114, 44, 109, 51, 209, 143, 12, 82,
	176, 238, 50, 108, 142, 208, 83, 13, 239, 177,
	240, 174, 76, 18, 145, 207, 45, 115, 202, 148, 118,
	40, 171, 245, 23, 73, 8, 86, 180, 234, 105, 55,
	213, 139, 87, 9, 235, 181, 54, 104, 138, 212, 149,
	203, 41, 119, 244, 170, 72, 22, 233, 183, 85, 11,
	136, 214, 52, 106, 43, 117, 151, 201, 74, 20, 246,
	168, 116, 42, 200, 150, 21, 75, 169, 247, 182, 232,
	10, 84, 215, 137, 107, 53
};



int default_cust_id = DEFAULT_CUST_ID;



struct tag
{
	uint8_t cust_id;		/* 8-bit version ID or customer ID */
	uint32_t data;			/* 32 data bits */
	bool has_cust_id;
};


struct format
{
	char keyword[KEYWORD_MAX_LEN + 1];
	int (*from)(const char *input, struct tag *tag);
	int (*to)(const struct tag *tag, char *output);
};


_Noreturn static void *die(char *format, ...) {
	va_list args;

	va_start(args, format);
	vfprintf(stderr, format, args);
	va_end(args);

	exit(EXIT_FAILURE);
}

static bool is_hexstr(const char *str)
{
	for (unsigned i = 0; str[i] != '\0'; i++)
	{
		if (!isxdigit(str[i]))
			return false;
	}

	return true;
}

static bool is_decstr(const char *str)
{
	for (unsigned i = 0; str[i] != '\0'; i++)
	{
		if (!isdigit(str[i]))
			return false;
	}

	return true;
}

static char *substring(const char *str, int start, int len)
{
	char *substr = (char*)malloc(len + 1);
	memcpy(substr, &str[start], len);
	substr[len] = '\0';

	return substr;
}

/* Extracts bytes to integer from hexstring */
static int64_t extract_bytes(const char *hexstr, int start, int len)
{
	// Get substring from input string
	char *substr = substring(hexstr, start, len);

	// Convert it to integer
	int64_t val = strtoll(substr, NULL, 16);

	free(substr);

	return val;
}

/* Mirrors bits order in all nibbles in the given integer, e.g. 1000 0010 -> 0001 0100 */
static uint32_t mirror_nibbles(uint32_t code)
{
	uint8_t *input_bytes = (uint8_t*)&code;
	uint32_t result = 0;
	for (int i = sizeof(uint32_t) - 1; i >= 0; i--)
	{
		result |= mirror_table[input_bytes[i]] << i * 8;
	}

	return result;
}

static uint8_t comp_parity(uint64_t code)
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

static uint8_t comp_crc(uint64_t code, uint8_t parity)
{
	// Prepare CRC input (bytes are in reversed order)
	uint8_t *code_bytes = (uint8_t*)&code;
	uint64_t crc_input = 0xFF; // Constant prefix (not part of ID)
	for (int i = 0; i < 5; i++)
	{
		crc_input <<= 8;
		crc_input |= code_bytes[i]; // ID in reverse order
	}
	crc_input <<= 8;
	crc_input |= 0xF0 | parity; // Append parity nibble to the end

	// Compute CRC by lookup table
	uint8_t *input_bytes = (uint8_t*)&crc_input;
	uint8_t crc = 0;
	for (int i = sizeof(uint64_t) - 1; i >= 0; i--)
	{
		crc = crc8_table[crc ^ input_bytes[i]];
	}

	return crc;
}



static int from_safeq(const char *input, struct tag *tag)
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

static int to_safeq(const struct tag *tag, char *output)
{
	char cust[2 + 1], data[8 + 1];

	snprintf(cust, 2 + 1, "%02x", tag->cust_id);
	snprintf(data, 8 + 1, "%08x", tag->data);

	strncpy(output, cust, 2 + 1);
	strncat(output, data, 8 + 1);

	return 0;
}


static int from_provis(const char *input, struct tag *tag)
{
	if (strlen(input) != 9)
		return 1;

	if (!is_decstr(input))
		return 2;

	tag->has_cust_id = false; // sporadically, then hasn't
	tag->cust_id = default_cust_id; // for formats that use it

	char code[8 + 1];
	long val = strtol(input, NULL, 10);
	snprintf(code, 8 + 1, "%lx", val);
	tag->data = extract_bytes(code, 0, 8);

	return 0;
}

static int to_provis(const struct tag *tag, char *output)
{
	snprintf(output, OUTPUT_MAX_LEN, "%i", tag->data);

	return 0;
}


static int from_alpus(const char *input, struct tag *tag)
{
	if (strlen(input) != 18)
		return 1;

	for (unsigned i = 0; i < strlen(input); i++)
	{
		switch (i)
		{
		case 0:
		case 1:
		case 3:
			if (input[i] != 'F')
				return 2;
			break;
		case 2:
			if (input[i] != '-')
				return 2;
			break;
		case 15:
			if (input[i] != '*')
				return 2;
			break;
		default:
			if (!isxdigit(input[i]))
				return 2;
			break;
		}
	}

	// Cut prefix, parity and crc
	char *code_str = substring(input, 5, 10);
	uint64_t code = strtoll(code_str, NULL, 16);

	// Parse data
	tag->has_cust_id = true;

	uint8_t cust_id = extract_bytes(code_str, 0, 2);
	tag->cust_id = mirror_nibbles(cust_id);

	uint32_t data = extract_bytes(code_str, 2, 8);
	tag->data = mirror_nibbles(data);

	free(code_str);

	// Check parity
	uint8_t parity = comp_parity(code);
	uint8_t input_parity = extract_bytes(input, 4, 1);
	if (parity != input_parity)
		return 3;

	// Check CRC
	uint8_t crc = comp_crc(code, parity);
	uint8_t input_crc = extract_bytes(input, 16, 2);
	if (crc != input_crc)
		return 4;

	return 0;
}

static int to_alpus(const struct tag *tag, char *output)
{
	uint8_t cust_id = mirror_nibbles(tag->cust_id);
	uint32_t data = mirror_nibbles(tag->data);
	char code_str[10 + 1];
	snprintf(code_str, sizeof(code_str), "%02X%08X", cust_id, data);

	uint64_t code = strtoll(code_str, NULL, 16);
	uint8_t parity = comp_parity(code);
	uint8_t crc = comp_crc(code, parity);

	snprintf(output, 18 + 1, "FF-F%1X%s*%02X", parity, code_str, crc);

	return 0;
}


static int not_implemented(const char *input, struct tag *tag)
{
	(void) input;
	(void) tag;

	return (-1);
}


static int to_alpus_dec(const struct tag *tag, char *output)
{
	uint8_t cust_id = mirror_nibbles(tag->cust_id);
	uint32_t data = mirror_nibbles(tag->data);
	uint64_t code = ((uint64_t)cust_id << 8 * sizeof(data)) | data;

	// Collect ID from data by bytes and conv them to DEC
	uint8_t *code_bytes = (uint8_t*)&code;
	char data_str[20 + 1] = "";
	for (int i = 4; i >= 0; i--)
	{
		char byte[4 + 1];
		snprintf(byte, 4 + 1, "%i:", code_bytes[i]);
		strncat(data_str, byte, 4 + 1);
	}

	// Compute parity nibble and CRC byte
	uint8_t parity = comp_parity(code);
	uint8_t crc = comp_crc(code, parity);

	// Print result
	snprintf(output, OUTPUT_MAX_LEN, "255:%i:%s%i", 0xF0 + parity, data_str, crc);

	return 0;
}


static const struct format formats[] = {
	{ "safeq", from_safeq, to_safeq },
	{ "provis", from_provis, to_provis },
	{ "alpus", from_alpus, to_alpus },
	{ "alpusdec", not_implemented, to_alpus_dec }
};


static const struct format *get_format(char *keyword)
{
	for (unsigned i = 0; i < ARRAY_SIZE(formats); i++)
	{
		if (strcmp(formats[i].keyword, keyword) == 0)
			return &formats[i];
	}

	return (NULL);
}


static void usage(const char *argv0)
{
	fprintf(stderr, "Usage: %s <src_format> <dst_format> <rfid> [cust_id]\n", argv0);
	fprintf(stderr, "Supported formats:");

	for (unsigned i = 0; i < ARRAY_SIZE(formats); i++) {
		fprintf(stderr, " %s", formats[i].keyword);
	}
	fprintf(stderr, ".\n");
}


int main(int argc, char *argv[])
{
	char *from, *to, *what;
	const struct format *input_format, *output_format;
	struct tag tag;
	char output[OUTPUT_MAX_LEN + 1];

	if (argc != 4 && argc != 5) {
		usage(argv[0]);
		return (EXIT_FAILURE);
	}

	from = argv[1];
	to = argv[2];
	what = argv[3];

	// optional cust_id argument
	if (argc == 5) {
		if (!is_hexstr(argv[4]))
			die("%s: cust_id is expected to be a hex byte\n", argv[0]);

		default_cust_id = strtol(argv[4], NULL, 16);

		if (default_cust_id < 0 || default_cust_id > 255)
			die("%s: cust_id must be non-negative and less than 255\n", argv[0]);
	}

	if ((input_format = get_format(from)) == NULL)
		die("%s: unknown input format: %s.\n", argv[0], from);

	if ((output_format = get_format(to)) == NULL)
		die("%s: unknown output format: %s.\n", argv[0], to);

	if (input_format->from(what, &tag) != 0)
		die("%s: error when parsing RFID of format %s.\n", argv[0], input_format->keyword);

	if (output_format->to(&tag, output) != 0)
		die("%s: error when converting RFID to format %s.\n", argv[0], output_format->keyword);

	printf("%s\n", output);
	return (EXIT_SUCCESS);
}
