#ifndef LOGIC_H
#define LOGIC_H

#include <vector>
#include "Node.h"
#include "Tree.h"

#include <vector>
#include "Node.h"
#include "Tree.h"

/* This is where all the AI logistic functions are stored */

int spots_filled = 0;
std::vector<std::vector<int>> columns;
std::vector<Node*> Node::bunch_nodes;

/* Returns true if the inputted number is in a row specified by its
	upper and lower bound */
bool row_value(int num, int lower_bound, int upper_bound) {;
	for (int i = 0; i < 4; i++) {
		if (num >= lower_bound && num <= upper_bound) {
			return true;
		}
		lower_bound+=16;
		upper_bound+=16;
	}
	return false;
}

/* Returns true if the inputted number is in a column specified by its
	upper and lower bound */
bool column_value(int num, int lower_bound, int upper_bound) {
	for (int i = 0; i < 4; i++) {
		if (num >= lower_bound && num <= upper_bound) {
			return true;
		}
		lower_bound+=4;
		upper_bound+=4;
	}
}

/* Generates an internal board used to convert between vector
	and numeric notation */
void generate_helper_board() {
	int num1 = 1, num2 = 5, num3 = 9, num4 = 13;
	for (int j = 0; j < 4; j++) {
		for (int i = 0; i < 4; i++) {
			std::vector<int> new_vector;
			new_vector.resize(4);
			new_vector.at(0) = num1 + i + j * 16;
			new_vector.at(1) = num2 + i + j * 16;
			new_vector.at(2) = num3 + i + j * 16;
			new_vector.at(3) = num4 + i + j * 16;
			columns.push_back(new_vector);
		}
	}
}

/* Finds in vector notation the numerical position of interest 
	ex. converts 16 to (0, 2) */
std::vector<int> convert_to_vector(int num) {
	std::vector<int> vector_notation; 
	vector_notation.resize(2);
	
	if (row_value(num, 1, 4)) vector_notation.at(0) = 0;
	else if (row_value(num, 5, 8)) vector_notation.at(0) = 1;
	else if (row_value(num, 9, 12)) vector_notation.at(0) = 2;
	else vector_notation.at(0) = 3;
	
	for (int i = 0; i < columns.size(); i++) {
		for (int j = 0; j < 4; j++) {
			if (num == columns.at(i).at(j)) { 
				vector_notation.at(1) = i; break;	
			}
		}
	}
	
	return vector_notation;
}

/* Returns in vector notation the set of points that are the winning coordinates 
	(you should only ever call this function after the game is won) */
std::vector<std::vector<int>> find_winning_combo(Tree &minimax_tree) {
	std::vector<std::vector<int>> return_vector;
	std::vector<int> winning_vector = minimax_tree.find_winning_combo(minimax_tree.check_win().at(1));
	for (int i = 0; i < winning_vector.size(); i++) {
		return_vector.push_back(convert_to_vector(winning_vector.at(i)));
	}
	
	return return_vector;
}

/* Finds numerical position of interest given its vector notation 
	ex. converts (0, 2) to 16 */
int convert_to_num(int x, int y) {
	return columns.at(y).at(x);
}

/* Generates a move for the AI 
	(for gorey details see Node.h/Node.cpp and Tree.h/Tree.cpp */
int AI_Move(Tree &minimax_tree, int difficulty) {
	/* initializes minimax tree with a specified depth; 
		larger depth means smarter but slower program */
	minimax_tree.initialize(difficulty);
	
	/* after all nodes are generated we compute the minimax for
		all nodes to find the top node's value */
	minimax_tree.evaluate_nodes();
	
	/* the AI will take the path that led to the top node's current value */
	return minimax_tree.decide_next_move();
}

/* The ncurses logic calls this middleman to know generate AI move and know
	where to put the AI's X or O */
std::vector<int> send_robot_move(Tree &minimax_tree, bool player_letter, int difficulty) {
	int ai_move = AI_Move(minimax_tree, difficulty);
	minimax_tree.current_game_state.at(ai_move - 1) = !player_letter;
	return convert_to_vector(ai_move);
}

/* The ncurses logic calls this middleman so the AI knows what position	
	the human has picked */
void receive_player_move(int row, int col, Tree &minimax_tree, bool player_letter) {
	int move = convert_to_num(row, col);
	minimax_tree.current_game_state.at(move - 1) = player_letter;
}

#endif
