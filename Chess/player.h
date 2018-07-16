#pragma once
#include "macros.h"
#include "moves.h"
#include "game_board.h"

class player
{
public:
	player();
	move choose_move(board &, std::vector<move>& possible_moves);
	~player() {}
	bool getSide() { return side; };
	void setSide(bool side) { this->side = side; };
private:
	double evaluate_move(move, board&);
	double evaluate_board(board &);
	bool side;
	OpenNN::NeuralNetwork nn;
};