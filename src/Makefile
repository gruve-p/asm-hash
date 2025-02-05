CC=gcc
CFLAGS=
INCLUDE_PATH=-I.

.PHONY: all clean

all: all_c all_armv6

clean:
	rm -rf md5sum sha1sum sha224sum sha256sum sha384sum sha512sum sha512_224sum sha512_256sum blake224sum blake256sum blake384sum blake512sum groestl224sum groestl256sum groestl384sum groestl512sum whirlpoolsum
	rm -rf md5sum.exe sha1sum.exe sha224sum.exe sha256sum.exe sha384sum.exe sha512sum.exe sha512_224sum.exe sha512_256sum.exe blake224sum.exe blake256sum.exe blake384sum.exe blake512sum.exe groestl224sum.exe groestl256sum.exe groestl384sum.exe groestl512sum.exe whirlpoolsum.exe

# C versions
all_c: md5sum sha1sum sha2 whirlpoolsum blake groestl

sha2: sha224sum sha256sum sha384sum sha512sum sha512_224sum sha512_256sum

blake: blake224sum blake256sum blake384sum blake512sum

groestl: groestl224sum groestl256sum groestl384sum groestl512sum

# Assembly versions
all_armv6: md5sum_armv6 sha1sum_armv6 sha2_armv6 blake_armv6

sha2_armv6: sha224sum_armv6 sha256sum_armv6 sha384sum_armv6 sha512sum_armv6 sha512_224sum_armv6 sha512_256sum_armv6

blake_armv6: blake224sum_armv6 blake256sum_armv6 blake384sum_armv6 blake512sum_armv6

# MD5
md5sum:
	$(CC) -std=c99 -o md5sum md5.c block.c int_util.c hashsum.c -DHASH_MD5 $(INCLUDE_PATH) $(CFLAGS)

md5sum_armv6:
	$(CC) -std=c99 -o md5sum md5.c md5_armv6.s -DMD5_USE_ASM block.c int_util.c hashsum.c -DHASH_MD5 $(INCLUDE_PATH) $(CFLAGS)

# SHA-1
sha1sum:
	$(CC) -std=c99 -o sha1sum sha1.c block.c int_util.c hashsum.c -DHASH_SHA1 $(INCLUDE_PATH) $(CFLAGS)
	
sha1sum_armv6: 
	$(CC) -std=c99 -o sha1sum sha1.c sha1_armv6.s -DSHA1_USE_ASM block.c int_util.c hashsum.c -DHASH_SHA1 $(INCLUDE_PATH) $(CFLAGS)
	
# SHA-2
sha224sum:
	$(CC) -std=c99 -o sha224sum sha256.c block.c int_util.c hashsum.c -DHASH_SHA224 $(INCLUDE_PATH) $(CFLAGS)
	
sha224sum_armv6:
	$(CC) -std=c99 -o sha224sum sha256.c sha256_armv6.s -DSHA256_USE_ASM block.c int_util.c hashsum.c -DHASH_SHA224 $(INCLUDE_PATH) $(CFLAGS)

sha256sum:
	$(CC) -std=c99 -o sha256sum sha256.c block.c int_util.c hashsum.c -DHASH_SHA256 $(INCLUDE_PATH) $(CFLAGS)

sha256sum_armv6:
	$(CC) -std=c99 -o sha256sum sha256.c sha256_armv6.s -DSHA256_USE_ASM block.c int_util.c hashsum.c -DHASH_SHA256 $(INCLUDE_PATH) $(CFLAGS)

sha384sum:
	$(CC) -std=c99 -o sha384sum sha512.c block.c int_util.c hashsum.c -DHASH_SHA384 $(INCLUDE_PATH) $(CFLAGS)

sha384sum_armv6:
	$(CC) -std=c99 -o sha384sum sha512.c sha512_armv6.s -DSHA512_USE_ASM block.c int_util.c hashsum.c -DHASH_SHA384 $(INCLUDE_PATH) $(CFLAGS)

sha512sum:
	$(CC) -std=c99 -o sha512sum sha512.c block.c int_util.c hashsum.c -DHASH_SHA512 $(INCLUDE_PATH) $(CFLAGS)

sha512sum_armv6:
	$(CC) -std=c99 -o sha512sum sha512.c sha512_armv6.s -DSHA512_USE_ASM block.c int_util.c hashsum.c -DHASH_SHA512 $(INCLUDE_PATH) $(CFLAGS)

sha512_224sum:
	$(CC) -std=c99 -o sha512_224sum sha512.c block.c int_util.c hashsum.c -DHASH_SHA512_224 $(INCLUDE_PATH) $(CFLAGS)

sha512_224sum_armv6:
	$(CC) -std=c99 -o sha512_224sum sha512.c sha512_armv6.s -DSHA512_USE_ASM block.c int_util.c hashsum.c -DHASH_SHA512_224 $(INCLUDE_PATH) $(CFLAGS)	

