#include <stdlib.h>
#include "executor.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "../lib/linkedlist.h"
#include "utils.h"
#include "buffer.h"


int main() {
	pbuffer b = buffer_allocate(8192);
	buffer_add_data(b, "tesa");
	printf("Buffer = %s and init size = %d, size = %d \n", b->buffer, b->size, b->current_size);

	char out[256];
	buffer_drain(b, 3, out);
	printf("Buffer = %s and init size = %d, size = %d \n", b->buffer, b->size, b->current_size);

	buffer_add_data(b, "titi");
	buffer_add_data(b, "pipi");
	buffer_drain(b, 9, out);

	printf("Buffer = %s and init size = %d, size = %d \n", b->buffer, b->size, b->current_size);

	return 0;
}

