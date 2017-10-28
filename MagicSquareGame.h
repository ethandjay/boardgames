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

class MagicSquareGame : public GameBase {
private:

	vector<piece> deck;
	friend ostream &operator<<(ostream &out, const MagicSquareGame &msg);
	int askWhich(unsigned int &);
	bool isTurnBased();
	int askTo(unsigned int, unsigned int &, unsigned int &);
	virtual int prompt(unsigned int &);
	virtual bool isValidPiece(string);
	virtual void print();
	virtual bool done();
	virtual bool stalemate();
	virtual int turn();
	virtual int save(bool real = true);



public:
	MagicSquareGame();
	MagicSquareGame(const MagicSquareGame &) = delete;
	MagicSquareGame &operator= (const MagicSquareGame &) = delete;
	MagicSquareGame(const MagicSquareGame &&) = delete;
	MagicSquareGame &operator= (const MagicSquareGame &&) = delete;
};