#pragma once

#pragma once

#include "stdafx.h"
#include "piece.h"
#include "GameBase.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

class Reversi : public GameBase {
private:
	string players[2];
	piece_color whoseTurn;

	friend ostream &operator<<(ostream &out, const Reversi &r);
	bool isTurnBased();
	int doMove(unsigned int, unsigned int);
	int askTo(unsigned int &, unsigned int &);
	vector<directions> validSpotFor(piece_color, int, int);
	virtual void print();
	virtual bool done();
	virtual bool stalemate();
	virtual int turn();
	virtual int save(bool real = true);



public:
	Reversi(string, string);
	Reversi(const Reversi &) = delete;
	Reversi &operator= (const Reversi &) = delete;
	Reversi(const Reversi &&) = delete;
	Reversi &operator= (const Reversi &&) = delete;
};