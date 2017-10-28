
#include "stdafx.h"
#include "common.h"
#include "piece.h"
#include "MagicSquareGame.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

MagicSquareGame::MagicSquareGame() : GameBase(3, 3) {
	pieces.resize(board_height, vector<piece>(board_width));
	deck.resize(board_height*board_width);
	int k = 0;
	for (int i = 0; i < board_height*board_width; i++) {
		k++;
		deck[i].display = to_string(k);
		deck[i].name = to_string(k);
	}
	for (int i = 0; i < board_height; i++) {
		for (int j = 0; j < board_width; j++) {
			pieces[i][j].name = " ";
		}
	}
	longestDisplay = 1;	
	try {
		ifstream ifs("MagicSquareGame.txt");
		string line;
		getline(ifs, line);
		getline(ifs, line);
		if (line == "NODATA") {
			//we're done
		}
		else {
			istringstream iss(line);
			iss >> turnCount;
			while (!ifs.eof()) {
				getline(ifs, line);
				istringstream iss(line);
				int i, j;
				char num;
				iss >> num >> i >> j;
				pieces[i][j].display = num;
				pieces[i][j].name = num;
				deck[num - '0'].name = "";
				deck[num - '0'].display = " ";
			}
		}
	}
	catch (ifstream::failure &e) {
		cout << "We're having trouble readin your saved file - make sure everything is okay: " << e.what() << endl;
		throw stream_fail;
	}
	catch (...) {
		cout << "We're having trouble readin your saved file - it may not exist." << endl;
		throw stream_fail;
	}

};

bool MagicSquareGame::stalemate() {
	bool empty = true;
	for (size_t i = 0; i < deck.capacity(); i++) {
		if (deck[i].display != " ") {
			empty = false;
		}
	}
	if (empty && !done()) {
		return true;
	}
	return false;
}

int MagicSquareGame::turn() {

	unsigned int a;
	askWhich(a);

	unsigned int x;
	unsigned int y;
	askTo(a, x, y);

	pieces[x][y].name = to_string(a);
	pieces[x][y].display = to_string(a);
	deck[a-1].name = " ";
	deck[a-1].display = " ";

	cout << *this << endl;
	char move[256] = {};
	sprintf_s(move, sizeof(move), "%d to %d,%d", a, y, x);
	cout << move << endl << endl;




	return success;
}
int MagicSquareGame::askWhich(unsigned int &a) {
	cout << "Enter in an available piece or \"quit\" to quit the game" << endl << endl;
	prompt(a);

	return success;

}
int MagicSquareGame::askTo(unsigned int a, unsigned int &x, unsigned int &y) {
	cout << "Enter in the coordinate of where you want to move the pieces in the format x,y or \"quit\" to quit the game" << endl << endl;

	GameBase::prompt(x, y);

	if (pieces[x][y].name != " ") {
		cout << "There's already a piece there. ";
		askTo(a, x, y);
	}


	return success;
}

int MagicSquareGame::prompt(unsigned int &a) {
	string in;
	cin >> in;
	cout << endl;
	if (in == "quit") {
		throw quit;
	}
	else if (isValidPiece(in)) {
		a = in[0] - '0';
		return success;
	}
	cout << "Invalid unplayed piece, try again: ";
	prompt(a);
	return this_unreachable_variable_just_prevents_compiler_warnings_that_you_take_off_points_for;
}

bool MagicSquareGame::isValidPiece(string a) {
	//Check avail
	for (size_t i = 0; i < deck.capacity(); i++) {
		if (deck[i].name != " " && deck[i].name == a) {
			return true;
		}
	}
	return false;
}

bool MagicSquareGame::done() {
	int correctSum = 15;
	int sum = 0;
	//check rows
	for (int i = 0; i < board_height; i++) {
		sum = 0;
		for (int j = 0; j < board_width; j++) {
			sum += pieces[i][j].name[0] - '0';
		}
		if (sum != correctSum) {
			return false;
		}
	}

	//check columns
	for (int i = 0; i < board_width; i++) {
		sum = 0;
		for (int j = 0; j < board_height; j++) {
			sum += pieces[i][j].name[0] - '0';
		}
		if (sum != correctSum) {
			return false;
		}
	}

	//check diags
	sum = 0;
	int i = 0, j = 0;
	while (i < board_height && j < board_width) {
		sum += pieces[i][j].name[0] - '0';
		i++;
		j++;
	}
	if (sum != correctSum) {
		return false;
	}

	sum = 0;
	i = board_height-1, j = 0;
	while (i >= 0 && j < board_width) {
		sum += pieces[i][j].name[0] - '0';
		i--;
		j++;
	}
	if (sum != correctSum) {
		return false;
	}


	return true;
}

bool MagicSquareGame::isTurnBased(){ return false;}

ostream &operator<<(ostream &out, const MagicSquareGame &msg) {

	for (int i = msg.board_height - 1; i >= 0; i--) {
		cout << i << string(msg.longestDisplay, ' ');
		for (int j = 0; j < msg.board_width; j++) {
			string holder(msg.longestDisplay, ' ');
			for (size_t k = 0; k < msg.pieces[i][j].display.length(); k++) {
				holder.at(k) = msg.pieces[i][j].display[k];
			}



			out << holder << " ";
		}
		out << endl << endl;
	}
	out << "X" << string(msg.longestDisplay, ' ');
	for (int i = 0; i < msg.board_width; i++) {
		out << i << string(msg.longestDisplay, ' ');
	}
	out << endl << endl;
	out << "Available pieces: ";
	for (size_t i = 0; i < msg.deck.capacity(); i++) {
		out << msg.deck[i].display << " ";
	}
	out << endl;
	return out;
}

void MagicSquareGame::print() {
	cout << *this << endl;
}

int MagicSquareGame::save(bool real) {
	try {
		if (!real) {
			ofstream ofs("MagicSquareGame.txt");
			ofs << "MagicSquareGame" << endl << "NODATA" << endl;
		}
		else {
			ofstream ofs("MagicSquareGame.txt");
			ofs << "MagicSquareGame" << endl;
			ofs << turnCount << endl;
			for (size_t i = 0; i < pieces.size(); i++) {
				for (size_t j = 0; j < pieces[0].size(); j++) {
					if (pieces[i][j].display != " ") {
						ofs << pieces[i][j].display << " " << i << " " << j << endl;
					}
				}
			}
		}
	}
	catch (ofstream::failure &e) {
		cout << "Save operation failed: " << e.what() << endl;
		throw stream_fail;
	}
	return success;
}