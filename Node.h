#ifndef NODE_H
#define NODE_H

#include <iostream>

struct Node {
	/* Stores pointers to all nodes created */
	static std::vector<Node*> bunch_nodes;
	
	/* Each node has a parent (or nullptr if root) */
	Node* parent;
	
	/* Eventually all nodes will have assigned minimax */
	int value;
	
	/* Each nodes has a game board corresponding to its state (used to find 
		children and eventually evaluate all nodes */
	std::vector<int> game_state;
	
	/* and 0 or more children */
	std::vector<Node*> children;
	
	/* All nodes are maximizing or minimizing */
	bool max;
	
	/* A node has children (false) or does not have children (true) */
	bool terminal_node;
	
	Node(Node* parent, std::vector<int> game_state, bool max, bool terminal_node):
		parent(parent), game_state(game_state), max(max),
		terminal_node(terminal_node) {
			add();
		}
	
	void add() {
		bunch_nodes.push_back(this);
	} 
};

#endif
