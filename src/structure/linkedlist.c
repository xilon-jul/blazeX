#include "linkedlist.h"
#include "stdlib.h"
#include "stdio.h"

/**
 * Allocates a new doubled linked list
 * @return a pointer to a DoubledLinkedList structure
 */
DoubledLinkedList list_new(){
	DoubledLinkedList l = calloc(1, sizeof(struct linkedlist));
	l->head = NULL;
	l->tail = NULL;
	l->size = 0;
	return l;
}

/**
 * Add a node to the beginning of the list
 */
Node list_add_node_head(DoubledLinkedList list, Node* n, void* data){
	*n = (Node) calloc(1, sizeof(struct node));
	if(*n == NULL){
		perror("Could not allocate new node");
		return 0;
	}
	(*n)->data = data;
	if(list->head == NULL){
		(*n)->prev = NULL;
		(*n)->next = NULL;
		list->head = n;
	}
	else {
		Node* head = list->head;
		(*n)->next = head;
		(*head)->prev = n;
		list->head = n;
	}
	if(list->size == 0){
		list->tail = list->head;
	}
	list->size++;
	return *n;
}

Node list_add_node_tail(DoubledLinkedList list, Node* n, void* data){
	*n = (Node) calloc(1, sizeof(struct node));
	if(*n == NULL){
		perror("Could not allocate new node");
		return 0;
	}
	(*n)->data = data;
	(*n)->next = NULL;
	(*n)->prev = NULL;
	if(list->size != 0){
		(*list->tail)->next = n;
		(*n)->prev = list->tail;
	    list->tail = n;
	}
    else {
    	list->head = list->tail = n;
    }
    list->size++;
	return *n;
}

void list_search_node(DoubledLinkedList list, DoubledLinkedList out, int (*match_cb)(void*)){
	Node* current = list->head;
	while(current != NULL){
		if(1 == match_cb((*current)->data)){
			Node n;
			list_add_node_tail(out, &n, *current);
		}
		current = (*current)->next;
	}
}

void list_del_node(DoubledLinkedList list, Node *node){
	list_del_node_soft(list, node);
	free(*node);
	*node = NULL;
}

/**
 * Softly removes a node. Does not free the memory taken by the node
 */
void list_del_node_soft(DoubledLinkedList list, Node *node){
	// Make previous node point to next node and next node->prev points to previous node
	Node* after = (*node)->next;
	Node* before = (*node)->prev;
	if(before != NULL){
		(*before)->next = after;
	}
	if(after != NULL){
		(*after)->prev = before;
	}
	if(node == list->tail){
		list->tail = (*(list->tail))->prev;
	}
	if(node == list->head && list->head != list->tail){
		list->head = (*(list->head))->next;
	}
	list->size--;
}

void list_iterate(DoubledLinkedList list, void (*print_cb)(Node)){
	Node *n = list->head;
	while(n != NULL){
		print_cb(*n);
		n = (*n)->next;
	}
}

/**
 * Removes or "pops" the node at the end of list
 * The node's linkage are destroyed, but the node is not freed
 * @return Node pointer or NULL
 */
Node list_pop(DoubledLinkedList list){
	if(list->tail == NULL) return NULL;
	Node pop = *(list->tail);
	list_del_node_soft(list, list->tail);
	return pop;
}

/**
 * Removes the node at the beginning of the list
 * Node is not freed, it is your responsability to free the node
 * @return Node pointer or NULL
 */
Node list_unshift(DoubledLinkedList list){
	if(list->head == NULL) return NULL;
	Node pop = *(list->head);
	list_del_node_soft(list, list->head);
	return pop;
}

void list_free(DoubledLinkedList* list){
    Node *current = (*list)->head;
    Node *tmp;
    while(current != NULL){
    	tmp = (*current)->next;
    	free(*current);
    	*current = NULL;
    	current = tmp;
    }
    (*list)->head = NULL;
    (*list)->tail = NULL;
    free(*list);
    *list = NULL;
}
