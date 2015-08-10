/*
 * buffer.h
 *
 *  Created on: Aug 10, 2015
 *      Author: julien
 */

#ifndef SRC_BUFFER_H_
#define SRC_BUFFER_H_

#include <stddef.h>

typedef struct buffer* pbuffer;

struct buffer {
	unsigned int size;
	unsigned int current_size;
	unsigned int offset;
	char* buffer;
};

pbuffer buffer_allocate(unsigned int size);

void buffer_free(pbuffer b);

/**
 * Copy out the first len bytes from the buffer to out pointer
 * and reduce the size of underlying buffer
 */
void buffer_drain(pbuffer buf, int len, char* out);

int buffer_add_data(pbuffer buf, char* data);

#endif /* SRC_BUFFER_H_ */
