#pragma once
#include "macros.h"
#include "moves.h"
#include "game_board.h"

class player
{
public:
	player();
	Move choose_move(board &, std::vector<Move>& possible_moves);
	~player() {}
	bool getSide() { return side; };
	void setSide(bool side) { this->side = side; };
private:
	double evaluate_move(Move, board&);
	double evaluate_board(board &);
	bool side;
	OpenNN::NeuralNetwork nn;
};