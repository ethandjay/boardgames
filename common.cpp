#include "stdafx.h"
#include <iostream>
#include "common.h"
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
using namespace std;

int usage(char* s, char* o) {
	printf("usage: %s %s", s, o);
	return wrong_args;
};

void lowercase(string & upper) {
	int ASCIIconversion = 32;
	for (size_t i = 0; i < upper.length(); i++) {
		if (int(upper[i]) >= 'a' && int(upper[i]) <= 'z') {
			int upnum = int(upper[i]) + ASCIIconversion;
			char upchar = char(upnum);
			upper[i] = upchar;
		}
	}
};