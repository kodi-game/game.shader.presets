/* Copyright  (C) 2010-2020 The RetroArch team
 *
 * ---------------------------------------------------------------------------------------
 * The following license statement only applies to this file (file_stream.h).
 * ---------------------------------------------------------------------------------------
 *
 * Permission is hereby granted, free of charge,
 * to any person obtaining a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef __LIBRETRO_SDK_FILE_STREAM_H
#define __LIBRETRO_SDK_FILE_STREAM_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

#include <sys/types.h>

#include <retro_common_api.h>
#include <retro_inline.h>
#include <boolean.h>

#include <stdarg.h>

#if defined _WIN32 || defined _WIN64 || defined __CYGWIN__
#define ATTR_HIDDEN
#else
#if __GNUC__ >= 4
#define ATTR_HIDDEN __attribute__((visibility("hidden")))
#else
#define ATTR_HIDDEN
#endif
#endif

/** @defgroup file_stream File Streams
 *
 * All functions in this header will use the VFS interface set in \ref filestream_vfs_init if possible,
 * or else they will fall back to built-in equivalents.
 *
 * @note These functions are modeled after those in the C standard library
 * (and may even use them internally),
 * but identical behavior is not guaranteed.
 *
 * @{
 */

RETRO_BEGIN_DECLS

/**
 * Opaque handle to a file stream.
 * @warning This is not interchangeable with \c FILE* or \c retro_vfs_file_handle.
 */
struct ATTR_HIDDEN RFILE;
typedef struct RFILE RFILE;

/**
 * Returns the size of the given file, in bytes.
 *
 * @param stream The open file to query.
 * @return The size of \c stream in bytes,
 * or -1 if there was an error.
 * @see retro_vfs_size_t
 */
int64_t filestream_get_size(RFILE *stream);

/**
 * Opens a file for reading or writing.
 *
 * @param path Path to the file to open.
 * Should be in the format described in \ref GET_VFS_INTERFACE.
 *
 * @return The opened file, or \c NULL if there was an error.
 * Must be cleaned up with \c filestream_close when no longer needed.
 * @see retro_vfs_open_t
 */
RFILE* filestream_open(const char *path);

/**
 * Reads data from the given file into a buffer.
 * If the read is successful,
 * the file's stream position will advance by the number of bytes read.
 *
 * @param stream The file to read from.
 * @param data The buffer in which to store the read data.
 * @param len The size of \c data, in bytes.
 * @return The number of bytes read,
 * or -1 if there was an error.
 * May be less than \c len, but never more.
 * @see retro_vfs_read_t
 */
int64_t filestream_read(RFILE *stream, void *data, int64_t len);

/**
 * Returns the current position of the given file in bytes.
 *
 * @param stream The file to return the stream position for.
 * @return The current stream position in bytes relative to the beginning,
 * or -1 if there was an error.
 * @see retro_vfs_tell_t
 */
int64_t filestream_tell(RFILE *stream);

/**
 * Closes the given file.
 *
 * @param stream The file to close.
 * This should have been created with \c filestream_open.
 * Behavior is undefined if \c NULL.
 * @return 0 if the file was closed successfully,
 * or -1 if there was an error.
 * @post \c stream is no longer valid and should not be used,
 * even if this function fails.
 * @see retro_vfs_close_t
 */
int filestream_close(RFILE *stream);

/**
 * Opens a file, reads its contents into a newly-allocated buffer,
 * then closes it.
 *
 * @param path[in] Path to the file to read.
 * Should be in the format described in \ref GET_VFS_INTERFACE.
 * @param buf[out] A pointer to the address of the newly-allocated buffer.
 * The buffer will contain the entirety of the file at \c path.
 * Will be allocated with \c malloc and must be freed with \c free.
 * @param len[out] Pointer to the size of the buffer in bytes.
 * May be \c NULL, in which case the length is not written.
 * Value is unspecified if this function fails.
 * @return 1 if the file was read successfully,
 * 0 if there was an error.
 * @see filestream_write_file
 */
int64_t filestream_read_file(const char *path, void **buf, int64_t *len);

/**
 * Reads a single character from the given file.
 *
 * @param stream The file to read from.
 * @return The character read, or -1 upon reaching the end of the file.
 */
int filestream_getc(RFILE *stream);

/**
 * Determines if there's any more data left to read from this file.
 *
 * @param stream The file to check the position of.
 * @return -1 if this stream has reached the end of the file,
 * 0 if not.
 */
int filestream_eof(RFILE *stream);

/**
 * Reads a line from the given file into a newly-allocated buffer.
 *
 * @param stream The file to read from.
 * @return Pointer to the line read from \c stream,
 * or \c NULL if there was an error.
 * Must be freed with \c free when no longer needed.
 */
char* filestream_getline(RFILE *stream);

RETRO_END_DECLS

/** @} */

#endif
