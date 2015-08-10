/*
 * linkedlist.h
 *
 *  Created on: Aug 7, 2015
 *      Author: julien
 */

#ifndef LIB_LINKEDLIST_H_
#define LIB_LINKEDLIST_H_

typedef struct node* pnode;
typedef struct linkedlist* plist;

struct node {
	pnode prev;
	pnode next;
	void* data;
};

struct linkedlist {
	pnode head;
	pnode tail;
	int size;
};

struct linkedlist* list_new();

pnode list_add_node_head(plist list, void* data);

pnode list_add_node_tail(plist list, void* data);

void list_search_node(plist list, plist out, int (*match_cb)(void*));

int list_del_node(plist list, pnode node);

void list_iterate(plist list, void(*print_cb)(void*));

void list_free(plist list);

#endif /* LIB_LINKEDLIST_H_ */