sha512_256sum:
	$(CC) -std=c99 -o sha512_256sum sha512.c block.c int_util.c hashsum.c -DHASH_SHA512_256 $(INCLUDE_PATH) $(CFLAGS)

sha512_256sum_armv6:
	$(CC) -std=c99 -o sha512_256sum sha512.c sha512_armv6.s -DSHA512_USE_ASM block.c int_util.c hashsum.c -DHASH_SHA512_256 $(INCLUDE_PATH) $(CFLAGS)

# Whirlpool
whirlpoolsum:
	$(CC) -std=c99 -o whirlpoolsum whirlpool.c block.c int_util.c hashsum.c -DHASH_WHIRLPOOL $(INCLUDE_PATH) $(CFLAGS)

# BLAKE
blake224sum:
	$(CC) -std=c99 -o blake224sum blake256.c block.c int_util.c hashsum.c -DHASH_BLAKE224 $(INCLUDE_PATH) $(CFLAGS)

blake224sum_armv6:
	$(CC) -std=c99 -o blake224sum blake256.c blake256_armv6.s -DBLAKE256_USE_ASM block.c int_util.c hashsum.c -DHASH_BLAKE224 $(INCLUDE_PATH) $(CFLAGS)

blake256sum:
	$(CC) -std=c99 -o blake256sum blake256.c block.c int_util.c hashsum.c -DHASH_BLAKE256 $(INCLUDE_PATH) $(CFLAGS)

blake256sum_armv6:
	$(CC) -std=c99 -o blake256sum blake256.c blake256_armv6.s -DBLAKE256_USE_ASM block.c int_util.c hashsum.c -DHASH_BLAKE256 $(INCLUDE_PATH) $(CFLAGS)

blake512sum:
	$(CC) -std=c99 -o blake512sum blake512.c block.c int_util.c hashsum.c -DHASH_BLAKE512 $(INCLUDE_PATH) $(CFLAGS)

blake512sum_armv6:
	$(CC) -std=c99 -o blake512sum blake512.c blake512_armv6.s -DBLAKE512_USE_ASM block.c int_util.c hashsum.c -DHASH_BLAKE512 $(INCLUDE_PATH) $(CFLAGS)

blake384sum:
	$(CC) -std=c99 -o blake384sum blake512.c block.c int_util.c hashsum.c -DHASH_BLAKE384 $(INCLUDE_PATH) $(CFLAGS)

blake384sum_armv6:
	$(CC) -std=c99 -o blake384sum blake512.c blake512_armv6.s -DBLAKE512_USE_ASM block.c int_util.c hashsum.c -DHASH_BLAKE384 $(INCLUDE_PATH) $(CFLAGS)

# Groestl
groestl256sum: groestl256.c block.c hashsum.c int_util.c
	$(CC) -std=c99 -o groestl256sum groestl256.c block.c int_util.c hashsum.c -DHASH_GROESTL256 $(INCLUDE_PATH) $(CFLAGS)

groestl224sum: groestl256.c block.c hashsum.c int_util.c
	$(CC) -std=c99 -o groestl224sum groestl256.c block.c int_util.c hashsum.c -DHASH_GROESTL224 $(INCLUDE_PATH) $(CFLAGS)

groestl512sum: groestl512.c block.c hashsum.c int_util.c
	$(CC) -std=c99 -o groestl512sum groestl512.c block.c int_util.c hashsum.c -DHASH_GROESTL512 $(INCLUDE_PATH) $(CFLAGS)

groestl384sum: groestl512.c block.c hashsum.c int_util.c
	$(CC) -std=c99 -o groestl384sum groestl512.c block.c int_util.c hashsum.c -DHASH_GROESTL384 $(INCLUDE_PATH) $(CFLAGS)

# JH	
jh256sum: jh.c block.c hashsum.c int_util.c
	$(CC) -std=c99 -o jh256sum jh.c block.c int_util.c hashsum.c -DHASH_JH256 $(INCLUDE_PATH) $(CFLAGS)

jh224sum: jh.c block.c hashsum.c int_util.c
	$(CC) -std=c99 -o jh224sum jh.c block.c int_util.c hashsum.c -DHASH_JH224 $(INCLUDE_PATH) $(CFLAGS)

jh512sum: jh.c block.c hashsum.c int_util.c
	$(CC) -std=c99 -o jh512sum jh.c block.c int_util.c hashsum.c -DHASH_JH512 $(INCLUDE_PATH) $(CFLAGS)

jh384sum: jh.c block.c hashsum.c int_util.c
	$(CC) -std=c99 -o jh384sum jh.c block.c int_util.c hashsum.c -DHASH_JH384 $(INCLUDE_PATH) $(CFLAGS)
