/*
 * sha1.h
 * Author: Fabjan Sukalia (fsukalia@gmail.com)
 * Date: 2014-04-25
 */

#ifndef SHA1_H
#define SHA1_H

#include <stdint.h>
#include "block.h"

#define SHA1_BLOCK_SIZE 64
#define SHA1_DIGEST_SIZE 20
#define SHA1_HASH_SIZE 20

typedef struct {
	uint8_t buffer[64];
	uint32_t hash[5];
	block b;
} sha1_context;

void sha1_init (sha1_context* ctxt);
void sha1_init_hash (uint32_t hash[5]);
void sha1_add (sha1_context* ctxt, const uint8_t data[], size_t length);
void sha1_finalize (sha1_context* ctxt);
void sha1_get_digest (sha1_context* ctxt, uint8_t digest[SHA1_DIGEST_SIZE]);

void sha1_process_blocks (const uint8_t block[], uint32_t hash[5], unsigned int n);

#if defined (SHA1_USE_ASM) || defined (SHA1_ENABLE_ASM)
extern void sha1_process_blocks_asm (const uint8_t block[], uint32_t hash[5], unsigned int n);
#endif

#endif
