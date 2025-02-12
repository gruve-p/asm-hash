/*
 * groestl512.c
 * Author: Fabjan Sukalia (fsukalia@gmail.com)
 * Date: 2014-10-02
 */
 
#include "groestl512.h"
#include "int_util.h"
#include <string.h>

static const uint8_t diffusion_matrix[8][8];
static const uint8_t table[256];

void process_blocks (block* b, const uint8_t block[], unsigned int n, bool data_bits);
void end_hash (block* b);

/* GROESTL-512 */
void groestl512_init (groestl512_context* ctxt) {
	if (ctxt == NULL)
		return;
	
	block_init_with_end (
		&ctxt->b, 
		BLOCK_SIZE_1024, 
		ctxt->buffer, 
		process_blocks, 
		end_hash, 
		ctxt->hash
	);
	groestl512_init_hash (ctxt->hash);
}

void groestl512_init_hash (uint8_t hash[8][16]) {
	memset (hash, 0, GROESTL512_HASH_SIZE);
	hash[6][15] = 0x02;
}

void groestl512_add (groestl512_context* ctxt, const uint8_t data[], size_t length) {
	block_add (&ctxt->b, length, data);
}

void groestl512_finalize (groestl512_context* ctxt) {
	block_util_finalize (
		&ctxt->b, 
		BLOCK_LENGTH_64 | BLOCK_BIG_ENDIAN | BLOCK_SIMPLE_PADDING | BLOCK_COUNT
	);
}

void groestl512_get_digest (groestl512_context* ctxt, uint8_t digest[GROESTL512_DIGEST_SIZE]) {
	for (uint_fast8_t row = 0; row < 8; row++) {
		for (uint_fast8_t column = 0; column < 8; column++) {
			digest[column * 8 + row] = ctxt->hash[row][column + 8];
		}
	}
}

/* GROESTL-224 */
void groestl384_init (groestl384_context* ctxt) {
	if (ctxt == NULL)
		return;
	
	block_init_with_end (
		&ctxt->b, 
		BLOCK_SIZE_1024, 
		ctxt->buffer, 
		process_blocks, 
		end_hash, 
		ctxt->hash
	);
	groestl384_init_hash (ctxt->hash);
}

void groestl384_init_hash (uint8_t hash[8][16]) {
	memset (hash, 0, GROESTL512_HASH_SIZE);
	hash[6][15] = 0x01;
	hash[7][15] = 0x80;
}

void groestl384_add (groestl384_context* ctxt, const uint8_t data[], size_t length) {
	groestl512_add (ctxt, data, length);
}

void groestl384_finalize (groestl384_context* ctxt) {
	groestl512_finalize (ctxt);
}

void groestl384_get_digest (groestl384_context* ctxt, uint8_t digest[GROESTL384_DIGEST_SIZE]) {	
	for (uint_fast8_t column = 2; column < 8; column++) {
		for (uint_fast8_t row = 0; row < 8; row++) {
			digest[column * 8 + row - 16] = ctxt->hash[row][column + 8];
		}
	}
}

void add_round_constants_p (uint8_t state[8][16], unsigned int round) {
	for (uint_fast8_t i = 0; i < 16; i++) {
		state[0][i] ^= round ^ (i << 4);
	}
}

void add_round_constants_q (uint8_t state[8][16], unsigned int round) {
	for (uint_fast8_t i = 0; i < 16; i++) {
		state[7][i] ^= round ^ (((15 - i) << 4) | 15);
	}
	
	for (uint_fast8_t row = 0; row < 7; row++) {
		for (uint_fast8_t column = 0; column < 16; column++) {
			state[row][column] ^= 0xFF;
		}
	}
}

void shift_bytes_p (uint8_t state[8][16]) {
	uint8_t temp[16];
	
	for (uint_fast8_t row = 1; row < 7; row++) {
		memcpy (temp, state[row], sizeof (temp));
		for (uint_fast8_t column = 0; column < 16; column++) {
			state[row][column] = temp[(column + row) % 16];
		}
	}
	
	memcpy (temp, state[7], sizeof (temp));
	for (uint_fast8_t column = 0; column < 16; column++) {
		state[7][column] = temp[(column + 11) % 16];
	}
}

void shift_bytes_q (uint8_t state[8][16]) {
	uint8_t temp[16];
	
	memcpy (temp, state[0], sizeof (temp));
	for (uint_fast8_t column = 0; column < 16; column++) {
		state[0][column] = temp[(column + 1) % 16];
	}
	
	memcpy (temp, state[1], sizeof (temp));
	for (uint_fast8_t column = 0; column < 16; column++) {
		state[1][column] = temp[(column + 3) % 16];
	}
	
	memcpy (temp, state[2], sizeof (temp));
	for (uint_fast8_t column = 0; column < 16; column++) {
		state[2][column] = temp[(column + 5) % 16];
	}
	
	memcpy (temp, state[3], sizeof (temp));
	for (uint_fast8_t column = 0; column < 16; column++) {
		state[3][column] = temp[(column + 11) % 16];
	}
	
	memcpy (temp, state[5], sizeof (temp));
	for (uint_fast8_t column = 0; column < 16; column++) {
		state[5][column] = temp[(column + 2) % 16];
	}
	
	memcpy (temp, state[6], sizeof (temp));
	for (uint_fast8_t column = 0; column < 16; column++) {
		state[6][column] = temp[(column + 4) % 16];
	}
	
	memcpy (temp, state[7], sizeof (temp));
	for (uint_fast8_t column = 0; column < 16; column++) {
		state[7][column] = temp[(column + 6) % 16];
	}
}

