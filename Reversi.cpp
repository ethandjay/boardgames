#include "stdafx.h"
#include "common.h"
#include "piece.h"
#include "Reversi.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

Reversi::Reversi(string p1, string p2) : GameBase(8, 8) {
	pieces.resize(board_height, vector<piece>(board_width));
	for (int i = 0; i < board_height; i++) {
		for (int j = 0; j < board_width; j++) {
			pieces[i][j].name = " ";
			pieces[i][j].color = no_color;
			pieces[i][j].display = " ";
		}
	}
	try {
		longestDisplay = 1;
		players[black] = p1;
		players[white] = p2;
		ifstream ifs("Reversi.txt");
		if (!ifs.is_open()) {
			throw stream_fail;
		}
		string line;
		getline(ifs, line);
		getline(ifs, line);
		// starting pieces
		if (line == "NODATA") {
			pieces[board_height / 2 - 1][board_width / 2 - 1].name = "X";
			pieces[board_height / 2 - 1][board_width / 2 - 1].display = "X";
			pieces[board_height / 2 - 1][board_width / 2 - 1].color = black;

			pieces[board_height / 2][board_width / 2].name = "X";
			pieces[board_height / 2][board_width / 2].display = "X";
			pieces[board_height / 2][board_width / 2].color = black;

			pieces[board_height / 2 - 1][board_width / 2].name = "O";
			pieces[board_height / 2 - 1][board_width / 2].display = "O";
			pieces[board_height / 2 - 1][board_width / 2].color = white;

			pieces[board_height / 2][board_width / 2 - 1].name = "O";
			pieces[board_height / 2][board_width / 2 - 1].display = "O";
			pieces[board_height / 2][board_width / 2 - 1].color = white;

			whoseTurn = black;
		}
		else {
			istringstream iss(line);
			iss >> turnCount;
			getline(ifs, line);
			istringstream iss1(line);
			int whoseTurnInt;
			iss1 >> whoseTurnInt;
			whoseTurn = (piece_color)whoseTurnInt;
			while (!ifs.eof()) {
				getline(ifs, line);
				istringstream iss(line);
				int i, j, color;
				iss >> color >> i >> j;
				string coord;
				coord += to_string(j);
				coord.push_back(',');
				coord += to_string(i);
				if (!isValidCoord(coord) || (color != black && color != white)) {
					continue;
				}
				if (color == black) {
					pieces[i][j].display = "X";
					pieces[i][j].name = "X";
					pieces[i][j].color = black;
				}
				else {
					pieces[i][j].display = "O";
					pieces[i][j].name = "O";
					pieces[i][j].color = white;
				}


				
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

bool Reversi::stalemate() {
	bool evenCount = false;
	int bCount = 0;
	int wCount = 0;
	for (size_t i = 0; i < pieces.size(); i++) {
		for (size_t j = 0; j < pieces.size(); j++) {
			if (pieces[i][j].color == black) {
				bCount++;
			}
			if (pieces[i][j].color == white) {
				wCount++;
			}
		}
	}
	if (wCount == bCount) { evenCount = true; }
	bool full = false;
	if (wCount + bCount == 64) { full = true; }

	bool canMove = false;
	//check all spots for possible moves
	for (int i = 0; i < board_height; i++) {
		for (int j = 0; j < board_width; j++) {
			if (!validSpotFor(black, i, j).empty() || !validSpotFor(white, i, j).empty()) {
				canMove = true;
			}
		}
	}

	return (evenCount && (full || !canMove));


}
bool Reversi::done() {
	bool same = true;
	piece_color firstGuyColor;
	if (pieces[0][0].color == no_color) {
		same = false;
	}
	else {
		firstGuyColor = pieces[0][0].color;
	}
	int bCount = 0;
	int wCount = 0;
	// check if all pieces match, incr color counts
	for (int i = 0; i < board_height; i++) {
		for (int j = 0; j < board_width; j++) {
			if (same == true && pieces[i][j].color != firstGuyColor) {
				same = false;
			}
			if (pieces[i][j].color == black) {
				bCount++;
			}
			if (pieces[i][j].color == white) {
				wCount++;
			}
		}
	}
	if (same) {
		winner = players[firstGuyColor];
		return true; 
	}
	bool unevenCount = false;
	bool full = false;
	if (wCount != bCount) { unevenCount = true; }
	if (wCount + bCount == 64) { full = true; }
	
	bool canMove = false;
	// check for possible moves
	for (int i = 0; i < board_height; i++) {
		for (int j = 0; j < board_width; j++) {
			if (!validSpotFor(black, i, j).empty() || !validSpotFor(white, i, j).empty()) {
				canMove = true;
			}
		}
	}

	if (unevenCount && (full || !canMove)) {
		if (wCount > bCount) {
			winner = players[white];
		}
		else {
			winner = players[black];
		}
		return true;
	}
	return false;

}

vector<directions> Reversi::validSpotFor(piece_color col, int a, int b) {
	vector<directions> dirs;
	
	if (pieces[a][b].name != " ") {
		return dirs;
	}
	piece_color diffCol;
	if (col == black) {
		diffCol = white;
	}
	else {
		diffCol = black;
	}

	// Each case walks path until it hits a piece of the same color, and then walks back checking pieces along the way for opposite coloring
	// Special case: same color piece is next to source piece
	//
	// Returns a vector filled with valid path directions for a given piece


	//up
	int i = a+1;
	int j = b;
	bool allDiff = true;
	bool hit = false;
	while (i < board_height) {
		if (i == a+1 && pieces[i][j].color == col){
			allDiff = false;
			break;
		}
		if (pieces[i][j].color == col) {
			hit = true;
			i--;
			while (i > a) {
				if (pieces[i][j].color != diffCol) {
					allDiff = false;
				}
				i--;
			}
			break;
		}
		i++;
	} if (!hit) { allDiff = false; }
	if (allDiff) { dirs.push_back(Up);  }

	//down
	 i = a - 1;
	 j = b;
	allDiff = true;
	hit = false;
	while (i >= 0) {
		if (i == a - 1 && pieces[i][j].color == col) {
			allDiff = false;
			break;
		}
		if (pieces[i][j].color == col) {
			hit = true;
			i++;
			while (i < a) {
				if (pieces[i][j].color != diffCol) {
					allDiff = false;
				}
				i++;
			}
			break;
		}
		i--;
	} if (!hit) { allDiff = false; }
	if (allDiff) { dirs.push_back(Down); }

	//right
	 i = a;
	 j = b + 1;
	allDiff = true;
	hit = false;
	while (j < board_width) {
		if (j == b + 1 && pieces[i][j].color == col) {
			allDiff = false;
			break;
		}
		if (pieces[i][j].color == col) {
			hit = true;
			j--;
			while (j > b) {
				if (pieces[i][j].color != diffCol) {
					allDiff = false;
				}
				j--;
			}
			break;
		}
		j++;
	} if (!hit) { allDiff = false; }
	if (allDiff) { dirs.push_back(Right); }

	//left
	 i = a;
	 j = b - 1;
	allDiff = true;
	hit = false;
	while (j >= 0) {
		if (j == b - 1 && pieces[i][j].color == col) {
			allDiff = false;
			break;
		}
		if (pieces[i][j].color == col) {
			hit = true;
			j++;
			while (j < b) {
				if (pieces[i][j].color != diffCol) {
					allDiff = false;
				}
				j++;
			}
			break;
		}
		j--;
	}  if (!hit) { allDiff = false; }
	if (allDiff) { dirs.push_back(Left);; }

	//upright
	 i = a + 1;
	 j = b + 1;
	allDiff = true;
	hit = false;
	while (i < board_height && j < board_width) {
		if (j == b + 1 && i == a + 1 && pieces[i][j].color == col) {
			allDiff = false;
			break;
		}
		if (pieces[i][j].color == col) {
			hit = true;
			j--;
			i--;
			while (i > a && j > b) {
				if (pieces[i][j].color != diffCol) {
					allDiff = false;
				}
				j--;
				i--;
			}
			break;
		}
		j++;
		i++;
	}  if (!hit) { allDiff = false; }
	if (allDiff) { dirs.push_back(Upright); }

	//downleft
	 i = a - 1;
	 j = b - 1;
	allDiff = true;
	hit = false;
	while (i >= 0 && j >= 0) {
		if (j == b - 1 && i == a - 1 && pieces[i][j].color == col) {
			allDiff = false;
			break;
		}
		if (pieces[i][j].color == col) {
			hit = true;
			j++;
			i++;
			while (i < a && j < b) {
				if (pieces[i][j].color != diffCol) {
					allDiff = false;
				}
				j++;
				i++;
			}
			break;
		}
		j--;
		i--;
	}  if (!hit) { allDiff = false; }
	if (allDiff) { dirs.push_back(Downleft); }

	//downright
	 i = a - 1;
	 j = b + 1;
	allDiff = true;
	hit = false;
	while (i >= 0 && j < board_width) {
		if (j == b + 1 && i == a - 1 && pieces[i][j].color == col) {
			allDiff = false;
			break;
		}
		if (pieces[i][j].color == col) {
			hit = true;
			j--;
			i++;
			while (i < a && j > b) {
				if (pieces[i][j].color != diffCol) {
					allDiff = false;
				}
				j--;
				i++;
			}
			break;
		}
		j++;
		i--;
	}  if (!hit) { allDiff = false; }
	if (allDiff) { dirs.push_back(Downright); }

	//upleft
	 i = a + 1;
	 j = b - 1;
	allDiff = true;
	hit = false;
	while (i < board_height && j >= 0) {
		if (j == b - 1 && i == a + 1 && pieces[i][j].color == col) {
			allDiff = false;
			break;
		}
		if (pieces[i][j].color == col) {
			hit = true;
			j++;
			i--;
			while (i > a && j < b) {
				if (pieces[i][j].color != diffCol) {
					allDiff = false;
				}
				j++;
				i--;
			}
			break;
		}
		j--;
		i++;
	}  if (!hit) { allDiff = false; }
	if (allDiff) { dirs.push_back(Upleft); }

	return dirs;
}

int Reversi::doMove(unsigned int i, unsigned int j) {


	for (directions d : validSpotFor(whoseTurn, i, j)) {
		int a, b;
		
		pieces[i][j].color = whoseTurn;

		// Each case walks path until it hits piece of its own color, and then recolors all of the pieces on the walk back

		
		switch (d) {

			case Up:
				a = i+1, b = j;
				while (pieces[a][b].color != whoseTurn) {
					pieces[a][b].color = whoseTurn;
					a++;
				}
				break;

			case Right:
				a = i, b = j+1;
				while (pieces[a][b].color != whoseTurn) {
					pieces[a][b].color = whoseTurn;
					b++;
				}
				break;

			case Down:
				a = i - 1, b = j;
				while (pieces[a][b].color != whoseTurn) {
					pieces[a][b].color = whoseTurn;
					a--;
				}
				break;

			case Left:
				a = i, b = j - 1;
				while (pieces[a][b].color != whoseTurn) {
					pieces[a][b].color = whoseTurn;
					b--;
				}
				break;

			case Upright:
				a = i+1, b = j + 1;
				while (pieces[a][b].color != whoseTurn) {
					pieces[a][b].color = whoseTurn;
					b++;
					a++;
				}
				break;

			case Upleft:
				a = i + 1, b = j - 1;
				while (pieces[a][b].color != whoseTurn) {
					pieces[a][b].color = whoseTurn;
					b--;
					a++;
				}
				break;

			case Downleft:
				a = i - 1, b = j - 1;
				while (pieces[a][b].color != whoseTurn) {
					pieces[a][b].color = whoseTurn;
					b--;
					a--;
				}
				break;

			case Downright:
				a = i - 1, b = j + 1;
				while (pieces[a][b].color != whoseTurn) {
					pieces[a][b].color = whoseTurn;
					b++;
					a--;
				}
				break;
		}
	}
	for (int i = 0; i < board_height; i++) {
		for (int j = 0; j < board_width; j++) {
			if (pieces[i][j].color == white) {
				pieces[i][j].display = "O";
				pieces[i][j].name = "O";
			}
			else if (pieces[i][j].color == black) {
				pieces[i][j].display = "X";
				pieces[i][j].name = "X";
			}
		}
	}

	return success;
}


int Reversi::turn() {
	piece_color diffCol;
	if (whoseTurn == black) {
		diffCol = white;
	}
	else { diffCol = black; };


	bool iCanMove = false;
	for (int i = 0; i < board_height; i++) {
		for (int j = 0; j < board_width; j++) {
			if (!validSpotFor(whoseTurn, i, j).empty()) {
				iCanMove = true;
			}
		}
	}
	bool youCanMove = false;
	for (int i = 0; i < board_height; i++) {
		for (int j = 0; j < board_width; j++) {
			if (!validSpotFor(diffCol, i, j).empty()) {
				youCanMove = true;
			}
		}
	}


	if (!iCanMove && youCanMove) {
		cout << players[whoseTurn] << " can't move, skipping..." << endl;
		whoseTurn = diffCol;
		return success;
	}




	unsigned int x;
	unsigned int y;
	askTo(x, y);

	doMove(x, y);



	cout << *this << endl;
	char move[256] = {};
	char* col;
	if (whoseTurn == black) {
		col = "Black";
	}
	else { col = "White";  }
	sprintf_s(move, sizeof(move), "%s to %d,%d", col, y, x);
	cout << move << endl << endl;

	if (whoseTurn == black) {
		whoseTurn = white;
	}
	else {
		whoseTurn = black;
	}
	return success;
}

int Reversi::askTo(unsigned int &x, unsigned int &y) {
	cout << players[whoseTurn] << ": enter in the coordinate of where you want to move the pieces in the format x,y or \"quit\" to quit the game" << endl << endl;

	prompt(x, y);

	if (validSpotFor(whoseTurn, x, y).empty()) {
		cout << "Invalid spot. ";
		askTo(x, y);
	}
	return success;
}

ostream &operator<<(ostream &out, const Reversi &r) {

	for (int i = r.board_height - 1; i >= 0; i--) {
		out << i << string(r.longestDisplay, ' ');
		for (int j = 0; j < r.board_width; j++) {
			string holder(r.longestDisplay, ' ');
			for (size_t k = 0; k < r.pieces[i][j].display.length(); k++) {
				holder.at(k) = r.pieces[i][j].display[k];
			}



			out << holder << " ";
		}
		out << endl << endl;
	}
	out << "X" << string(r.longestDisplay, ' ');
	for (int i = 0; i < r.board_width; i++) {
		cout << i << string(r.longestDisplay, ' ');
	}
	out << endl;
	return out;
}
bool Reversi::isTurnBased() { return false; }
void Reversi::print() {
	cout << *this << endl;
}
int Reversi::save(bool real) {
	try {
		if (!real) {
			ofstream ofs("Reversi.txt");
			ofs << "Reversi" << endl << "NODATA" << endl;
		}
		else {
			ofstream ofs("Reversi.txt");
			ofs << "Reversi" << endl;
			ofs << turnCount << endl;
			ofs << whoseTurn << endl;
			for (size_t i = 0; i < pieces.size(); i++) {
				for (size_t j = 0; j < pieces[0].size(); j++) {
					if (pieces[i][j].display != " ") {
						ofs << pieces[i][j].color << " " << i << " " << j << endl;
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