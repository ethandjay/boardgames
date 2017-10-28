// lab4.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "common.h"
#include "almond_board.h"
#include "GameBase.h"
#include "piece.h"
#include <string>
#include <memory>
#include <iostream>
using namespace std;

int main(int argc, char * argv[])
{
	try {
		GameBase::gameCheck(argc, argv);
		return GameBase::instance()->play();
	}
	catch (bad_alloc &ba) {
		cout << "bad allocation: " << ba.what() << endl;
		return bad_allocation;
	}
	catch (exception &e) {
		if (e.what() == string("Double instantiation")) {
			cout << e.what() << endl;
			return double_instantiation;
		}
		else if (e.what() == string("Bad args")) {
			cout << e.what() << endl;
			return usage("lab4.exe", "[NineAlmonds, MagicSqaure, Reversi <Player 1> <Player 2>]");
		}
		else {
			cout << e.what() << endl;
			return gen_error;
		}
	}
}

