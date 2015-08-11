/*
 * linkedlist.h
 *
 *  Created on: Aug 7, 2015
 *      Author: julien
 */

#ifndef LIB_LINKEDLIST_H_
#define LIB_LINKEDLIST_H_

typedef struct node* Node;
typedef struct linkedlist* DoubledLinkedList;

struct node {
	Node prev;
	Node next;
	void* data;
};

struct linkedlist {
	Node head;
	Node tail;
	int size;
};

struct linkedlist* list_new();

Node list_add_node_head(DoubledLinkedList list, void* data);

Node list_add_node_tail(DoubledLinkedList list, void* data);

void list_search_node(DoubledLinkedList list, DoubledLinkedList out, int (*match_cb)(void*));

void list_del_node(DoubledLinkedList list, Node node);

void list_iterate(DoubledLinkedList list, void(*print_cb)(void*));

void list_free(DoubledLinkedList list);

#endif /* LIB_LINKEDLIST_H_ */
