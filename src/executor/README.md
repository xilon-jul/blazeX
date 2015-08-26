# Libexec

Libexec is a library that allows user to have a higher level control of processes and job execution while using non blocking IO

## General overview



## Features


## Implementation details

```cl
// executor.h
struct libex_worker {	
	char name[16];
	pid_t pid;
	u_short state;
	u_short state_info; 
	DoubledLinkedList routes;
}

struct libex_job {
	char name[16];
	int gid;
	bool execute(void* data, size_t data_len);
}

struct libex_parser {
	libex_job* parse(ByteBuffer b, int origin_fd, int *nb_job);
};

void libex_thread_io_write_cb(Executor e){
	while(true){
		// While there are bytes to write out
		IO_JOB = libex_dequeue_job_write();
		struct event* e = event_new(&l);
		event_add(
	}	
}

// Job event generic callback
void libex_job_event_cb (int fd, int flags, void* data){
	struct libex_user_data udata = (struct libex_user_data*) data;
	
	int r;
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
				break;
			}
		break;
		case 0:
			// End of file (fd closed)
			break;
		default:
			break;
		}
	} while (r > 0);
	// Try to parse data 
	int nb_job;
	struct libex_job jobs = udata->parser->parse(&buffer, &nb_job);
	if(jobs == NULL)
		return;
	libex_execute_job(jobs[0]);
}

// Defines a new parsable read event, make event pending, and try to make out jobs from stream
libex_register_job_event(Executor e, int fd, struct libex_parser parser){
	struct libex_user_data udata = {
	};
	struct event* e = event_new(int fd, EVENT_READ | EVENT_PERSIST, &libex_job_event_cb, &udata);
	event_add(e);
}

// Indicate libex to write data stream terminated by NULL char to out fd when possible
libex_register_job_io(Executor e, int out_fd, char* data){
		
}

```


```c
	# Define a simple stdin parser that echos the input
	libex_job* f(ByteBuffer b, int origin_fd, int* nb_job){
		libex_queue_io(stdout, stream);
	}
	struct libex_parser stdin_parser = { &f }
	libex_register_job_event(executor, stdin, stdin_parser); 

```
