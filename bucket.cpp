#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <fstream>
#include <chrono>
#include <thread>
#include <unistd.h>
#include "logic.h"
#include <algorithm>
#include <list>

#define YBOARDSTART 5
#define XBOARDSTART 5

std::vector< std::string > sayings = { 
	"Fine. I'll gut you standing.",
	"Time to die.",
	"Not going down.",
	"Man, you're ugly.",
	"I'll tell you about my mother...",
	"Goina surrender?",
	"BANG BANG BANG - K R S 1",
	"Wa da da dang",
	"Ain't going out like that",
	"*blinks*",
	"DO YOU READ",
	"Bet you could suck a golfball through a gardenhose",
	"What a nancy",
	"Yeah hang on",
	"M u S t B e U n D e R 1 0 s E c O n D s"
};
std::string player_name = "";

// - - - - - - - - - I N I T - - - - - - - - -
void init_curses() {
	initscr();
	cbreak(); //NOT SURE IF NEEDED
	noecho(); //NOT SURE IF NEEDED
	keypad(stdscr, TRUE);
}

// - - - - - - - - - U S E R I N P U T - - - - - - - - -
//CALLED AT BEGINING OF THE GAME
std::string get_initials() {
	mvprintw(30, 10, "WHAT IS YOUR NAME? ");
	refresh();
	nocbreak();
	echo();
	std::string input;
	int ch = getch();
	int counter = 0;
	while(ch != '\n' && counter < 20 && ch != ' ') {
		counter++;
		input.push_back(ch);
		ch = getch();
	}
	cbreak();
	noecho();
	mvprintw(30, 10, "                                            ");
	std::ofstream output_file;
	output_file.open("scores.txt",std::ios_base::app);
	output_file << "\n" << input << " 0";
	player_name = input;
	return input;
}

// - - - - - - - - - U S E R I N P U T - - - - - - - - -
char get_x_or_o() {
	mvprintw(30, 10, "PRESS X TO START FIRST OR O TO START SECOND");
	refresh();
	int ch = getch();
	if(ch == 'x' || ch == 'X'){
		mvprintw(30, 10, "                                            ");
		return 'X';
	}
	else if(ch == 'o' || ch == 'O'){
		mvprintw(30, 10, "                                            ");
		return 'O';
	}
	else{ return get_x_or_o(); }
}

// - - - - - - - - - U S E R I N P U T - - - - - - - - -
int how_hard() {
	mvprintw(31, 10, "HOW DIFFICULT DO YOU WANT THIS?(1-4)");
	refresh();
	int ch = getch();
	if(ch == '1'){
		mvprintw(31, 10, "I'm too young to die                ");
		return 1;
	}
	else if(ch == '2') {
		mvprintw(31, 10, "Hey, not too rough                  ");
		return 2;
	} 
	else if(ch == '3') {
		mvprintw(31, 10, "Hurt me plenty                      ");
		return 3;
	}
	else if(ch == '4') {
		mvprintw(31, 10, "Nightmare!                          ");
		return 4;
	}
	else{ return how_hard(); }

}

// - - - - - - - - - V E C T O R M A N I P U L A T I O N - - - - - - - - -
std::vector< std::string > merge_scores(std::vector< std::string > input) {
	std::vector< std:: string > scores = input;
	for(int i = 0; i < scores.size(); i++) {
		int num_score = stoi(scores[i].substr(scores[i].find(" ")+1));
		std:: string name = scores[i].substr(0, scores[i].find(" "));
		for(int j = i + 1; j < scores.size(); j++){
			int other_score = stoi(scores[i].substr(scores[i].find(" ")+1));
			std::string other_name = scores[j].substr(0, scores[j].find(" "));
			if(name == other_name){
				num_score += other_score;
				scores[i] = name + " " + std::to_string(num_score);
				scores.erase(scores.begin() + j);
			}
		}
	}
	return scores;
}

