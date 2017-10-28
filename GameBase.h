#pragma once

#include "stdafx.h"
#include "common.h"
#include "piece.h"
#include <string>
#include <memory>
#include <iostream>
#include <vector>
using namespace std;



class GameBase {
protected:
	int board_height;
	int board_width;
	vector<vector<piece>> pieces;
	int longestDisplay;
	int minTurns;
	int turnCount;
	string winner;
	static shared_ptr<GameBase> sp;

	virtual bool isTurnBased() = 0;
	virtual int prompt(unsigned int &, unsigned int &);
	virtual bool isValidCoord(string);
	virtual void print() = 0;
	virtual bool done() = 0;
	virtual bool stalemate() = 0;
	virtual int turn() = 0;
	virtual int save(bool real = true) = 0;
public:
	static shared_ptr<GameBase> instance();
	GameBase(int, int);

	//turn based
	GameBase(int, int, int);


	static void gameCheck(int, char**);
	int play();

	GameBase(const GameBase &) = delete;
	GameBase &operator= (const GameBase &) = delete;
	GameBase(const GameBase &&) = delete;
	GameBase &operator= (const GameBase &&) = delete;
};