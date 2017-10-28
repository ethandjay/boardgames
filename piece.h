#pragma once

#include "stdafx.h"
#include <string>
#include <iostream>
using namespace std;

enum piece_color { white, black, brown, no_color, invalid_color };
struct piece {
	string name;
	piece_color color = no_color;
	string display = " ";
};
