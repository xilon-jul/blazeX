#include <stdlib.h>
#include "executor.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "utils.h"
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
        int match(void* data){
            if(strcmp((char*)data, "b") == 0){
                return 1;
            }
            return 0;
        }
	char s[12];
        plist out = list_new();
        list_search_node(l, out, match);
	//list_iterate(l, f);
	list_iterate(out, f);
        list_free(out);
        plist test = NULL;
        printf("List is null ? %d", test == NULL);
        return 0;
}
