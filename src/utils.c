#include "stdio.h"
#include "sys/types.h"

void long_to_string(const long l, char* string){
	sprintf(string, "%ld", l);
}