// - - - - - - - - - V E C T O R M A N I P U L A T I O N - - - - - - - - -
std::vector<std::string> sort_scores(std::vector< std::string > highscores){
	std::list<std::pair< int, std::string> > list_pair;
	std::vector<std::string> sorted_highscores;
	for(int i=0; i<highscores.size(); i++){
		std::string line = highscores[i];
		for(int j=0; j<line.length(); j++){
			char ch = line[j];
			if(isdigit(ch)){
				int num = stoi(line.substr(j));
				list_pair.push_back(make_pair(num, highscores[i]));
				break;
			}
		}
	}
	list_pair.sort();
	std::vector<std::pair<int, std::string>> v;
	v.reserve(list_pair.size());
	std::copy(std::begin(list_pair), std::end(list_pair), std::back_inserter(v));
	for(int i=0; i<v.size(); i++){
		std::pair<int, std::string> p1 = v[i];
		sorted_highscores.push_back(std::get<1>(p1));
	}
	std::reverse(sorted_highscores.begin(), sorted_highscores.end());
	return merge_scores(sorted_highscores);
}

// - - - - - - - - - R E A D F I L E - - - - - - - - -
std::vector< std::string > read_scores_from_file(std::string score_file_name){
	std::ifstream file(score_file_name.c_str());
	std::vector< std::string > highscores;
	int counter = 1;
	std::string line;
	while(std::getline(file, line)) {
		highscores.push_back(line);
	}
	return sort_scores(highscores);
}

// - - - - - - - - - V E C T O R M A N I P U L A T I O N - - - - - - - - -
std::vector< std::string > increment_score(std::vector< std::string > input) {
	std::vector< std::string > scores = input;
	for(int i = 0; i < scores.size(); i++) {
		int num_score = stoi(scores[i].substr(scores[i].find(" ")+1));
		std:: string name = scores[i].substr(0, scores[i].find(" "));
		if(name == player_name) {

			num_score++;
			std::string new_name_and_score = name + " " + std::to_string(num_score);
			scores[i] = new_name_and_score;
		}
	}
	return scores;
}

// - - - - - - - - - W R I T E F I L E - - - - - - - - -
void write_scores_to_file(std::vector< std::string > scores, bool did_they_win){
	std::ofstream file("scores.txt", std::ios::in);
	file.close();
	file.open("scores.txt", std::ios::out | std::ios::trunc);

	file.close();		//CLEARS THE FILE OUT!
	std::ofstream file_("scores.txt");
	if(did_they_win) {
		scores = increment_score(scores);
		scores = sort_scores(scores);
	}
	int size = scores.size();
	if(scores.size() > 5) {
		size = 5;
	}
	for(int name = 0; name < size; name++) {
		file_ << scores[name] << "\n";
	}

}

// - - - - - - - - - U S E R I N P U T - - - - - - - - -
bool play_again_prompt(){
	mvprintw(30, 10, "                                                 ");
	mvprintw(30, 10, "PLAY AGAIN? (y/n) ");
	refresh();
	int ch = getch();
	if(ch == 'y'){
		return TRUE;
	}
	else if(ch == 'n'){
		mvprintw(32, 15, "Later gator!");
		mvprintw(33, 15, "Press any key to exit.");
		return FALSE;
	}
	else{
		return play_again_prompt();
	}
}

// - - - - - - - - - D R A W - - - - - - - - -
//CALLED AT BEGINING OF THE GAME
void draw_start_button() {
	mvprintw(4, 18, "THE RED ONE");
	mvprintw(10, 19, "Team 1: Chris, Joseph, Yuxuan");
	attron(A_REVERSE);
	mvprintw(30, 10, "( P R E S S E N T E R ) S T A R T ( P R E S S E N T E R )");
	attroff(A_REVERSE);
	refresh();
	char enter_check;
	while(enter_check != '\n'){
		enter_check = getch();
	}
	mvprintw(10, 19, "                             ");
	mvprintw(30, 10, "                                                         ");
}
// - - - - - - - - - D R A W - - - - - - - - -
//DRAWS THE RED OUTLINE
void draw_splash() {
	start_color();
	init_pair(1, COLOR_BLACK, COLOR_RED);
	attron(COLOR_PAIR(1));
	for(int col = 1; col < 78; col++) {
		mvaddch(0, col,'-');
		mvaddch(38, col, '_');
	}
	for(int row = 0; row < 39; row++) {
		mvaddch(row,0,'|');
		mvaddch(row,78,'|');
	}
	attroff(COLOR_PAIR(1));
}

