#include <cstring>
#include <iostream>
#include <vector>
#include <array>
using namespace std;

enum Player { NONE = 0, AI = 1, HUMAN = 2, DRAW = 3 };

struct Coord {
	int x;
	int y;
};

struct Move {
	Player player;
	Coord coord;
};

class Board {
public:
	Board() { memset(tiles, 0, sizeof(tiles[0][0]) * kSize * kSize); }
	Player checkWinner();
	bool tryMove(Player p, Coord c);
	void undoMove();
	void printBoard();
	static constexpr int kSize = 3;
private:
	vector<Move> history;
	int tiles[kSize][kSize];
};

Player Board::checkWinner() {
	int isWinnerAI, isWinnerHuman;
	for (int row = 0; row < kSize; ++row) {
		isWinnerAI = 1;
		isWinnerHuman = 1;
		for (int col = 0; col < kSize; ++col) {
			isWinnerAI &= (tiles[row][col] == Player::AI);
			isWinnerHuman &= (tiles[row][col] == Player::HUMAN);
		}
		if (isWinnerAI || isWinnerHuman) {
			return isWinnerAI ? Player::AI : Player::HUMAN;
		}
	}

	for (int col = 0; col < kSize; ++col) {
		isWinnerAI = 1;
		isWinnerHuman = 1;
		for (int row = 0; row < kSize; ++row) {
			isWinnerAI &= (tiles[row][col] == Player::AI);
			isWinnerHuman &= (tiles[row][col] == Player::HUMAN);
		}
		if (isWinnerAI || isWinnerHuman) {
			return isWinnerAI ? Player::AI : Player::HUMAN;
		}
	}

	isWinnerAI = 1, isWinnerHuman = 1;
	for (int row = 0, col = 0; row < kSize; ++row, ++col) {
		isWinnerAI &= (tiles[row][col] == Player::AI);
		isWinnerHuman &= (tiles[row][col] == Player::HUMAN);
	}
	if (isWinnerAI || isWinnerHuman) {
		return isWinnerAI ? Player::AI : Player::HUMAN;
	}

	isWinnerAI = 1, isWinnerHuman = 1;
	for (int row = 0, col = kSize - 1; row < kSize; ++row, --col) {
		isWinnerAI &= (tiles[row][col] == Player::AI);
		isWinnerHuman &= (tiles[row][col] == Player::HUMAN);
	}
	if (isWinnerAI || isWinnerHuman) {
		return isWinnerAI ? Player::AI : Player::HUMAN;
	}

	int isDraw = 1;
	for (int row = 0; row < kSize; ++row) {
		for (int col = 0; col < kSize; ++col) {
			isDraw &= (tiles[row][col] != Player::NONE);
		}
	}
	if (isDraw) {
		return Player::DRAW;
	}

	return Player::NONE;
}

bool Board::tryMove(Player p, Coord c) {
	if (tiles[c.x][c.y] == Player::NONE) {
		history.push_back({ p, c });
		tiles[c.x][c.y] = p;
		return true;
	}
	else {
		return false;
	}
}

void Board::undoMove() {
	if (history.size()) {
		Move m = history.back();
		history.pop_back();
		tiles[m.coord.x][m.coord.y] = Player::NONE;
	}
}

void Board::printBoard() {
	static char kToken[3] = { '*', 'X', 'O' };
	cout << "  ";
	for (int i = 0; i < kSize; ++i) {
		cout << i + 1 << " ";
	}
	cout << endl;
	for (int row = 0; row < kSize; ++row) {
		cout << char('A' + row) << " ";
		for (int col = 0; col < kSize; ++col) {
			cout << kToken[tiles[row][col]] << " ";
		}
		cout << endl;
	}
}

class Game {
public:
	Game() {}
	bool loop();
	void aiMove();
	int calcMove(bool isMax, Coord& move);
private:
	static constexpr int kSize = Board::kSize;
	Board board;
};

int Game::calcMove(bool isMax, Coord& bestMove) {
	static int maxVal[4] = { 0, 1, -1, 0 };
	Player winner;
	if (winner = board.checkWinner()) {
		return maxVal[winner];
	}
	int curVal = isMax ? -1000 : 1000;
	Coord move;
	Player currentPlayer = isMax ? Player::AI : Player::HUMAN;
	for (int i = 0; i < kSize; ++i) {
		for (int j = 0; j < kSize; ++j) {
			if (board.tryMove(currentPlayer, { i, j })) {
				int val = calcMove(!isMax, bestMove);
				if (isMax && val > curVal) {
					curVal = val;
					move = { i, j };
				}
				else if (!isMax && val < curVal) {
					curVal = val;
					move = { i, j };
				}
				board.undoMove();
			}
		}
	}
	bestMove = move;
	return curVal;
}

void Game::aiMove() {
	Coord bestMove;
	calcMove(true, bestMove);
	board.tryMove(Player::AI, bestMove);
}

bool Game::loop() {
	string s;
	board.printBoard();
	cout << "Enter row and column for move, Q to quit: ";
	cin >> s;
	if (s.length() == 0)
		return true;
	char ch = tolower(s[0]);
	if (ch == 'q')
		return false;
	if (s.length() < 2)
		return true;
	int num = s[1] - '0';
	if (ch >= 'a' && ch <= 'c') {
		if (num >= 1 && num <= 3) {
			if (board.tryMove(Player::HUMAN, { ch - 'a', num - 1 })) {
				if (board.checkWinner()) {
					board.printBoard();
					cout << "Game over!" << endl;
					return false;
				}
				else {
					aiMove();
					if (board.checkWinner()) {
						board.printBoard();
						cout << "Game over!" << endl;
						return false;
					}
				}
			}
		}
	}
	return true;
}

int main() {
	Game game;
	while (game.loop()) {
		;
	}
	std::cout << "Goodbye, World!\n";
	return 0;
}