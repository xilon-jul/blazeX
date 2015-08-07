#include <stdlib.h>
#include "executor.h"
#include <stdio.h>
#include <unistd.h>
#include "utils.h";
#include "../lib/linkedlist.h"


int main() {
//	Executor e = blazex_executor_new();
//	printf("Executor %d", e->running);
//	e->fork(e);
//	printf("Executor %d", e->running);
//	e->start(e);
//	exit(0);
	plist l = list_new();
	pnode a = list_add_node_head(l, "a");
	pnode b = list_add_node_head(l, "b");
	pnode c = list_add_node_tail(l, "c");
	list_add_node_tail(l, "z");
	list_add_node_head(l, "titi");
	//list_del_node(l, a);
	list_del_node(l, c);
	void f(void* data){
		printf("%s\n", (char*)data);
	}
	char s[12];
	long_to_string(12, s);
printf("long to string : %s", s);
	list_iterate(l, f);
}
