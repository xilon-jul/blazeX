#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include "../src/structure/btree.h"
#include "../src/structure/linkedlist.h"
#include "../src/structure/buffer.h"l

BinaryTree create_btree(){
	BinaryTree t = binary_tree_new("a");
	binary_tree_add_node(&t->root, "b", BINARY_TREE_NODE_SIDE_LEFT);
	binary_tree_add_node(&t->root, "c", BINARY_TREE_NODE_SIDE_RIGHT);
	return t;
}

void test_binary_tree_traversal(){
	BinaryTree t = create_btree();
	char node[3][16];
	int j = 0;
	void f(BinaryTreeNode *n){
		if(*n == NULL){
			return;
		}
		strcpy(node[j++], (*n)->data);
	}
	binary_tree_iterate_post_order(&(t->root), f);
	assert( strcmp(node[0], "b") == 0 );
	assert( strcmp(node[1], "c") == 0 );
	assert( strcmp(node[2], "a") == 0 );

	j = 0;
	binary_tree_iterate_pre_order(&(t->root), f);
	assert( strcmp(node[0], "a") == 0 );
	assert( strcmp(node[1], "b") == 0 );
	assert( strcmp(node[2], "c") == 0 );
}

void test_binary_tree_free(){
	BinaryTree t = create_btree();
	char node[3][16] = {{0},{0},{0}};
	int j = 0;
	void f(BinaryTreeNode *n){
		if(*n == NULL){
			return;
		}
		strcpy(node[j++], (*n)->data);
	}
	binary_tree_node_free(&(t->root->left));
	assert( t->root->left == NULL );

	binary_tree_iterate_post_order(&(t->root), f);
	assert( strcmp(node[0], "c") == 0 );
	assert( strcmp(node[1], "a") == 0 );
	assert( node[2][0] == '\0' );

	binary_tree_free(&t);
	assert(t == NULL);
}

// Linked list test

void test_linkedlist_iterate(){
	DoubledLinkedList l = list_new();
	Node n1, n2;
	list_add_node_head(l, &n1, "3");
	list_add_node_head(l, &n2, "2");

	char node[3][16] = {{0},{0},{0}};
	int j = 0;
	void f(Node n){
		strcpy(node[j++], n->data);
	}
	list_iterate(l, f);
	// head-> 2 -> 3 -> NULL
	assert ( l->size == 2 );
	assert ( l->head == &n2 );
	assert ( l->tail == &n1 );
	assert( strcmp(node[0], "2") == 0 );
	assert( strcmp(node[1], "3") == 0 );

	// Remove last node, make sure head and tail points to the same node
	list_del_node(l, &n1); // delete 3 would give head->2->NULL
	assert ( l->size == 1 );
	assert ( l->head == &n2 );
	assert ( l->tail == &n2 );
	assert ( n1 == NULL );

	list_del_node (l, &n2);
	assert ( l->size == 0 );
	assert ( l->head == NULL );
	assert ( l->tail == NULL );
	assert ( n2 == NULL );

	list_free(&l);
	assert ( l == NULL );
	assert ( n1 == NULL );
	assert ( n2 == NULL );
}

void test_linkedlist_insert(){
	DoubledLinkedList l = list_new();
	Node ptr;
	list_add_node_tail(l, &ptr, "3");
	Node ptr2;
	list_add_node_tail(l, &ptr2, "4");
	Node ptr3;
	list_add_node_tail(l, &ptr3, "5");
	assert( l->head == &ptr );
	list_free(&l);
	assert ( ptr == NULL );
	assert ( ptr2 == NULL );
	assert ( ptr3 == NULL );
}

void test_linkedlist_stack_usage(){
	void f(Node n){
		printf("%s\n", n->data);
	}
	DoubledLinkedList l = list_new();
	Node n1, n2, n3;
	list_add_node_tail(l, &n1, "3");
	assert ( l->head == l->tail );
	list_add_node_tail(l, &n2, "2");
	assert ( l->head != l->tail && l->head != NULL);
	assert ( l->head == &n1 );
	assert ( l->tail == &n2 );
	list_iterate(l, f);
	printf("----\n");
	Node pop = list_pop(l);
	assert ( pop == n2 );
	// head -> 3 -> NULL
	assert ( l->tail == &n1 );
	assert ( l->head == l->tail );
	assert ( l->size == 1 );
	list_iterate(l, f);
	printf("----\n");
	// Test unshift head->3->2->NULL
	list_add_node_tail(l, &n3, "2");
	list_iterate(l, f);
	printf("----\n");

	Node shift = list_unshift(l);
	assert ( shift == n1 );
	assert ( l->head == &n3 );
	assert ( l->tail == l->head );
	list_free(&l);
	// Do not forget to free memory for shifted/popped nodes
	free(shift);
	free(pop);
	assert ( l == NULL );
}

void test_linkedlist_free(){
	DoubledLinkedList l = list_new();
	Node n1, n2, n3;
	list_add_node_tail(l, &n1, "1");
	list_add_node_tail(l, &n2, "2");
	list_add_node_tail(l, &n3, "3");
	assert ( n1 == (*l->head) );
	assert ( n3 == (*l->tail) );
	// Node is unshift, we get a pointer the the node but chain links were destroyed
	list_unshift(l);
	assert ( n2 == (*l->head) );
	assert ( n3 == (*l->tail) );
}

void test_linkedlist_search(){
	DoubledLinkedList l = list_new();
	Node n1, n2, n3;
	list_add_node_tail(l, &n1, "1");
	list_add_node_tail(l, &n2, "2");
	list_add_node_tail(l, &n3, "3");
	// Copy list
	DoubledLinkedList out = list_new();
	void f(Node n){
		// node->data is pointer to linked list l
		Node ref = (Node) n->data;
		if(ref == NULL)
			printf("Copy is empty");
		else
			printf("Copy: %s", ref->data );
	}
	int match(void* data){
		return strcmp(data, "3") == 0;
	}
	list_search_node(l, out, match);
	list_iterate(out, f);
	list_del_node(l, &n3);
	list_iterate(out, f);
	assert ( out->size == 1 );
	//assert ( (*(out->head)) == n3 );
	list_free(&l);
}

int main() {
	// Tree tests
	test_binary_tree_traversal();
	test_binary_tree_free();

	// Linkedlist test
	test_linkedlist_iterate();
	test_linkedlist_stack_usage();
	test_linkedlist_insert();
	test_linkedlist_free();
	test_linkedlist_search();
	return 0;
}

