#include "Tree.h"

using namespace std;

/* Creates the minimax tree with the specified depth 
		(after 4 it gets ridiculous) */
void Tree::initialize(int depth) {
	/* Initializes the root with not parent (nullptr, game state, and
		the whether the player is X; also, the root is not a teriminal node (false) */
	_root = new Node(nullptr, current_game_state, _player_x, false);
	/* recursively build tree using the initial node */
	build_tree(_root, depth);
}

/* Recursive function that builds the minimax tree 
	(lookaheads is how deep we still have to go from the current node) */
void Tree::build_tree(Node* parent, int lookaheads) {
	if (lookaheads == 0) return; /* Base case: we're done building the tree once we reach the terminals */
	
	/* contains vector of possible moves from the current parent node */
	vector<vector<int>> current_moves = possible_moves(parent->game_state, parent->max);
	for (int i = 0; i < current_moves.size(); i++) {
		/* make a child for each of those moves (current_moves.at(i) is a whole separarate game state) */
		parent->children.push_back(new Node(parent, current_moves.at(i), !parent->max, lookaheads == 1));
		build_tree(parent->children.at(i), lookaheads - 1);
	}
}

/* Evaluates and assigns minimax to each node starting with the terminals */
void Tree::evaluate_nodes() {
	/* Enters recursive minmax method */
	compute_minimax(_root, -99999, 99999);
}

/* Recursively computes minimax of a given node */
int Tree::compute_minimax(Node* to_compute, int alpha, int beta) {
	if (to_compute->terminal_node) /* terminal node */ {
		/* Simply find the heuristic for the terminal nodes */
		to_compute->value = util_eval(to_compute->game_state);
		return util_eval(to_compute->game_state);
	}
	else if (to_compute->max) /* maximizing node */ {
		int value = -99999;
		for (int i = 0; i < to_compute->children.size(); i++) {
			/* Call yourself again on all your children */
			value = max(value, compute_minimax(to_compute->children.at(i), alpha, beta));
			alpha = max(alpha, value);
			if (beta <= alpha) break;
		}
		to_compute->value = value;
		return value;
	} 
	else /* minimizing node */ {
		int value = 99999;
		for (int i = 0; i < to_compute->children.size(); i++) {
			/* Call yourself again on all your children */
			value = min(value, compute_minimax(to_compute->children.at(i), alpha, beta));
			beta = min(beta, value);
			if (beta <= alpha) break;
		}
		to_compute->value = value;
		return value;
	}
}

/* Returns the value in numeric notation of the square the AI 
	will move to */
int Tree::decide_next_move() {
	/* Needs to find what move gave _root its value (which child has the same value as me?) */
	for (int i = 0; i < _root->children.size(); i++) {
		if (_root->children.at(i)->value == _root->value) {
			return _root->children.at(i)->game_state.at(64);
		}
	}
}

void Tree::generate_win_states() {		
	generate_horizontal_planes();
	generate_plane_diagonals();
	generate_vertical_planes();
	/* Top-to-bottom vertical lines 1 to 16 */
	for (int i = 0; i < 16; i++) {
		vector<int> win_state;
		for (int j = 0; j < 4; j++) {
			win_state.push_back((i + 1) + (j * 16));
		}
		win_states.push_back(win_state);
	}
	/* Corner-to-corner diagonals */
	win_states.push_back({1, 22, 43, 64});
	win_states.push_back({16, 27, 38, 49});
	win_states.push_back({4, 23, 42, 61});
	win_states.push_back({13, 26, 39, 52});

}

void Tree::generate_plane_diagonals() {
		/* Plane diagonals 1 to 4 */
	for (int i = 0; i < 4; i++) {
		vector<int> win_state;
		for (int j = 0; j < 4; j++) {
			win_state.push_back((j + 1) + (4 * j) + (i * 16));
		}
		win_states.push_back(win_state);
	}
	
	/* Plane diagonals 5 to 8 */
	for (int i = 0; i < 4; i++) {
		vector<int> win_state;
		for (int j = 0; j < 4; j++) {
			win_state.push_back((j + 4) + (2 * j) + (i * 16));
		}
		win_states.push_back(win_state);
	}
}

void Tree::generate_horizontal_planes() {
	/* Horizontal plane lines 1 to 16*/
	for (int i = 0; i < 16; i++) {
		vector<int> win_state;
		for (int j = 0; j < 4; j++) {
			win_state.push_back((j + 1) + 4 * i);
		}
		win_states.push_back(win_state);
	}
	/* Horizontal plane diagonals 1 to 4 */
	for (int i = 0; i < 4; i++) {
		vector<int> win_state;
		for (int j = 0; j < 4; j++) {
			win_state.push_back((i * 4 + 1) + (17 * j));
		}
		win_states.push_back(win_state);
	}
	
	/* Horizontal plane diagonals 5 to 8 */
	for (int i = 0; i < 4; i++) {
		vector<int> win_state;
		for (int j = 0; j < 4; j++) {
			win_state.push_back(4 * (i + 1) + (15 * j));
		}
		win_states.push_back(win_state);
	}
}

