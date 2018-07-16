#include "player.h"

player::player() : nn(neural_net_location) {}

move player::choose_move(board &b, std::vector<move>& possible_moves) {
	move best_move = possible_moves.at(0);

	double best_move_eval = evaluate_move(best_move, b);

	for (move move : possible_moves) {
		double eval = evaluate_move(move, b);
		if (side) {
			if (eval < best_move_eval) {
				best_move = move;
				best_move_eval = eval;
			}
		}
		else {
			if (eval > best_move_eval) {
				best_move = move;
				best_move_eval = eval;
			}
		}
	}
	return best_move;
}

double player::evaluate_move(move move, board &b) {
	board b_copy = board(b);

	b_copy.make_move(move);

	return evaluate_board(b_copy);
}

double player::evaluate_board(board &b) {
	OpenNN::Vector<double> inputs(12);

	for (int i = pawns; i < king; i++) {
		inputs[i] = (double)b.get_bitboard(i, false);
		inputs[6 + i] = (double)b.get_bitboard(i, true);
	}

	OpenNN::Vector<double> outputs = nn.calculate_outputs(inputs);

	return outputs[0];
}

OpenNN::NeuralNetwork create_nn_from_scratch() {
	OpenNN::Vector<size_t> architecture(5);

	architecture[0] = 12;
	architecture[1] = 2000;
	architecture[2] = 1000;
	architecture[3] = 500;
	architecture[4] = 1;

	std::cout << "Architecture done, creating NN..." << std::endl;

	return OpenNN::NeuralNetwork(architecture);
}