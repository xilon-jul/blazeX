/*
 * executor.h
 *
 *  Created on: Aug 6, 2015
 *      Author: julien
 */

#ifndef SRC_EXECUTOR_H_
#define SRC_EXECUTOR_H_

#define BUFFER_READ_SIZE 8192

#define EXECUTOR_MAX_GROUP_SIZE 8
#define EXECUTOR_MAX_GROUP_NAME_LEN 16


typedef struct executor* Executor;
typedef struct event_route* Route;


#include "../structure/buffer.h"
#include "../structure/linkedlist.h"


struct executor {
	struct event_base *eb;
	short running;
    char groups[EXECUTOR_MAX_GROUP_SIZE][EXECUTOR_MAX_GROUP_NAME_LEN];
    DoubledLinkedList routes;
	void (*start)(Executor e);
	void (*shutdown)(Executor e);
	int (*fork)(Executor e);
};

struct event_route {
	char name[16];
    int fd;
	struct event* ev;
    ByteBuffer in;
    ByteBuffer out;
    void* data;
};

Executor blazex_executor_new();

void blazex_executor_shutdown(Executor e);

void blazex_executor_start(Executor e);

int blazex_executor_fork(Executor e);

void blazex_executor_join_group(Executor e, const char* group);

void blazex_executor_quit_group(Executor e, const char* group);

void blazex_executor_event_readcb(int fd, short flags, void* data);

#endif /* SRC_EXECUTOR_H_ */
