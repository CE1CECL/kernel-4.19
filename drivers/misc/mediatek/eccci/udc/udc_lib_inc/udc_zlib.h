/* SPDX-License-Identifier: GPL-2.0 */


#ifndef ZLIB_H
#define ZLIB_H

#ifdef __cplusplus
extern "C" {
#endif

#define ZLIB_VERSION "1.2.11"
#define ZLIB_VERNUM 0x12b0
#define ZLIB_VER_MAJOR 1
#define ZLIB_VER_MINOR 2
#define ZLIB_VER_REVISION 11
#define ZLIB_VER_SUBREVISION 0

struct internal_state;

struct z_stream_s {
	const unsigned char *next_in;     /* next input byte */
	unsigned int avail_in;  /* number of bytes available at next_in */
	unsigned long total_in;  /* total number of input bytes read so far */

	unsigned char *next_out; /* next output byte will go here */
	unsigned int avail_out; /* remaining free space at next_out */
	unsigned long total_out; /* total number of bytes output so far */

	const char *msg;  /* last error message, NULL if no error */
	struct internal_state *state; /* not visible by applications */

	/* used to allocate the internal state */
	void *(*zalloc)(void *opaque, unsigned int items, unsigned int size);

	/* used to free the internal state */
	void (*zfree)(void *opaque, void *address);

	void *opaque;  /* private data object passed to zalloc and zfree */

	/* best guess about the data type: binary or text for deflate,
	 * or the decoding state for inflate
	 */
	int data_type;
	/* Adler-32 or CRC-32 value of the uncompressed data */
	unsigned long adler;
	unsigned long reserved; /* reserved for future use */
};

enum udc_query_id_e {
	UDC_QUERY_WORKSPACE_SIZE = 1,
	UDC_QUERY_SUCCESS = 0,
	UDC_QUERY_NOT_SUPPORT = -1
};

	/* constants */

#define Z_NO_FLUSH      0
#define Z_PARTIAL_FLUSH 1
#define Z_SYNC_FLUSH    2
#define Z_FULL_FLUSH    3
#define Z_FINISH        4
#define Z_BLOCK         5
#define Z_TREES         6
/* Allowed flush values; see deflate() and inflate() below for details */

#define Z_OK            0
#define Z_STREAM_END    1
#define Z_NEED_DICT     2
#define Z_ERRNO        (-1)
#define Z_STREAM_ERROR (-2)
#define Z_DATA_ERROR   (-3)
#define Z_MEM_ERROR    (-4)
#define Z_BUF_ERROR    (-5)
#define Z_VERSION_ERROR (-6)

#define Z_NO_COMPRESSION         0
#define Z_BEST_SPEED             1
#define Z_BEST_COMPRESSION       9
#define Z_DEFAULT_COMPRESSION  (-1)
/* compression levels */

#define Z_FILTERED            1
#define Z_HUFFMAN_ONLY        2
#define Z_RLE                 3
#define Z_FIXED               4
#define Z_DEFAULT_STRATEGY    0
/* compression strategy; see deflateInit2() below for details */

#define Z_BINARY   0
#define Z_TEXT     1
#define Z_ASCII    Z_TEXT   /* for compatibility with 1.2.2 and earlier */
#define Z_UNKNOWN  2
/* Possible values of the data_type field for deflate() */

#define Z_DEFLATED   8
/* The deflate compression method (the only one supported in this version) */

#define Z_NULL  0  /* for initializing zalloc, zfree, opaque */

#ifdef __cplusplus
}
#endif

#endif /* ZLIB_H */
