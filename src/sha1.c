/*
 * sha1.c
 * Author: Fabjan Sukalia (fsukalia@gmail.com)
 * Date: 2014-04-25
 */

#include "sha1.h"
#include "int_util.h"

void process_block (block* b, const uint8_t block[], unsigned int n, bool data_bits) {
	(void)data_bits;
#ifdef SHA1_USE_ASM
	sha1_process_blocks_asm (block, (uint32_t*)(b->func_data), n);
#else
	sha1_process_blocks (block, (uint32_t*)(b->func_data), n);
#endif
}

void sha1_init (sha1_context* ctxt) {
	if (ctxt == NULL)
		return;
	
	sha1_init_hash (ctxt->hash);	
	block_init (&ctxt->b, BLOCK_SIZE_512, ctxt->buffer, process_block, ctxt->hash);
}

void sha1_init_hash (uint32_t hash[5]) {
	hash[0] = UINT32_C (0x67452301);
	hash[1] = UINT32_C (0xEFCDAB89);
	hash[2] = UINT32_C (0x98BADCFE);
	hash[3] = UINT32_C (0x10325476);
	hash[4] = UINT32_C (0xC3D2E1F0);
}

void sha1_add (sha1_context* ctxt, const uint8_t data[], size_t length) {
	block_add (&ctxt->b, length, data);
}

void sha1_finalize (sha1_context* ctxt) {
	block_util_finalize (&ctxt->b, BLOCK_LENGTH_64 | BLOCK_BIG_ENDIAN | BLOCK_SIMPLE_PADDING);
}

void sha1_get_digest (sha1_context* ctxt, uint8_t digest[SHA1_DIGEST_SIZE]) {
	for (size_t i = 0; i < 5; i++) {
		u32_to_u8_be (ctxt->hash[i], &digest[i * 4]);
	}
}

void sha1_process_blocks (const uint8_t block[], uint32_t hash[5], unsigned int n) {
	for (unsigned int j = 0; j < n; j++) {
		const uint8_t* block1 = &block[j * 64];
		
		uint32_t a = hash[0];
		uint32_t b = hash[1];
		uint32_t c = hash[2];
		uint32_t d = hash[3];
		uint32_t e = hash[4];
		
		uint32_t temp;
		uint32_t k;
		uint32_t f;
		uint32_t W[80];
		
		k = UINT32_C (0x5A827999);
		for (uint_fast8_t i = 0; i < 16; i++) {
			W[i] = u8_to_u32_be (&block1[i * 4]);
			
			f = (b & c) | ((~b) & d);
			temp = W[i] + f + k + rotate_left_32 (a, 5) + e;
			
			e = d;
			d = c;
			c = rotate_left_32 (b, 30);
			b = a;
			a = temp;
		}
		for (uint_fast8_t i = 16; i < 20; i++) {
			W[i] = rotate_left_32 (W[i - 3] ^ W[i - 8] ^ W[i - 14] ^ W[i - 16], 1);
			
			f = (b & c) | ((~b) & d);
			temp = W[i] + f + k + rotate_left_32 (a, 5) + e;
			
			e = d;
			d = c;
			c = rotate_left_32 (b, 30);
			b = a;
			a = temp;
		}
		
		k = UINT32_C (0x6ED9EBA1);
		for (uint_fast8_t i = 20; i < 40; i++) {
			W[i] = rotate_left_32 (W[i - 3] ^ W[i - 8] ^ W[i - 14] ^ W[i - 16], 1);
			
			f = b ^ c ^ d; 
			temp = W[i] + f + k + rotate_left_32 (a, 5) + e;
			
			e = d;
			d = c;
			c = rotate_left_32 (b, 30);
			b = a;
			a = temp;
		}
		
		k = UINT32_C (0x8F1BBCDC);
		for (uint_fast8_t i = 40; i < 60; i++) {
			W[i] = rotate_left_32 (W[i - 3] ^ W[i - 8] ^ W[i - 14] ^ W[i - 16], 1);
			
			f = (b & c) | (b & d) | (c & d);
			temp = W[i] + f + k + rotate_left_32 (a, 5) + e;
			
			e = d;
			d = c;
			c = rotate_left_32 (b, 30);
			b = a;
			a = temp;
		}
		
		k = UINT32_C (0xCA62C1D6);
		for (uint_fast8_t i = 60; i < 80; i++) {
			W[i] = rotate_left_32 (W[i - 3] ^ W[i - 8] ^ W[i - 14] ^ W[i - 16], 1);
			
			f = b ^ c ^ d; 
			temp = W[i] + f + k + rotate_left_32 (a, 5) + e;
			
			e = d;
			d = c;
			c = rotate_left_32 (b, 30);
			b = a;
			a = temp;
		}
		
		hash[0] += a;
		hash[1] += b;
		hash[2] += c;
		hash[3] += d;
		hash[4] += e;
	}
}