// - - - - - - - - - D R A W - - - - - - - - -
//DRAWS ERASE INITIAL GAME BOARD
void clear_board() {
	mvprintw(15+YBOARDSTART,17,"DEMO OVER! PLEASE LIKE AND SUBSCRIBE!");
	mvprintw(15+YBOARDSTART,17,"                                     ");
	for(int row = 0; row < 9; row++) {
		for(int col = 0; col < 69; col++) {
			mvaddch(YBOARDSTART + row, XBOARDSTART + col, ' ');
		}
	}
}

// - - - - - - - - - D R A W - - - - - - - - -
//DRAWS ERASE INITIAL GAME BOARD
void draw_instructions() {
	mvprintw(35,15,"PRESS SPACE TO PLACE A MARK! ARROW KEYS TO MOVE!");
}

// - - - - - - - - - D R A W - - - - - - - - -
//DRAWS INITIAL BOARD
void draw_board() {
	start_color();
	init_pair(2, COLOR_WHITE, COLOR_BLACK);
	attron(COLOR_PAIR(2));
	for(int row = 0; row < 9; row++) {
		int board_separator = 0;
		for(int col = 0; col < 69; col += 3) {	//CAUTION ITERATOR +3
			if(board_separator != 5) {		//IF ON A BOARD
				mvaddch(YBOARDSTART + row, XBOARDSTART + col, '|');
				board_separator++;
			}
			else { 
				board_separator = 0;
			}
		}
		if((row % 2) == 0) {
			for(int col = 1; col < 66; col++){
				mvaddch(YBOARDSTART + row, XBOARDSTART + col, '-');
			}
		}
	}
	refresh();
}

// - - - - - - - - - D R A W - - - - - - - - -
//DRAWS HIGHSCORES
void draw_scores(std::vector< std::string > ledger) {
	int size = ledger.size();
	if(ledger.size() > 5){ 
		size = 5;
		std::string name = ledger[ledger.size()-1];
		mvprintw(10 + 5 + YBOARDSTART, 1, "                             " );
		mvprintw(10 + 5 + YBOARDSTART, 1, name.c_str()); 
	}
	for(int name_index = 0; name_index < size; name_index++){
		std::string name = ledger[name_index];
		mvprintw(10 + name_index + YBOARDSTART, 1, "                             " );
		mvprintw(10 + name_index + YBOARDSTART, 1, name.c_str());
	}
}

// - - - - - - - - - M A K E - - - - - - - - -
//INITIAL CALL TO CREATE EMPTY DATA STRUCTURE
std::vector< std::vector< char > > make_board() {

	std::vector< std::vector< char > > * board = 
			new std::vector< std::vector< char > >;

	for(int row = 0; row < 4; row++ ) {
		std::vector< char > temp_row;
		for(int col = 0; col < 16; col++){
			temp_row.push_back(' ');
		}
		board->push_back(temp_row);
	}
	return *board;
}
// - - - - - - - - - C A L C U L A T I O N - - - - - - - - -
int y_pos_to_row(int y_pos) {
	return (y_pos - 1 - YBOARDSTART) / 2;			//WEIRD CONVERSION FROM ncurses COORDS
}

// - - - - - - - - - C A L C U L A T I O N - - - - - - - - -
int x_pos_to_col(int x_pos) {
	int col = (x_pos - 2 - XBOARDSTART) / 3;
	if(col > 3) {
		col = col - 2;
		if(col > 9) {
			col = col -2;
			if(col > 13) {
				col = col - 2;
			}
		}
	}
	return col;
}

// - - - - - - - - - C A L C U L A T I O N - - - - - - - - -
int row_to_y_pos(int row) {
	return row * 2 + 1 + YBOARDSTART;
}

