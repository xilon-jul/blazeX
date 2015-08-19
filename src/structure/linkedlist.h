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

/**
 * Allocates a new doubled linked list
 */
DoubledLinkedList list_new();

/**
 * Add a new node with the given data to the beginning of the list
 * Complexity: O(1)
 * @return a pointer to the node added
 */
Node list_add_node_head(DoubledLinkedList list, void* data);

/**
 * Add a new node with the given data to the end of the list
 * Complexity: O(1)
 * @return a pointer to the node added
 */
Node list_add_node_tail(DoubledLinkedList list, void* data);

/**
 * Search for nodes matching the filter callback function.
 * All matches are added to the destination list, note that it should be already allocated
 * Complexity: O(n)
 * @return void
 */
void list_search_node(DoubledLinkedList list, DoubledLinkedList dest, int (*match_cb)(void*));

/**
 * Free a node's memory and re-arrange the linked list.
 * Complexity: O(1)
 * @return void
 */
void list_del_node(DoubledLinkedList list, Node *node);

/**
 * Iterates over a doubled linked list starting from the head
 * Invokes the print_cb callback with the node's data as argument
 * @return void
 */
void list_iterate(DoubledLinkedList list, void(*print_cb)(Node));

/**
 * Free a list structure and all of its nodes
 */
void list_free(DoubledLinkedList list);

#endif /* LIB_LINKEDLIST_H_ */
