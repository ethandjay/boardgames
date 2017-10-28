#pragma once

#include "stdafx.h"
#include <iostream>
#include <vector>
#include <string>
using namespace std;

enum rets {
	success = 0,
	getline_fail = 1,
	wrong_args = 2,
	stream_fail = 3,
	open_fail = 4,
	quit = 5,
	double_instantiation = 6,
	stalemate = 7,
	bad_allocation = 8,
	gen_error = 9,
	this_unreachable_variable_just_prevents_compiler_warnings_that_you_take_off_points_for = -1
};
enum indices {
	progName = 0,
	filename = 1,
	numArguments = 2,
	numArgumentsReversi = 4,
	player1 = 2,
	player2 = 3

};
//capitalized to prevent enum ambiguity
enum directions {
	Up = 0,
	Upright = 1,
	Right = 2,
	Downright = 3,
	Down = 4,
	Downleft = 5,
	Left = 6,
	Upleft = 7
};

int usage(char* s, char* o);
void lowercase(string & upper);