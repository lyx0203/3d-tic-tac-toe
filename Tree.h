#ifndef TREE_H
#define TREE_H

#include <iostream>
#include <vector>
#include "Node.h"

class Tree {
	Node* _root;
public:
	/* The game-state for minimax purposes is stored as a 
		one-dimensional array with 64 elements */
	std::vector<int> current_game_state;
	
	/* The win_states are generated every time the program starts, 
		used to determine if someone (anyone) has won the game */
	std::vector<std::vector<int>> win_states;
	
	/* Stores whether or not the player is 'X' */
	bool _player_x;
	
	/* Constructor will take in whether the player is X or O (true and false) 
		and initializes the internal data where to board state is stored */
	Tree(bool x): _player_x(x) {
		current_game_state.resize(64);
		for (int i = 0; i < 64; i++) {
			current_game_state.at(i) = -1;
		}
	}
	
	/* Creates the minimax tree with the specified depth 
		(after 4 it gets ridiculous) */
	void initialize(int depth);
	
	/* Recursive function that builds the minimax tree */
	void build_tree(Node* parent, int lookaheads);
	
	/* Evaluates and assigns minimax to each node starting with the terminals */
	void evaluate_nodes();
	
	/* Recursively computes minimax of a given node 
		*Uses alpha-beta pruning* :D */
	int compute_minimax(Node* to_compute, int alpha, int beta);
	
	/* Returns the value in numeric notation of the square the AI 
		will move to */
	int decide_next_move();
	
	/* Returns true if the game is over due to a player or machine winning the game 
		Note: NOT FOR TIES */
	std::vector<bool> check_win();
	
	/* Finds IN NUMERIC NOTATION the moves that led to the win (useful for UI?) */
	std::vector<int> find_winning_combo(bool who_won);
	
	/* Delete all Nodes (parents, children, brothers, uncles etc. */
	void destroy_tree();
	
	/* Called to have win states in place for easy finding */
	void generate_win_states();
	void generate_horizontal_planes();
	void generate_vertical_planes();
	void generate_plane_diagonals();
	/* '                                                   ' */
	
	
	/* Prints in numeric notations all 76 possible ways to win */
	void print_win_states();
	
	/* Generates all possible moves for X or O from a current game state 
		returns moves as a vector of game states */
	std::vector<std::vector<int>> possible_moves(std::vector<int> game_state, bool max);
	
	/* Evaluates the utility function for X of any given game state 
		(should only used to find the utility of terminal nodes) */
	int util_eval(std::vector<int>& game_state);
	
	/* Destructor cleanup */
	~Tree();
};

#endif
