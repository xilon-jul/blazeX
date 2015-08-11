/*
 * utils.h
 *
 *  Created on: Aug 7, 2015
 *      Author: julien
 */

#ifndef SRC_UTILS_H_
#define SRC_UTILS_H_

#define TRUE 1
#define FALSE 0

typedef unsigned short boolean;

#define NELEMS(x)  (sizeof(x) / sizeof((x)[0]))

void long_to_string(const long l, char* string);

#endif /* SRC_UTILS_H_ */
