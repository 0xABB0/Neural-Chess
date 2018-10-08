#include "moves.h"

std::string Move::to_long_algebraic_notation()
{
	std::string to_ret = "";
	to_ret += (starting_position % 8) + 'a';
	to_ret += (starting_position / 8) + '1';
	to_ret += (ending_position % 8) + 'a';
	to_ret += (ending_position / 8) + '1';
	if (special_move_flag == 2) {
		to_ret += promotion_type == 1 ? 'r' : promotion_type == 2 ? 'n' : promotion_type == 3 ? 'b' : 'q';
	}
	return to_ret;
}

std::string Move::to_string()
{
	std::string to_ret = "";
	std::string player_color = player ? "black" : "white";
	to_ret += "player " + player_color;
	to_ret += " piece_type " + std::to_string(piece_type);
	to_ret += " starting pos " + std::to_string(starting_position);
	to_ret += " ending pos " + std::to_string(ending_position);
	return to_ret;
}
