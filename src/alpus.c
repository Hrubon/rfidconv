#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

#include "alpus.h"
#include "common.h"
#include "crc8.h"

#define	OUTPUT_MAX_LEN	32 // TODO

int default_cust_id;


int to_alpus_dec(const struct tag *tag, char *output)
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
	uint8_t crc = crc8(code, parity);

	// Print result
	snprintf(output, OUTPUT_MAX_LEN, "255:%i:%s%i", 0xF0 + parity, data_str, crc);

	return 0;
}


int from_alpus(const char *input, struct tag *tag)
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
	uint8_t crc = crc8(code, parity);
	uint8_t input_crc = extract_bytes(input, 16, 2);
	if (crc != input_crc)
		return 4;

	return 0;
}

int to_alpus(const struct tag *tag, char *output)
{
	uint8_t cust_id = mirror_nibbles(tag->cust_id);
	uint32_t data = mirror_nibbles(tag->data);
	char code_str[10 + 1];
	snprintf(code_str, sizeof(code_str), "%02X%08X", cust_id, data);

	uint64_t code = strtoll(code_str, NULL, 16);
	uint8_t parity = comp_parity(code);
	uint8_t crc = crc8(code, parity);

	snprintf(output, 18 + 1, "FF-F%1X%s*%02X", parity, code_str, crc);

	return 0;
}
