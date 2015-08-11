#include "buffer.h"
#include "stdlib.h"
#include <string.h>
#include <sys/types.h>

ByteBuffer buffer_allocate(unsigned int size){
	// Allocate a new buffer
	ByteBuffer b = (ByteBuffer) malloc(sizeof(ByteBuffer));
	if(b == NULL){
		return NULL;
	}
	// initial buffer size
	b->size = size;
	b->current_size = size;
	// current offet in buffer
	b->offset = 0;
	b->buffer = (char*) malloc(sizeof(char) * size + 1);
	if(b->buffer == NULL){
		free(b);
		b = NULL;
		return NULL;
	}
	return b;
}

void buffer_free(ByteBuffer b){
	free(b->buffer);
	b->buffer = NULL;
	free(b);
	b = NULL;
}

void* buffer_drain(ByteBuffer buf, int len, char* out){
	strncpy(out, buf->buffer, len);
	*(out + len) = '\0';
	// Over lapping move of the memory bytes
	// printf("Copy from %d with size %d",len, buf->current_size - len);
	memset(buf->buffer, 0, len);
	memmove(buf->buffer, buf->buffer + len, buf->current_size - len);
	// Next write be old offset minus number of bytes we have removed
	buf->offset -= len;
	// Shrink the size if ok
	// Reduce to fit initial chunk size
	int nb_chunk = len / buf->size + 1;
	int nsize = buf->current_size - ((nb_chunk-1) * buf->size);
	if(nb_chunk <= 1 || nsize == 0){
			return buf;
	}
	if(NULL == realloc(buf->buffer, sizeof(char) * nsize + 1)){
		perror("realloc() failed");
		return NULL;
	}
	return buf;
}

void* buffer_add_data(ByteBuffer buf, char* data){
	unsigned int dlen = strlen(data);
	if(buf->offset + 1 + dlen > buf->size){
		// What is the good factor to hold new data ?
		unsigned int s = dlen % buf->size == 0 ? dlen / buf->size : dlen / buf->size + 1;
		printf("Expand buffer with old size = %d new size %d", buf->size, buf->size * s);
		if(NULL == realloc(buf->buffer, sizeof(char) * buf->size * s)){
			return NULL;
		}
		buf->current_size = buf->size * s;
	}
	strcpy(buf->buffer + buf->offset, data);
	buf->offset += strlen(data);
	return buf;
}
