#pragma once
#include "macros.h"
#include "moves.h"
#include "game_board.h"

class Player
{
public:
	Player();
	Single_move choose_move(board &, std::vector<Single_move>& possible_moves);
	~Player() {}
	bool getSide() { return side; };
	void setSide(bool side) { this->side = side; };
private:
	double evaluate_move(Single_move, board&);
	double evaluate_board(board &);
	bool side;
	OpenNN::NeuralNetwork nn;
};