void sub_bytes (uint8_t state[8][16]) {
	for (uint_fast8_t row = 0; row < 8; row++) {
		for (uint_fast8_t column = 0; column < 16; column++) {
			state[row][column] = table[state[row][column]];
		}
	}
}

static uint8_t gf_mult (uint8_t a, uint8_t b) {
	/* x^8 + x^4 + x^3 + x^1 + 1 = 0x11B */
	uint8_t result = 0;
	for (uint_fast8_t i = 0; i < 8; i++) {
		if ((b & 0x01) != 0) {
			result ^= a;
		}
		b = b >> 1;
		bool carry = (a & 0x80) != 0;
		a = a << 1;
		if (carry) {
			a ^= 0x1B;
		}
	}
	return result;
}

static void mix_bytes (uint8_t block[8][16]) {
	uint8_t temp[8][16];
	
	memset (temp, 0, sizeof (temp));	
	for (uint_fast8_t i = 0; i < 8; i++) {
		for (uint_fast8_t j = 0; j < 16; j++) {
			for (uint_fast8_t k = 0; k < 8; k++) {
				temp[i][j] ^= gf_mult (diffusion_matrix[i][k], block[k][j]);
			}
		}
	}	
	memcpy (block, temp, sizeof (temp));
}

void groestl512_process_block (const uint8_t block[128], uint8_t hash[8][16], unsigned int n) {
	uint8_t message[8][16];
	uint8_t state[8][16];
	
	memcpy (state, hash, sizeof (state));
	
	for (uint_fast8_t row = 0; row < 8; row++) {
		for (uint_fast8_t column = 0; column < 16; column++) {
			message[row][column] = block[column * 8 + row];
			state[row][column] ^= message[row][column];
		}
	}
	
	for (uint_fast8_t r = 0; r < 14; r++) {
		/* P (state) */
		add_round_constants_p (state, r);
		sub_bytes (state);
		shift_bytes_p (state);
		mix_bytes (state);
	}
	
	for (uint_fast8_t r = 0; r < 14; r++) {
		/* Q (message) */
		add_round_constants_q (message, r);
		sub_bytes (message);
		shift_bytes_q (message);
		mix_bytes (message);
	}
	
	for (uint_fast8_t row = 0; row < 8; row++) {
		for (uint_fast8_t column = 0; column < 16; column++) {
			hash[row][column] ^= state[row][column] ^ message[row][column];
		}
	}
}

void process_blocks (block* b, const uint8_t block[], unsigned int n, bool data_bits) {
	(void)data_bits;
	for (unsigned int i = 0; i < n; i++) {
		groestl512_process_block (&block[i * 128], (uint8_t(*)[16])(b->func_data), n);
	}
}

void end_hash (block* b) {
	uint8_t* hash = (uint8_t*)(b->func_data);
	uint8_t state[8][16];
	memcpy (state, hash, sizeof (state));
	
	for (uint_fast8_t r = 0; r < 14; r++) {
		/* P (state) */
		add_round_constants_p (state, r);
		sub_bytes (state);
		shift_bytes_p (state);
		mix_bytes (state);
	}
	
	for (uint_fast8_t row = 0; row < 8; row++) {
		for (uint_fast8_t column = 0; column < 16; column++) {
			hash[row * 16 + column] ^= state[row][column];
		}
	}
}

static const uint8_t table[256] = {
	0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5, 
	0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76, /*  16 */
	0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0,
	0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0, /*  32 */
	0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC,
	0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15, /*  48 */
	0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A,
	0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75, /*  64 */
	0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0,
	0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84, /*  80 */
	0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B,
	0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF, /*  96 */
	0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85,
	0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8, /* 112 */
	0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5, 
	0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2, /* 128 */
	0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17,
	0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73, /* 144 */
	0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88,
	0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB, /* 160 */
	0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C,
	0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79, /* 176 */
	0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9,
	0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08, /* 192 */
	0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6,
	0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A, /* 208 */
	0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E,
	0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E, /* 224 */
	0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94,
	0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF, /* 240 */
	0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68,
	0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16  /* 256 */
};

static const uint8_t diffusion_matrix[8][8] = {
	{2, 2, 3, 4, 5, 3, 5, 7},
	{7, 2, 2, 3, 4, 5, 3, 5},
	{5, 7, 2, 2, 3, 4, 5, 3},
	{3, 5, 7, 2, 2, 3, 4, 5},
	{5, 3, 5, 7, 2, 2, 3, 4},
	{4, 5, 3, 5, 7, 2, 2, 3},
	{3, 4, 5, 3, 5, 7, 2, 2},
	{2, 3, 4, 5, 3, 5, 7, 2}
};
