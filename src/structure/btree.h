/*
 * btree.h
 *
 *  Created on: Aug 12, 2015
 *      Author: julien
 */

#ifndef SRC_STRUCTURE_BTREE_H_
#define SRC_STRUCTURE_BTREE_H_


typedef struct binary_tree* BinaryTree;
typedef struct binary_tree_node* BinaryTreeNode;
typedef void (*binary_tree_iterate_cb)(BinaryTreeNode*);

#define BINARY_TREE_NODE_SIDE_LEFT 0
#define BINARY_TREE_NODE_SIDE_RIGHT 1




struct binary_tree {
	BinaryTreeNode root;
};

struct binary_tree_node {
	BinaryTreeNode left;
	BinaryTreeNode right;
	BinaryTreeNode parent;
	void* data;
};

/**
 * Allocates a new binary tree. Creates the root node with data as value.
 * @param data the root node value
 * @return pointer to BinaryTree structure
 */
BinaryTree binary_tree_new(void* data);

/**
 * Free all the memory taken by a binary tree
 * @return void
 */
void binary_tree_free(BinaryTree *tree);

/**
 * Allocates a new binary tree node with data as node's value
 *
 */
BinaryTreeNode binary_tree_node_new(void* data);

/**
 * Frees the memory taken by a binary tree node. When invoked on a node it will
 * free the memory of this node's subtree to avoid memory leaks
 */
void binary_tree_node_free(BinaryTreeNode* node);

/**
 * Allocates a new node and fill it with data. Attach node to src_node as its right or left node depending on constant used.
 * @return a pointer to the new node. New node becomes a leaf
 */
BinaryTreeNode binary_tree_add_node(BinaryTreeNode *src_node, void* data, unsigned short side);

/**
 * Iterates over a binary tree pre-order
 * For each node, applies the user callback with the Node being iterated as first argument
 */
void binary_tree_iterate_pre_order(BinaryTreeNode *node, binary_tree_iterate_cb cb);

/**
 * Iterates over a binary tree in post-order mode
 * For each node, applies the user callback with the Node being iterated as first argument
 */
void binary_tree_iterate_post_order(BinaryTreeNode *node, binary_tree_iterate_cb cb);

#endif /* SRC_STRUCTURE_BTREE_H_ */
