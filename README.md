# blazeX


# SHORT:
Add nodep list_get_node(int (*compare)(void* data))  : search for a node base on data pointer user fnc comparison
Maintain list of routes for ipc communication struct { pid, struct event* e}
Bufferize received data

# MIDDLE:
signal handling (sigchld, wait4pid)
daemonize (detach from tty via setsid) ( touch a daemon /var/run/<pid> filename)
config file via inotify and via args also with getopt() 
  --process=5
  --threads_pool_size=5
  --verbose
  

  


