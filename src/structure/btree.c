#include "btree.h"
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>

/**
 * Allocates a new binary tree
 * @return pointer to BinaryTree structure
 */
BinaryTree binary_tree_new(void* data){
	BinaryTree tree = (BinaryTree) malloc(sizeof(struct binary_tree));
	if(tree == NULL){
		perror("malloc() failed");
		return NULL;
	}
	tree->root = binary_tree_node_new(data);
	return tree;
}

/**
 * Free all the memory taken by a binary tree
 * @return void
 */
void binary_tree_free(BinaryTree *tree){
	binary_tree_node_free(&((*tree)->root));
	free(*tree);
	*tree = NULL;
}

/**
 * Allocates a new binary tree node with data as node's value
 *
 */
BinaryTreeNode binary_tree_node_new(void* data){
	BinaryTreeNode n = malloc(sizeof(struct binary_tree_node));
	if(n == NULL){
		perror("Failed to allocate new binary tree node");
		return NULL;
	}
	n->data = data;
	n->left = n->right = n->parent = NULL;
	return n;
}

/**
 * Frees the memory taken by a binary tree node. When invoked on a node it will
 * free the memory of this node's subtree to avoid memory leaks
 */
void binary_tree_node_free(BinaryTreeNode *node){
	void free_cb(BinaryTreeNode* n){
		if((*n)->left != NULL){
			(*n)->left = NULL;
		}
		if((*n)->right != NULL){
			(*n)->right = NULL;
		}
		free(*n);
		*n = NULL;
	}
	free_cb(node);
	//binary_tree_iterate_post_order(node, free_cb);
}

/**
 * Allocates a new node and fill it with data. Attach node to src_node as its right or left node depending on constant used.
 * @return a pointer to the new node. New node becomes a leaf
 */
BinaryTreeNode binary_tree_add_node(BinaryTreeNode *src_node, void* data, unsigned short side){
	if(*src_node == NULL){
		perror("Bad src_node given");
		return NULL;
	}
	BinaryTreeNode n = binary_tree_node_new(data);
	if(n == NULL){
		perror("Failed to add binary tree node");
		return NULL;
	}
	n->parent = *src_node;
	switch(side){
		case BINARY_TREE_NODE_SIDE_LEFT:
			(*src_node)->left = n;
			break;
		case BINARY_TREE_NODE_SIDE_RIGHT:
			(*src_node)->right = n;
			break;
		default:
			(*src_node)->left = n;
			break;
	}
	return n;
}

/**
 * Iterates over a binary tree pre-order
 * For each node, applies the user callback with the Node being iterated as first argument
 */
void binary_tree_iterate_pre_order(BinaryTreeNode *node, binary_tree_iterate_cb cb){
	if(*node == NULL){
		return;
	}
	cb(node);
	binary_tree_iterate_pre_order(&(*node)->left, cb);
	binary_tree_iterate_pre_order(&(*node)->right, cb);
}

/**
 * Iterates over a binary tree in post-order mode
 * For each node, applies the user callback with the Node being iterated as first argument
 */
void binary_tree_iterate_post_order(BinaryTreeNode *node, binary_tree_iterate_cb cb){
	if(*node == NULL){
		return;
	}
	binary_tree_iterate_post_order(&((*node)->left), cb);
	binary_tree_iterate_post_order(&((*node)->right), cb);
	cb(node);
}
