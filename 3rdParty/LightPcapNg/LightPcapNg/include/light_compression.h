// light_compression.h
// Created on: Aug 13, 2019

// Copyright (c) 2019 TMEIC Corporation - Robert Kriener

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#ifndef INCLUDE_LIGHT_COMPRESSION_H_
#define INCLUDE_LIGHT_COMPRESSION_H_

#include <stdint.h>

//This block should include the compression type you want to build for
#if defined(USE_Z_STD)
#include "light_zstd_compression.h"
//Setup some other compression
#elif defined(USE_THIS_COMPRESSION_INSTEAD)
//No compression
#else
#define USE_NULL_COMPRESSION
#include "light_null_compression.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

struct light_file_t;

/// Compression options. Pass to light_pcapng_open_write_with_options when the
/// legacy single int compression level is not enough. num_workers > 0 enables
/// multi-threaded zstd compression (ZSTD_c_nbWorkers); other backends ignore it.
typedef struct light_pcapng_compression_options_t
{
	int compression_level;
	int num_workers;
} light_pcapng_compression_options_t;

//Any compression types to be added need to plug their appropriate code into these functions

//Init anything needed to keep state of your compression or configure your compression here
void light_free_compression_context(_compression_t* context);
_compression_t * light_get_compression_context(int compression_level);

/// Returns a compression context configured from the given options. Passing
/// NULL or options->compression_level == 0 returns NULL. The caller owns the
/// returned context and must release it via light_free_compression_context().
_compression_t * light_get_compression_context_with_options(const light_pcapng_compression_options_t * options);

//Init anything needed to keep state of your decompression or configure your decompression here
void light_free_decompression_context(_decompression_t* context);
_decompression_t * light_get_decompression_context();

//Return true if the file at file_path is a compressed file and should be decompressed
int light_is_compressed_file(const char* file_path);

//Return number of decompressed bytes read from file
size_t light_read_compressed(struct light_file_t *fd, void *buf, size_t count);

//Return number of bytes written to file from the provided buffer - do not return the number of compressed bytes written
size_t light_write_compressed(struct light_file_t *fd, const void *buf, size_t count);

//Called when the file being read/written is to be closed - this is called first!
int light_close_compressed(struct light_file_t *fd);

#ifdef __cplusplus
}
#endif

#endif /* INCLUDE_LIGHT_COMPRESSION_H_ */
