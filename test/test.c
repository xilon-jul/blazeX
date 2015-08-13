#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include "../src/structure/btree.h"
#include "../src/structure/linkedlist.h"
#include "../src/structure/buffer.h"

int main() {
	BinaryTree t = binary_tree_new("a");
	BinaryTreeNode left = binary_tree_add_node(&t->root, "b", BINARY_TREE_NODE_SIDE_LEFT);
	BinaryTreeNode right = binary_tree_add_node(&t->root, "c", BINARY_TREE_NODE_SIDE_RIGHT);

	char node[3][16];
	int j = 0;
	void f(BinaryTreeNode *n){
		if(*n == NULL){
			return;
		}
		node[j++] = (*n)->data;
	}
	//binary_tree_node_free(&(t->root->left));
	//binary_tree_node_free(&(t->root->left));
	binary_tree_iterate_post_order(&(t->root), f);
	assert( strcmp(node[0], "b") == 0 );
	return 0;
}

