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


int main() {
	test_binary_tree_traversal();
	test_binary_tree_free();
	return 0;
}