void Tree::generate_vertical_planes() {
	/* Vertical plane lines 1 to 16 */
	for (int i = 0; i < 16; i++) {
		vector<int> win_state;
		for (int j = 0; j < 4; j++) {
			if (i < 4)
				win_state.push_back(i + 1 + 4 * j);
			else if (i < 8) 
				win_state.push_back(i + 13 + 4 * j);
			else if (i < 12)
				win_state.push_back(i + 25 + 4 * j);
			else if (i < 16)
				win_state.push_back(i + 37 + 4 * j);
		}
		win_states.push_back(win_state);
	}

	/* Vertical plane diagonals 1 to 4 */
	for (int i = 0; i < 4; i++) {
		vector<int> win_state;
		for (int j = 0; j < 4; j++) {
			win_state.push_back((i + 1) + 20 * j);
		}
		win_states.push_back(win_state);
	}
	
	/* Vertical plane diagonals 5 to 8 */
	for (int i = 0; i < 4; i++) {
		vector<int> win_state;
		for (int j = 0; j < 4; j++) {
			win_state.push_back((i + 13) + 12 * j);
		}
		win_states.push_back(win_state);
	}
}


/* Generates all possible moves for X or O from a current game state 
	returns moves as a vector of game states */
vector<vector<int>> Tree::possible_moves(vector<int> game_state, bool max) {
	vector<vector<int>> move_vector;
	game_state.resize(65);
	if (max) /* maximizing player */ {
		for (int i = 0; i < 64; i++) {
			if (game_state.at(i) == -1) {
				game_state.at(i) = 1;
				game_state.at(64) = i + 1;
				move_vector.push_back(game_state);
				game_state.at(i) = -1;
			}
		}
	}
	
	else /* minimizing player */ {
		for (int i = 0; i < 64; i++) {
			if (game_state.at(i) == -1) {
				game_state.at(i) = 0;
				game_state.at(64) = i + 1;
				move_vector.push_back(game_state);
				game_state.at(i) = -1;
			}
		}
	}
	
	return move_vector;
}

/* Evaluates the utility function for X of any given game state 
	(should only used to find the utility of terminal nodes) */
int Tree::util_eval(vector<int>& game_state) {
	int util_func = 0;
	for (int i = 0; i < 76; i++) {
		int x_count = 0;
		int o_count = 0;
		for (int j = 0; j < 4; j++) {
			if (game_state.at(win_states.at(i).at(j) - 1) == 0) {
				o_count++;
			}
			else if (game_state.at(win_states.at(i).at(j) - 1) == 1) {
				x_count++;
			}
		}
		if ((x_count > 0 && o_count > 0) || (x_count == 0 && o_count == 0)) continue;
		bool x = (x_count > 0);
		if (x) {
			if (x_count == 4) util_func += 1000;
			else if (x_count == 3) util_func += 100;
			else if (x_count == 2) util_func += 10;
			else util_func++;
		} 
		else {
			if (o_count == 4) util_func -= 1000;
			else if (o_count == 3) util_func -= 100;
			else if (o_count == 2) util_func -= 10;
			else util_func--;
		}
	}
	return util_func;
}
/* Returns true if a player has won the game (check_win.at(0)), 
	will also return a second true/false if X/O has won check_win.at(1) */
vector<bool> Tree::check_win() {
	vector<bool> win_vec;
	win_vec.resize(2);
	win_vec.at(0) = false; win_vec.at(1) = false;
	for (int i = 0; i < 76; i++) {
		int x_count = 0;
		int o_count = 0;
		for (int j = 0; j < 4; j++) {
			if (current_game_state.at(win_states.at(i).at(j) - 1) == 1)
				x_count++;
			else if (current_game_state.at(win_states.at(i).at(j) - 1) == 0)
				o_count++;
		}
		if (x_count == 4) {
			win_vec.at(0) = true;
			win_vec.at(1) = true;
			break;
		}
		else if (o_count == 4) {
			win_vec.at(0) = true;
			break;
		}
	}
	return win_vec;
}

/* After the game is won, returns which positions led to a win (useful for UI) 
	NOTE: THIS IS NOT VECTOR NOTATION, THIS IS NUMERIC NOTATION
	SEPARATE FUNCTION EXISTS IN LOGIC.H TO CONVERT THIS VECTOR INTO vector<vector<int>> */
vector<int> Tree::find_winning_combo(bool who_won) {
	vector<int> return_vector;
	int win_space;
	for (int i = 0; i < 76; i++) {
		int count = 0;
		for (int j = 0; j < 4; j++) {
			if (current_game_state.at(win_states.at(i).at(j) - 1) == who_won)
				count++;
		}
		if (count == 4) { 
			for (int j = 0; j < win_states.at(0).size(); j++) {
				return_vector.push_back(win_states.at(i).at(j));
			}
		}
	}
	return return_vector;
}

/* Deletes all nodes before Tree goes out of scope to avoid running out of memory*/
void Tree::destroy_tree() {
	for (size_t i = 0; i < Node::bunch_nodes.size(); i++) {
		delete Node::bunch_nodes.at(i);
	}
}

/* This method was used for debugging purposes 
	Prints in numeric notations all 76 possible ways to win */
void Tree::print_win_states() {
	int k = 1;
	for (int i = 0; i < win_states.size(); i++) {
		cout << k << ": ";
		for (int j = 0; j < win_states.at(i).size(); j++) {
			cout  << win_states.at(i).at(j) << " ";
		}
		k++;
		cout << endl;
	}
}

/* Destructor cleanup */
Tree::~Tree() {
	destroy_tree();
}
