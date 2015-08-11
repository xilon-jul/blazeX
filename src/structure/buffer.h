/*
 * buffer.h
 *
 *  Created on: Aug 10, 2015
 *      Author: julien
 */

#ifndef SRC_BUFFER_H_
#define SRC_BUFFER_H_

#include <stddef.h>

typedef struct buffer* ByteBuffer;

struct buffer {
	unsigned int size;
	unsigned int current_size;
	unsigned int offset;
	char* buffer;
};

/**
 * Allocates a new byte buffer of size size + 1 (for the null char)
 */
ByteBuffer buffer_allocate(unsigned int size);

/**
 * Free the mem of the buffer
 */
void buffer_free(ByteBuffer b);

/**
 * Drained "len" bytes from buffer starting at offset 0.
 * Copy out the first len bytes from the buffer to out pointer. Out pointer should be already be allocated
 * Might also shrink the buffer size
 * @return NULL if a mem operation fails, a pointer to the buf structure otherwise
 */
void* buffer_drain(ByteBuffer buf, int len, char* out);

/**
 * Add bytes to the buffer and re-allocate memory if needed
 * @return NULL if a mem operaiton fails, a pointer to the ByteBuffer structure otherwise
 */
void* buffer_add_data(ByteBuffer buf, char* data);

#endif /* SRC_BUFFER_H_ */
