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

ByteBuffer buffer_allocate(unsigned int size);


void buffer_free(ByteBuffer b);

/**
 * Copy out the first len bytes from the buffer to out pointer
 * and reduce the size of underlying buffer
 */
void* buffer_drain(ByteBuffer buf, int len, char* out);

void* buffer_add_data(ByteBuffer buf, char* data);

#endif /* SRC_BUFFER_H_ */
