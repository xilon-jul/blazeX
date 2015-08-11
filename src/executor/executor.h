/*
 * executor.h
 *
 *  Created on: Aug 6, 2015
 *      Author: julien
 */

#ifndef SRC_EXECUTOR_H_
#define SRC_EXECUTOR_H_

#define BUFFER_READ_SIZE 8192

typedef struct executor* Executor;
typedef struct fullduplex_ipc_route* Route;


struct executor {
	struct event_base *eb;
	short running;
	// A list maitaining pid to event
	void (*start)(Executor e);
	void (*shutdown)(Executor e);
	int (*fork)(Executor e);
};

struct fullduplex_ipc_route {
	unsigned int pid;
	struct event* ev;
};

Executor blazex_executor_new();

void blazex_executor_shutdown(Executor e);

void blazex_executor_start(Executor e);

int blazex_executor_fork(Executor e);

void blazex_executor_event_readcb(int fd, short flags, void* data);

#endif /* SRC_EXECUTOR_H_ */
