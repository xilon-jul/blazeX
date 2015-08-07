#include "linkedlist.h"
#include "stdlib.h"
#include "stdio.h"

struct linkedlist* list_new(){
	plist l = malloc(sizeof(struct linkedlist));
	l->head = NULL;
	l->tail = NULL;
	l->size = 0;
	return l;
}

pnode list_add_node_head(plist list, void* data){
	pnode n = (pnode) malloc(sizeof(struct node));
	if(n == NULL){
		perror("Could not allocate new node");
		return 0;
	}
	n->data = data;
	if(list->head == NULL){
		n->prev = NULL;
		n->next = NULL;
		list->head = n;
	}
	else {
		pnode head = list->head;
		n->next = head;
		head->prev = n;
		list->head = n;
	}
	if(list->size == 0){
		list->tail = list->head;
	}
	list->size++;
	return n;
}

pnode list_add_node_tail(plist list, void* data){
	pnode n = (pnode) malloc(sizeof(struct node));
	if(n == NULL){
		perror("Could not allocate new node");
		return 0;
	}
	n->data = data;
	n->next = NULL;
	if(list->size != 0){
		list->tail->next = n;
		n->prev = list->tail;
	}
	list->tail = n;
	return n;
}


int list_del_node(plist list, pnode node){
	if(list->size == 1){
		list->head = list->tail = NULL;
	}
	else {
		pnode after = node->next;
		pnode before = node->prev;
		if(before != NULL){
			before->next = after;
		}
		if(after != NULL){
			after->prev = before;
		}
	}
	free(node);
	node = NULL;
	list->size--;
	return 0;
}

void list_iterate(plist list, void (*print_cb)(void*)){
	pnode n = list->head;
	while(n != NULL){
		print_cb(n->data);
		n = n->next;
	}
}