// - - - - - - - - - C A L C U L A T I O N - - - - - - - - -
int col_to_x_pos(int col) {
	int x_pos = col;
	if(col > 11){
		x_pos += 6; //6
	} else if(col > 7) {
		x_pos += 4; //4
	} else if(col > 3) {
		x_pos += 2; //2
	}
	return x_pos*3+2+XBOARDSTART;
}

// - - - - - - - - - O P E N ? - - - - - - - - 
//CHECKS IF SPOT HAS BEEN PLAYED IN YET
bool spot_open(std::vector< std::vector< char > > board, int y, int x) {
	int row = y_pos_to_row(y);
	int col = x_pos_to_col(x);
	if(board[row][col] == ' ') {	//' ' IS THE EMPTY CHAR FOR THE STRUCTURE
		return TRUE;
	} else { return FALSE; }
}

// - - - - - - - - - O P E N ? - - - - - - - -
bool spot_valid(int y_pos, int x_pos) {
	int row = y_pos_to_row(y_pos);
	int col = (x_pos - 2 - XBOARDSTART) / 3;
	if ((col == 4) || (col == 5) || (col == 10) || (col == 11) || (col == 16) || (col == 17)){
		return FALSE;
	}
	else{ return TRUE; }
}

// - - - - - - - - - U P D A T E - - - - - - - - -
//ADDS player_char TO THE board FOR DATA KEEPING
void spot_update(std::vector< std::vector< char > > * board, int y, int x, char& player_char) {
	int row = y_pos_to_row(y);
	int col = x_pos_to_col(x);
	board->at(row).at(col) = player_char;
	if(player_char == 'O'){
		player_char = 'X';
	} else { player_char = 'O';}
}

// - - - - - - - - - M O V E - - - - - - - - -
//CHECKS SAFETY OF MOVEMENT AND MOVES CURSOR
void my_move(int user_input_char, int& y_pos, int& x_pos) {
	if(user_input_char != ' ') {
		switch (user_input_char) {
			case KEY_UP:
				if(y_pos != 1+YBOARDSTART) { move(y_pos = y_pos - 2, x_pos); }
				break;
			case KEY_DOWN:
				if(y_pos != 7+YBOARDSTART) { move(y_pos = y_pos + 2, x_pos); }
				break;
			case KEY_LEFT:
				if(x_pos != 2+XBOARDSTART) { move(y_pos, x_pos = x_pos - 3); }
				break;
			case KEY_RIGHT:
				if(x_pos != 65+XBOARDSTART) { move(y_pos, x_pos = x_pos + 3); }
				break;
		}
	}
	refresh();
}

// - - - - - - - - - D E B U G - - - - - - - - -
//FUNCTION TO MAKE SURE BOARD DATA IS BEING PRINTED CORRECTLY
void debug_send_to_file(std::vector< std::vector< char > > board){
	std::ofstream output_file("debug_info.txt");
	output_file << "-MOST RECENT TEST-";		//COMMENT OUT THIS LINE TO MAKE CSV
	for(int row = 0; row < 4; row++){
		output_file << "\n";
		for(int col = 0; col < 16; col++){
			output_file << board[row][col];
			if(col != 15) { output_file << " , "; }
		}
	}
}

