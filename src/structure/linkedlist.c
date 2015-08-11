#include "linkedlist.h"
#include "stdlib.h"
#include "stdio.h"

/**
 * Allocates a new doubled linked list
 * @return a pointer to a DoubledLinkedList structure
 */
DoubledLinkedList list_new(){
	DoubledLinkedList l = malloc(sizeof(struct linkedlist));
	l->head = NULL;
	l->tail = NULL;
	l->size = 0;
	return l;
}

/**
 * Add a node to the beginning of the list
 */
Node list_add_node_head(DoubledLinkedList list, void* data){
	Node n = (Node) malloc(sizeof(struct node));
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
		Node head = list->head;
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

Node list_add_node_tail(DoubledLinkedList list, void* data){
	Node n = (Node) malloc(sizeof(struct node));
	if(n == NULL){
		perror("Could not allocate new node");
		return 0;
	}
	n->data = data;
	n->next = NULL;
	if(list->size != 0){
		list->tail->next = n;
		n->prev = list->tail;
	        list->tail = n;
	}
        else {
            list->head = list->tail = n;
        }
        list->size++;
	return n;
}

void list_search_node(DoubledLinkedList list, DoubledLinkedList out, int (*match_cb)(void*)){
    if(out == NULL){
        return;
    }
    Node n = list->head;
    while(n != NULL){
        if(1 == match_cb(n->data)){
            list_add_node_tail(out, n->data);
        }
        n = n->next;
    }
    return;
}

void list_del_node(DoubledLinkedList list, Node node){
	if(list->size == 1){
		list->head = list->tail = NULL;
	}
	else {
		Node after = node->next;
		Node before = node->prev;
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
}

void list_iterate(DoubledLinkedList list, void (*print_cb)(void*)){
	Node n = list->head;
	while(n != NULL){
		print_cb(n->data);
		n = n->next;
	}
}

void list_free(DoubledLinkedList list){
    Node next = list->head;
    Node n;
    while(next != NULL){
        n = next->next;
        free(next);
        next = NULL;
        next = n;
    }
    list->head = NULL;
    list->tail = NULL;
    free(list);
    list = NULL;
}
