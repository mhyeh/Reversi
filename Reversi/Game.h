#pragma once
#include "Player.h"
#include <iostream>


using namespace std;

class Game {
public:
	static bool ChangePlayer(const Board&, Color);
	static bool CheckAct(Move&, int, int);
	static int ExecAct(Board&, int, int, Color, vector<Direction>, bool);
	static Move GetAvaliableAct(const Board&, Color);
	static int CheckEnd(const Board&);

	Player* players[2];
	Board board;
	Game();
	void Print(const Move&);
	void Start();
private:
	static bool canMove(int, int);
	static vector<Direction> getDirection(const Board&, int, int, Color);
	
	int currentPlayer;
	int winner;
	bool isMove;
	vector<pair<int, Board>> history;
	vector<pair<int, Board>>::iterator it;
	void init();
	void printWinner();
	void execAct(Move&, int, int);
	int checkEnd();
};

