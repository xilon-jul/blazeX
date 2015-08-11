/*
 * executor.c
 *
 *  Created on: Aug 6, 2015
 *      Author: julien
 */
#include "../util/utils.h"
#include "executor.h"
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <event.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>


// Event part of executor
void blazex_executor_event_readcb(int fd, short flags, void* data){
	int r;
        char buffer[8192];
	do {
		r = read(fd, buffer, EXECUTOR_BUFFER_SIZE);
		switch(r){
		case -1:
			switch(errno){
			#ifndef EWOULDBLOCK
				case EAGAIN:
			#endif
			case EWOULDBLOCK:
				// Read has no more data
				printf("Read would block");
				fflush(stdout);
				break;
			}
		break;
		case 0:
			printf("End of file");
			fflush(stdout);
			break;
		default:
			printf("Read %d bytes and buffer is %s", r, buffer);
			fflush(stdout);
			write(fd, "test", 4);
			break;
		}
	} while (r > 0);
}


Executor blazex_executor_new(){
	Executor e = (Executor) malloc(sizeof(struct executor));
	e->running = 0;
	e->eb = event_base_new();
	if(e->eb == NULL){
		fprintf(stderr, "Failed to create executor()");
		exit(1);
	}
	// Assign function pointers
	e->shutdown = &blazex_executor_shutdown;
	e->start = &blazex_executor_start;
	e->fork = &blazex_executor_fork;
	if(e == NULL){
		perror("Failed to allocate executor");
		return NULL;
	}
	return e;
}

Route blazex_executor_route_new(const char* name, int fd, struct event *ev){
	Route r = malloc(sizeof(Route));
	if(r == NULL){
		perror("malloc() failed");
		return NULL;
	}
	strncpy(r->name , name, 16);
	r->fd = fd;
	r->ev = ev;
	r->in = buffer_allocate(EXECUTOR_BUFFER_SIZE);
	r->out = buffer_allocate(EXECUTOR_BUFFER_SIZE);
	if(r->in == NULL || r->out == NULL){
		perror("malloc() failed");
		return NULL;
	}
	return r;
}

void blazex_executor_route_free(Route r){
    buffer_free(r->in);
    buffer_free(r->out);
    r->in = r->out = NULL;
    // FIXME: Check if the underlying file descriptor is closed
    event_free(r->ev);
    r->ev = NULL;
    free(r);
    r = NULL;
}

void blazex_executor_join_group(Executor e, const char* group){
	for(int i=0; i< NELEMS(e->groups); i++){
		if(e->groups[i][0] == '\0'){
			strncpy(e->groups[i], group, EXECUTOR_MAX_GROUP_NAME_LEN);
			break;
		}
	}
}

void blazex_executor_quit_group(Executor e,  const char* group){
	for(int i=0; i< NELEMS(e->groups); i++){
		if(strcmp(e->groups[i], group) == 0){
			memset(e->groups[i], 0x0, EXECUTOR_MAX_GROUP_NAME_LEN);
			return;
		}
	}
}

boolean blazex_executor_has_group(Executor e, const char* group){
	for(int i=0; i< NELEMS(e->groups); i++){
			if(strcmp(e->groups[i], group) == 0){
				return TRUE;
			}
	}
	return FALSE;
}

void blazex_executor_shutdown(Executor e){
	e->running = 0;
}

void blazex_executor_start(Executor e){
	e->running = 1;
	while(e->running){
		event_base_loop(e->eb, EVLOOP_ONCE);
	}
}

int blazex_executor_fork(Executor e){
	int pair[2];
	if(-1 == socketpair(AF_UNIX, SOCK_STREAM, 0, pair)){
		perror("Failed to create socket_pair()");
		exit(0);
	}
	fcntl(pair[0], O_NONBLOCK);
	fcntl(pair[1], O_NONBLOCK);
	pid_t pid = fork();
	char hkey[5];
	switch(pid){
	case -1:
		perror("fork() failed");
		exit(1);
	case 0:
		// Children process
		event_reinit(e->eb);
		close(pair[0]);
		e->start(e);
		break;
	default:
		break;
	}
	return pid;
}
