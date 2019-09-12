#include <cstdlib>
#include <ctime>
#include <string>
#include "Game.h"

int x[] = { 0, 1, 1, 1, 0, -1, -1, -1 };
int y[] = { -1, -1, 0, 1, 1, 1, 0, -1 };


bool Game::CheckAct(vector<Move> avaliableMoves, int row, int col) {
	for (auto& move : avaliableMoves) {
		if (move.first / 8 == row && move.first % 8 == col) {
			return true;
		}
	}
	return false;
}

int Game::ExecAct(Board& board, int row, int col, Color color) {
	int revCnt = 0;
	if (board[row][col] != Color::Empty) {
		return false;
	}
	board[row][col] = color;
	for (int w = 0; w < 8; w++) {
		int startY = row;
		int startX = col;
		if (Game::canMove(startX + x[w], startY + y[w]) &&
			board[startY + y[w]][startX + x[w]] != color &&
			board[startY + y[w]][startX + x[w]] != Color::Empty) {
			startY += y[w];
			startX += x[w];
			int count = 1;
			bool fill = false;
			while (Game::canMove(startX + x[w], startY + y[w])) {
				startY += y[w];
				startX += x[w];
				if (board[startY][startX] == color) {
					fill = true;
					break;
				}
				count++;
			}
			if (fill) {
				revCnt += count;
				startY = row;
				startX = col;
				for (int i = 0; i < count; i++) {
					startY += y[w];
					startX += x[w];
					board[startY][startX] = color;
				}
			}
		}
	}
	return revCnt;
}

bool Game::ChangePlayer(const Board& board, Color color) {
	return this->GetAvaliableAct(board, color).size() != 0;
}

int Game::CheckEnd(const Board& board) {
	if (!this->GetAvaliableAct(board, Color::Black).size() && !this->GetAvaliableAct(board, Color::White).size()) {
		this->winner = this->players[0]->num < this->players[1]->num;
		return 1;
	}
	return 0;
}

Game::Game() {
	srand(time(NULL));
}

void Game::Start() {
	this->init();

	int first = rand() % 2;
	this->currentPlayer = first;
	this->players[this->currentPlayer]->color = Color::Black;
	this->players[1 - this->currentPlayer]->color = Color::White;

	while (1) {
		this->Print();

		pair<int, int> act;
		while (1) {
			vector<Move> avaliableMoves = this->GetAvaliableAct(this->board, this->players[this->currentPlayer]->color);
			act = this->players[this->currentPlayer]->Move();

			if (this->CheckAct(avaliableMoves, act.first, act.second)) {
				break;
			}
		}
		this->execAct(act.first, act.second);

		if (this->checkEnd()) {
			this->printWinner();
			break;
		}

		if (this->ChangePlayer(this->board, this->players[1 - this->currentPlayer]->color)) {
			this->currentPlayer = 1 - this->currentPlayer;
		}

		this->round++;
	}
}

void Game::Print() {
	system("cls");

	string row = "   +---+---+---+---+---+---+---+---+";
	cout << "\n     A   B   C   D   E   F   G   H" << endl;
	cout << row << endl;
	for (int i = 0; i < 8; i++) {
		cout << " " << i + 1 << " | ";
		for (int j = 0; j < 8; j++) {
			cout << static_cast<char>(this->board[i][j]) << " | ";
		}
		if (i == 3) {
			cout << "   player0 (" << static_cast<char>(this->players[0]->color) << ": " << this->players[0]->num << ")";
		}
		if (i == 4) {
			cout << "   player1 (" << static_cast<char>(this->players[1]->color) << ": " << this->players[1]->num << ")";
		}
		cout << endl << row << endl;
	}
	cout << endl;
	cout << "Turn: " << static_cast<char>(this->players[this->currentPlayer]->color) << endl;
}

bool Game::canMove(int x, int y) {
	return x >= 0 && x < 8 && y >= 0 && y < 8;
}

void Game::init() {
	this->players[0]->Init();
	this->players[1]->Init();

	this->board = vector<vector<Color>>(8, vector<Color>(8, Color::Empty));
	this->board[3][3] = this->board[4][4] = Color::White;
	this->board[3][4] = this->board[4][3] = Color::Black;

	this->round = 0;
}

void Game::printWinner() {
	cout << "winner is player" << this->winner << " !!!" << endl;
}

void Game::execAct(int row, int col) {
	int revCnt = this->ExecAct(this->board, row, col, this->players[this->currentPlayer]->color);
	this->players[this->currentPlayer]->num += revCnt + 1;
	this->players[1 - this->currentPlayer]->num -= revCnt;
}

int Game::checkEnd() {
	return this->CheckEnd(this->board);
}