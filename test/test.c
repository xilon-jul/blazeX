#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include "../src/structure/btree.h"
#include "../src/structure/linkedlist.h"
#include "../src/structure/buffer.h"

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
	Node n1 = list_add_node_head(l, "3");
	Node n2 = list_add_node_head(l, "2");

	char node[3][16] = {{0},{0},{0}};
	int j = 0;
	void f(Node n){
		strcpy(node[j++], n->data);
	}
	list_iterate(l, f);
	// head-> 2 -> 3 -> NULL
	assert ( l->size == 2 );
	assert ( l->head == n2 );
	assert ( l->tail == n1 );
	assert( strcmp(node[0], "2") == 0 );
	assert( strcmp(node[1], "3") == 0 );

	// Remove last node, make sure head and tail points to the same node
	list_del_node(l, &n1); // delete 3 would give head->2->NULL
	assert ( l->size == 1 );
	assert ( l->head == n2 );
	assert ( l->tail == n2 );
	assert ( n1 == NULL );

	list_del_node (l, &n2);
	assert ( l->size == 0 );
	assert ( l->head == NULL );
	assert ( l->tail == NULL );
	assert ( n2 == NULL );

	list_free(&l);
	assert ( l == NULL );
}

void test_linkedlist_insert(){
	DoubledLinkedList2 l = list_new_2();
	Node2 ptr;
	list_add_node_tail_2(l, &ptr, "3");
	Node2 ptr2;
	list_add_node_tail_2(l, &ptr2, "4");
	Node2 ptr3;
	list_add_node_tail_2(l, &ptr3, "5");
	assert( l->head == &ptr );
	list_free_2(&l);
	assert ( ptr == NULL );
	assert ( ptr2 == NULL );
	assert ( ptr3 == NULL );
}

void test_linkedlist_stack_usage(){
	DoubledLinkedList l = list_new();
	Node n1 = list_add_node_tail(l, "3");
	assert ( l->head == l->tail );
	Node n2 = list_add_node_tail(l, "2");
	assert ( l->head != l->tail && l->head != NULL);
	assert ( l->head == n1 );
	assert ( l->tail == n2 );
	Node pop = list_pop(l);
	assert ( pop == n2 );
	// head -> 3 -> NULL
	assert ( l->tail == n1 );
	assert ( l->head == l->tail );
	assert ( l->size == 1 );

	// Test unshift head->3->2->NULL
	Node n3 = list_add_node_tail(l, "2");
	Node shift = list_unshift(l);
	assert ( shift == n1 );
	assert ( l->head == n3 );
	assert ( l->tail == l->head );

	list_free(&l);
	assert ( l == NULL );
}

int main() {
	// Tree tests
	test_binary_tree_traversal();
	test_binary_tree_free();
	// Linkedlist test
	test_linkedlist_iterate();
	test_linkedlist_stack_usage();
	test_linkedlist_insert();
	return 0;
}

