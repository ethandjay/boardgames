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

class almond_board : public GameBase {

private:
	friend ostream &operator<<(ostream &out, const almond_board &ab);

	int askFrom(unsigned int &a, unsigned int &b);
	int askTo(unsigned int, unsigned int, unsigned int &, unsigned int &);
	bool anyValid();
	bool canMove(int, int);
	int askCont(unsigned int, unsigned int, char*);
	int doMove(unsigned int, unsigned int, unsigned int, unsigned int);
	bool isTurnBased();
	virtual bool done();
	virtual bool stalemate();
	virtual int turn();
	virtual void print();
	virtual int save(bool real = true);

public:
	almond_board();

	almond_board(const almond_board &) = delete;
	almond_board &operator= (const almond_board &) = delete;

	almond_board(const almond_board &&) = delete;
	almond_board &operator= (const almond_board &&) = delete;
};

ostream &operator<<(ostream &out, const almond_board &ab);