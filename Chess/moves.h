#pragma once
#include "macros.h"

struct move{
	bool player;
	int piece_type;
	int starting_position;
	int ending_position;

	int special_move_flag; // 0 Normal move - 1 Promotion - 2 En Passant - 3 Castling
	int promotion_type; // 1 Rook - 2 Knight - 3 Bishop - 4 Queen

	std::string to_long_algebraic_notation();

	move(bool player, int piece_type, int starting_position, int ending_position, 
		int special_move_flag, int promotion_type) : player(player), piece_type(piece_type), 
		starting_position(starting_position), ending_position(ending_position), 
		special_move_flag(special_move_flag), promotion_type(promotion_type) {}
	std::string to_string();
};