
#include "stdafx.h"
#include "common.h"
#include "almond_board.h"
#include "MagicSquareGame.h"
#include "Reversi.h"
#include "GameBase.h"
#include <iostream>
#include <memory>
#include <exception>
#include <vector>
using namespace std;

shared_ptr<GameBase> GameBase::sp = nullptr;

shared_ptr<GameBase> GameBase::instance() {
	if (GameBase::sp != nullptr) {
		return GameBase::sp;
	}
	else {
		throw exception("Static instance of GameBase is null");
	}
}

GameBase::GameBase(int y, int x)
	: pieces(0), board_height(y), board_width(x), minTurns(0), turnCount(0)
{}

//turn based games
GameBase::GameBase(int y, int x, int m)
	: pieces(0), board_height(y), board_width(x), minTurns(m), turnCount(0)
{}

void GameBase::gameCheck(int gargc, char** gargv) {
	if (GameBase::sp == nullptr) {
		if (gargc == numArguments && gargv[filename] == string("NineAlmonds")) {
			almond_board* ab = new almond_board();
			GameBase::sp = shared_ptr<GameBase> (ab);
		}
		else if (gargc == numArguments && gargv[filename] == string("MagicSquare")) {
			MagicSquareGame* msg = new MagicSquareGame();
			GameBase::sp = shared_ptr<GameBase>(msg);
		}
		else if (gargc == numArgumentsReversi && gargv[filename] == string("Reversi")) {
			Reversi* r = new Reversi(gargv[player1], gargv[player2]);
			GameBase::sp = shared_ptr<GameBase>(r);
		}
		else {
			throw exception("Bad args");
		}
	}
	else {
		throw exception("Double instantiation");
	}
}

int GameBase::prompt(unsigned int &a, unsigned int &b) {
	string in;
	cin >> in;
	cout << endl;
	if (in == "quit") {
		throw quit;
	}
	else if (isValidCoord(in)) {
		a = in[2] - '0';
		b = in[0] - '0';
		return success;
	}
	cout << "Invalid coordinate, try again: ";
	prompt(a, b);
	return this_unreachable_variable_just_prevents_compiler_warnings_that_you_take_off_points_for;
}

bool GameBase::isValidCoord(string a) {
	//Check formatting
	if (a.length() != 3)
	{
		return false;
	}
	int x = a[0] - '0';
	int y = a[2] - '0';
	if (!isdigit(a[0]) || x < 0 || x > board_width - 1) {
		return false;
	}
	if (!isdigit(a[2]) || y < 0 || y > board_height - 1) {
		return false;
	}
	if (a[1] != ',') {
		return false;
	}
	return true;
}


int GameBase::play() {
	cout << endl;
	print();
	while (true) {
		try {
			turn();
			turnCount++;
		}
		catch (rets err) {
			if (err == quit) {
				cout << "Would you like to save? (y/n): ";
				string ans = "";
				cin >> ans;
				cout << endl;
				if (ans == "y") {
					GameBase::sp->save();
					cout << "Game saved, you quit after " << turnCount << " turns." << endl;
					return quit;
				}
				else if (ans == "n") {
					GameBase::sp->save(false);
					cout << "You quit after " << turnCount << " turns." << endl;
					return quit;
				}
				else {
					cout << "Invalid input" << endl;
				}
			}
		}
		if (done()) {
			GameBase::sp->save(false);
			if (typeid(*this).name() == string("class Reversi")) {
				cout << sp->winner;
			}
			else {
				cout << "You";
			}
			cout << " won! ";
			if (isTurnBased()) {
				cout << "It took" << turnCount << " turns. ";
				if (turnCount > 4) {
					cout << "Try to beat " << minTurns <<  " turns." << endl;
				}
				else {
					cout << "This is the fewest amount of turns in which this puzzle can be completed, congrats!" << endl;
				}
			}
			cout << endl;
			return success;
		}
		if (stalemate()) {
			cout << "Oops! Stalemate";
			if (isTurnBased()) {
				cout << " after " << turnCount << " turns. " << endl;
			}
			return rets::stalemate;
		}
	}
}