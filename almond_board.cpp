
#include "stdafx.h"
#include "common.h"
#include "piece.h"
#include "almond_board.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

almond_board::almond_board() : GameBase(5, 5, 4) {
	pieces.resize(board_height, vector<piece>(board_width));

	
	longestDisplay = 1;

	try {
		ifstream ifs("NineAlmonds.txt");
		string line;
		getline(ifs, line);
		getline(ifs, line);
		if (line == "NODATA") {
			for (int i = board_width/2 -1 ; i <= board_width/2 + 1; i++) {
				for (int j = board_width/2 - 1; j <= board_width/2 + 1; j++) {
					pieces[i][j].color = brown;
					pieces[i][j].display = "A";
					pieces[i][j].name = "almond";
				}
			}
		}
		else {
			istringstream iss(line);
			iss >> turnCount;
			while (!ifs.eof()) {
				getline(ifs, line);
				istringstream iss(line);
				int i, j;
				iss >> i >> j;
				pieces[i][j].display = "A";
				pieces[i][j].name = "almond";
				pieces[i][j].color = brown;
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

bool almond_board::done() {
	if (pieces[board_height/2][board_width/2].name != "almond") {
		return false;
	}
	for (int i = 0; i < board_height; i++) {
		for (int j = 0; j < board_width; j++) {
			if ((i != board_height/2 && j != board_width/2) && pieces[i][j].name == "almond"){
				return false;
			}
		}
	}
	return true;
}

bool almond_board::stalemate() {
	if (done() || anyValid()) {
		return false;
	}
	return true;
}

bool almond_board::anyValid() {
	bool valid = false;
	for (int i = 0; i < board_height; i++) {
		for (int j = 0; j < board_width; j++) {
			if (pieces[i][j].name == "almond" && canMove(i, j)) {
				valid = true;
			}
			
		}
	}
	return valid;
}

bool almond_board::canMove(int i, int j) {
	bool valid = false;
	
	//up
	if (i + 2 <= 4 && pieces[i + 2][j].display == " ") {
		if (pieces[i + 1][j].name == "almond") {
			valid = true;
		}
	}
	
	//down
	if (i - 2 >= 0 && pieces[i - 2][j].display == " ") {
		if (pieces[i - 1][j].name == "almond") {
			valid = true;
		}
	}
	
	//right
	if (j + 2 <= 4 && pieces[i][j + 2].display == " ") {
		if (pieces[i][j + 1].name == "almond") {
			valid = true;
		}
	}

	//left
	if (j - 2 >= 0 && pieces[i][j - 2].display == " ") {
		if (pieces[i][j - 1].name == "almond") {
			valid = true;
		}
	}

	//upper right
	if (j + 2 <= 4 && i + 2 <= 4 && pieces[i + 2][j + 2].display == " ") {
		if (pieces[i + 1][j + 1].name == "almond") {
			valid = true;
		}
	}
	
	//upper left
	if (j - 2 >= 0 && i + 2 <= 4 && pieces[i + 2][j - 2].display == " ") {
		if (pieces[i + 1][j - 1].name == "almond") {
			valid = true;
		}
	}
	
	//lower right
	if (j + 2 <= 4 && i - 2 >= 0 && pieces[i - 2][j + 2].display == " ") {
		if (pieces[i - 1][j + 1].name == "almond") {
			valid = true;
		}
	}
	
	//lower left
	if (j - 2 >= 0 && i - 2 >= 0 && pieces[i - 2][j - 2].display == " ") {
		if (pieces[i - 1][j - 1].name == "almond") {
			valid = true;
		}
	}
	
	return valid;
}

int almond_board::turn() {

	unsigned int a;
	unsigned int b;
	askFrom(a, b);

	unsigned int x;
	unsigned int y;
	askTo(a, b, x ,y);

	doMove(a, b, x, y);

	cout << *this << endl;
	char move[256] = {};
	sprintf_s(move, sizeof(move), "%d,%d to %d,%d", b, a, y, x);
	cout << move << endl << endl;


	//continue the move?
	if (canMove(x, y)) {
		askCont(x, y, move);
	}




	return success;
}
int almond_board::doMove(unsigned int a, unsigned int b, unsigned int x, unsigned int y) {
	//clear From coord
	pieces[a][b].name.clear();
	pieces[a][b].display = " ";
	pieces[a][b].color = no_color;

	//set To coord
	pieces[x][y].name = "almond";
	pieces[x][y].display = "A";
	pieces[x][y].color = brown;

	//clear midpiece
	pieces[(x + a) / 2][(y + b) / 2].name.clear();
	pieces[(x + a) / 2][(y + b) / 2].display = " ";
	pieces[(x + a) / 2][(y + b) / 2].color = no_color;

	return 0;
}
int almond_board::askCont(unsigned int x, unsigned int y, char* move) {
	cout << "Do you want to continue this move? (y/n)" << endl << endl;
	string ans = "";
	cin >> ans;
	cout << endl;
	try {
		if (ans == "y") {
			unsigned int a;
			unsigned int b;
			askTo(x, y, a, b);

			doMove(x, y, a, b);

			cout << *this << endl;
			char newMove[256] = {};
			sprintf_s(newMove, sizeof(newMove), "%s to %d,%d", move, b, a);
			cout << newMove << endl << endl;

			if (canMove(a, b)) {
				askCont(a, b, newMove);
			}
		}
		else if (ans == "n") {
			return 0;
		}
		else if (ans == "quit") {
			throw quit;
		}
		else {
			cout << "Invalid input. ";
			askCont(x, y, move);
		}
	}
	catch (rets quit) {
		turnCount++;
		throw quit;
	}
	return 0;
}

int almond_board::askFrom(unsigned int &a, unsigned int &b) {
	cout << "Enter in the coordinate of the piece you want to move in the format x,y or \"quit\" to quit the game" << endl << endl;
	prompt(a, b);

	if (!(pieces[a][b].name == "almond")) {
		cout << "Not a valid starting place. ";
		askFrom(a, b);
	}
	else if(!canMove(a, b)) {
		cout << "This piece can't move anywhere. ";
		askFrom(a, b);
	}
	return success;
}
int almond_board::askTo(unsigned int a, unsigned int b, unsigned int &x, unsigned int &y) {
	cout << "Enter in the coordinate of where you want to move the pieces in the format x,y or \"quit\" to quit the game" << endl << endl;

	prompt(x, y);

	//check if 2 spaces away
	if (!(
		(x == a + 2 && y == b + 2) ||
		(x == a		&& y == b + 2) ||
		(x == a + 2 && y == b) ||
		(x == a + 2 && y == b - 2) ||
		(x == a		&& y == b - 2) ||
		(x == a - 2 && y == b) ||
		(x == a - 2 && y == b + 2) ||
		(x == a - 2 && y == b - 2)
		)) {
		cout << "The spot you chose wasn't two spaces away. ";
		askTo(a, b, x, y);
	}
	//check if there is a piece in destination
	if (pieces[x][y].name == "almond") {
		cout << "There seems to already be a piece where you want to move. ";
		askTo(a, b, x, y);
	}
	//check if there is a piece in between w/ midpoint
	if (pieces[(x + a) / 2][(y + b) / 2].name != "almond") {
		cout << "You need to jump an almond in order to move. ";
		askTo(a, b, x, y);
	}
	return success;
}

ostream &operator<<(ostream &out, const almond_board &ab) {
	
	for (int i = ab.board_height-1; i >= 0; i--) {
		out << i << string(ab.longestDisplay, ' ');
		for (int j = 0; j < ab.board_width; j++) {
			string holder(ab.longestDisplay, ' ');
			for (size_t k = 0; k < ab.pieces[i][j].display.length(); k++) {
				holder.at(k) = ab.pieces[i][j].display[k];
			}



			out << holder << " ";
		}
		out << endl << endl;
	}
	out << "X" << string(ab.longestDisplay, ' ');
	for (int i = 0; i < ab.board_width; i++) {
		cout << i << string(ab.longestDisplay, ' ');
	}
	out << endl;
	return out;
}
bool almond_board::isTurnBased() { return true; }
void almond_board::print() {
	cout << *this << endl;
}
int almond_board::save(bool real) {
	try {
		if (!real) {
			ofstream ofs("NineAlmonds.txt");
			ofs << "NineAlmonds" << endl << "NODATA" << endl;
		}
		else {
			ofstream ofs("NineAlmonds.txt");
			ofs << "NineAlmonds" << endl;
			ofs << turnCount << endl;
			for (size_t i = 0; i < pieces.size(); i++) {
				for (size_t j = 0; j < pieces[0].size(); j++) {
					if (pieces[i][j].display != " ") {
						ofs << i << " " << j << endl;
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