// - - - - - - - - - B O N U S - - - - - - - - -
void blink_red(std::vector< std::vector< int >> win_set, char marker) {
	start_color();
	init_pair(4, COLOR_BLACK, COLOR_RED);
	attron(COLOR_PAIR(4));
	init_pair(5, COLOR_WHITE, COLOR_BLACK);
	int user_input_char = 0;
	for(int i = 0; i < 5; i++){
		for(int i = 0; i < 4; i++) {
			attron(COLOR_PAIR(4));
			mvaddch(row_to_y_pos(win_set[i][0]), col_to_x_pos(win_set[i][1]), marker);
			refresh();
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		for(int i = 0; i < 4; i++) {
			attron(COLOR_PAIR(5));
			mvaddch(row_to_y_pos(win_set[i][0]), col_to_x_pos(win_set[i][1]), marker);
			refresh();
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(500));

	}
	attroff(COLOR_PAIR(5));
}


// - - - - - - - - - G A M E P L A Y - - - - - - - - -
bool play_game(char p_c, int difficulty) {
	bool win = FALSE;
	Node::bunch_nodes.clear();
	bool x_or_o = p_c != 'X';
	spots_filled = 0;
	Tree minimax_tree(x_or_o);
	minimax_tree.generate_win_states();	
	std::vector< std::vector< char > > board = make_board();
	int y_pos = 1+YBOARDSTART, x_pos = 2+YBOARDSTART;
	int user_input_char = 0;	//BASE, IS OVERWRITTEN CONSTANTLY
	char player_char = p_c;
	char turn = 'X';
	move(y_pos,x_pos);			//CRUCIAL OTHERWISE CURSOR STARTS AT BOTTOM
	while(user_input_char != 'q'){
		if(player_char == turn)
		{
			user_input_char = getch();
			my_move(user_input_char, y_pos, x_pos);	//SAFETY CHECKER MOVEMENT
			if((user_input_char == ' ')){
				getyx(stdscr, y_pos, x_pos);
				if(spot_open(board, y_pos, x_pos) && spot_valid(y_pos, x_pos)){
					mvaddch(y_pos, x_pos, player_char);
					spot_update(&board, y_pos, x_pos, turn);
					mvprintw(10+YBOARDSTART,30, "                                   ");
					move(y_pos, x_pos);
					std::this_thread::sleep_for(std::chrono::milliseconds(100));
					receive_player_move(y_pos_to_row(y_pos),x_pos_to_col(x_pos), minimax_tree, x_or_o);
					spots_filled++;
					refresh();
				}
				else{ mvprintw(10+YBOARDSTART, 30, "BAD MOVE! YOU'LL GET IT NEXT TIME.."); move(y_pos,x_pos); }
			}
		} else {
			int num = rand() % sayings.size();
			mvprintw(20+YBOARDSTART, 10, sayings[num].c_str());
			refresh();
			std::vector<int> robot_moves = send_robot_move(minimax_tree, x_or_o, difficulty);
			mvprintw(20+YBOARDSTART, 10, "                                          ");
			int robot_x_move = col_to_x_pos(robot_moves[1]); //0 CHANGES TO WHAT AI MOVE RETURNS
			int robot_y_move = row_to_y_pos(robot_moves[0]);
			mvaddch(robot_y_move, robot_x_move, turn);
			spot_update(&board, robot_y_move, robot_x_move, turn);
			move(robot_y_move, robot_x_move);
			spots_filled++;
		}
		if(minimax_tree.check_win().at(0)) {
			if(minimax_tree.check_win().at(1) && x_or_o) {
				win = TRUE; 
				mvprintw(15+YBOARDSTART,17,"YOU WON!");
			}
			if (!minimax_tree.check_win().at(1) && !x_or_o) {
				win = TRUE;
				mvprintw(15+YBOARDSTART,17,"YOU WON!"); 
			}	
			std::vector<std::vector<int>> winner = find_winning_combo(minimax_tree);
			char win_char = !minimax_tree.check_win().at(1) ? 'X' : 'O';
			blink_red(winner, win_char);
			break;
		}
		if(spots_filled == 64){ break; }
	}
	return win;
}

int main() {
	generate_helper_board();
	init_curses();
	draw_splash();
	draw_start_button();
	int difficulty;
	bool play = TRUE;
	while(play){
		clear_board();
		draw_board();
		get_initials();
		draw_scores(sort_scores(read_scores_from_file("scores.txt")));
		draw_instructions();
		difficulty = how_hard();
		char option = 'O';
		option = get_x_or_o();
		bool win = play_game(option, difficulty);
		write_scores_to_file(read_scores_from_file("scores.txt"), win);
		play = play_again_prompt();
	}
	getch();
	endwin();
}