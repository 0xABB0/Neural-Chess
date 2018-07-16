#include "game_board.h"

int shift[2] = { 1, -1 };

using namespace std;

BB board::get_pawn_moves_as_bitboard(unsigned short position, bool is_black) {
	/*
	* fuck en_passants
	*/

	BB pos = (1i64 << (int)position);
	BB attack;

	if (!is_black) {
		attack = (((column_zero[0] & pos) << 7) | ((column_zero[7] & pos) << 9)) & occupied_array[is_black];
	}
	else {
		attack = (((column_zero[0] & pos) >> 9) | ((column_zero[7] & pos) >> 7)) & occupied_array[!is_black];
	}

	BB one_forward = (is_black ? pos >> 8 : pos << 8) & ~occupied;
	if (pos & starting_positions[is_black][0]) {
		BB two_forward = (is_black ? one_forward >> 8 : one_forward << 8) & ~occupied;
		return one_forward | two_forward | attack;
	}

	return one_forward | attack;
}

BB board::get_rook_moves_as_bitboard(unsigned short pos, bool is_black) {
	BB rook_attacks = sliders.RookAttacks(occupied, (int)pos);
	return rook_attacks & ~(occupied_array[is_black]);
}

BB board::get_knight_moves_as_bitboard(unsigned short pos, bool is_black) {
	return Knight_Lookup_Table[pos] & ~(occupied_array[is_black]);
}

BB board::get_bishop_moves_as_bitboard(unsigned short pos, bool is_black) {
	return sliders.BishopAttacks(occupied, pos) & ~occupied_array[is_black ? 1 : 0];
}

BB board::get_queen_moves_as_bitboard(unsigned short position, bool isBlack) {
	if (false) {
		return get_rook_moves_as_bitboard(position, isBlack) | get_bishop_moves_as_bitboard(position, isBlack);
	}
	else {
		return sliders.QueenAttacks(occupied, position) & ~occupied_array[(isBlack ? 1 : 0)];
	}
}

BB board::get_king_moves_as_bitboard(unsigned short position, bool is_black) {
	return King_Lookup_Table[position] & ~(occupied_array[is_black]);
}

void board::make_move(move move) {
	if (move.special_move_flag == 0 || move.special_move_flag == 1) {
		BB end = 1i64 << move.ending_position;
		BB start = 1i64 << move.starting_position;
		short int piece_type_ending = support_game_board[move.ending_position];

		game_board[move.player][move.piece_type] ^= start;

		occupied_array[move.player] ^= start ^ end;

		if (piece_type_ending != -1) {
			game_board[!move.player][piece_type_ending] ^= end;
			occupied_array[!move.player] ^= end;
		}

		support_game_board[move.starting_position] = -1;

		occupied = occupied_array[0] | occupied_array[1];
		if (move.special_move_flag == 0) {
			support_game_board[move.ending_position] = move.piece_type;
			game_board[move.player][move.piece_type] ^= end;
		}
		else {
			support_game_board[move.ending_position] = move.promotion_type;
			game_board[move.player][move.promotion_type] ^= end;
		}
	}
	else if (move.special_move_flag == 2) {}
	else if (move.special_move_flag == 3) {
		make_move(move(move.player, king, move.starting_position, move.ending_position, 0, 0));
		if (!move.player) {
			if (move.ending_position == 7) {
				//short castle
				make_move(move(white, rooks, 7, 5, 0, 0));
			}
			else if (move.ending_position == 3) {
				//long castle
				make_move(move(white, rooks, 0, 3, 0, 0));
			}
		}
		else {
			if (move.ending_position == 63) {
				//short castle
				make_move(move(black, rooks, 64, 62, 0, 0));
			}
			else if (move.ending_position == 57) {
				//long castle
				make_move(move(white, rooks, 56, 59, 0, 0));
			}
		}
	}
}

int board::promote_pawn(bool player, int pos) {
	std::cout << "Promote to what? Q N B R" << std::endl;
	char promote = getchar();
	BB place = 1i64 << pos;
	switch (promote) {
	case 'Q':
		return queens;
	case 'R':
		return rooks;
	case 'B':
		return bishops;
	case 'N':
		return knights;
	default:
		return 0;
		break;
	}
}

BB board::get_all_moves_as_bitboards(bool color) {
	BB totalMoves = 0i64;
	int piece;
	for (int loc = 0; loc < 64; loc++) {
		if (occupied_array[color] & (1i64 << loc)) {
			piece = support_game_board[loc];
			switch (piece) {
			case 0:
				totalMoves |= get_pawn_moves_as_bitboard(loc, color);
				break;
			case 1:
				totalMoves |= get_rook_moves_as_bitboard(loc, color);
				break;
			case 2:
				totalMoves |= get_knight_moves_as_bitboard(loc, color);
				break;
			case 3:
				totalMoves |= get_bishop_moves_as_bitboard(loc, color);
				break;
			case 4:
				totalMoves |= get_queen_moves_as_bitboard(loc, color);
				break;
			case 5:
				totalMoves |= get_king_moves_as_bitboard(loc, color);
				break;
			default:
				break;
			}
		}
	}
	return totalMoves;
}

board::board(const board& to_copy) {
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 6; j++) {
			game_board[i][j] = to_copy.game_board[i][j];
		}
	}

	for (int i = 0; i < 2; ++i) {
		occupied_array[i] = to_copy.occupied_array[i];
	}

	for (int i = 0; i < 64; i++) {
		support_game_board[i] = to_copy.support_game_board[i];
	}

	occupied = to_copy.occupied;

	can_black_castle_long = to_copy.can_black_castle_long;
	can_black_castle_short = to_copy.can_black_castle_short;
	can_white_castle_long = to_copy.can_white_castle_long;
	can_white_castle_short = to_copy.can_white_castle_short;

	sliders = to_copy.sliders;
}

bool board::check(bool color) {
	if ((game_board[color][king] & get_all_moves_as_bitboards(!color)) != 0)
		return true;
	return false;
}

bool board::checkmate(bool color)
{
	std::vector<move> all_possible_moves = get_all_moves(color);
	for (move move : all_possible_moves) {
		board copy(*this);
		copy.make_move(move);
		if (!copy.check(color)) {
			return false;
		}
	}
	return true;
}

board::board() {
	sliders.Initialize();
}

void board::print_board_letters() {
	for (int row = 7; row >= 0; row--) {
		std::string row_filled = "";
		for (int col = 0; col < 8; col++) {
			BB location = 1i64 << ((row * 8) + col);
			if (occupied & location) {
				row_filled += piece_initials[occupied_array[white] & location ? 0 : 1][support_game_board[((row * 8) + col)]];
			}
			else {
				row_filled.append("0");
			}
		}
		cout << row_filled << endl;
	}
	cout << endl;
}

void board::print_occupied() {
	print_bitboard(occupied);
}

void board::init_standard_game() {
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 8; j++) {
			game_board[i][j] = starting_positions[i][j];
		}
	}

	for (int i = 0; i < 64; i++) {
		support_game_board[i] = support_starting_positions[i];
	}

	occupied_array[0] = white_start_all;
	occupied_array[1] = black_start_all;
	occupied = occupied_array[0] | occupied_array[1];

	can_black_castle_long = true;
	can_black_castle_short = true;
	can_white_castle_long = true;
	can_white_castle_short = true;
}

void print_bitboard(const BB bitboard) {
	for (int row = 7; row >= 0; --row) {
		for (int col = 0; col <= 7; ++col) {
			BB mask = 1i64 << (BB)((row * 8) + col);
			if (bitboard & mask) {
				std::cout << "1 ";
			}
			else {
				std::cout << "0 ";
			}
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

vector<move> board::get_pawns_moves(bool player, int position) {
	std::vector<move> possible_moves;
	BB bitboard_move = get_pawn_moves_as_bitboard(position, player);
	for (int i = 0; i < 64; i++) {
		BB shifted_moves = bitboard_move >> (BB)i;
		if (shifted_moves & (BB)1) {
			if (player ? i < 8 : i > 55) {
				for (int promotion = 1; i < 5; i++) {
					possible_moves.push_back(move(player, pawns, position, i, 1, promotion));
				}
			}
			else {
				possible_moves.push_back(move(player, pawns, position, i, 0, 0));
			}
		}
	}
	return possible_moves;
}

vector<move> board::get_bishops_moves(bool player, int position) {
	std::vector<move> possible_moves;
	BB bitboard_move = get_bishop_moves_as_bitboard(position, player);
	for (int i = 0; i < 64; i++) {
		BB shifted_moves = bitboard_move >> i;
		if (shifted_moves & 1) {
			possible_moves.push_back(move(player, bishops, position, i, 0, 0));
		}
	}
	return possible_moves;
}

vector<move> board::get_rooks_moves(bool player, int position) {
	std::vector<move> possible_moves;
	BB bitboard_move = get_rook_moves_as_bitboard(position, player);
	for (int i = 0; i < 64; i++) {
		BB shifted_moves = bitboard_move >> i;
		if (shifted_moves & 1) {
			possible_moves.push_back(move(player, rooks, position, i, 0, 0));
		}
	}
	return possible_moves;
}

vector<move> board::get_queens_moves(bool player, int position) {
	std::vector<move> possible_moves;
	BB bitboard_move = get_queen_moves_as_bitboard(position, player);
	for (int i = 0; i < 64; i++) {
		BB shifted_moves = bitboard_move >> i;
		if (shifted_moves & 1) {
			possible_moves.push_back(move(player, queens, position, i, 0, 0));
		}
	}
	return possible_moves;
}

vector<move> board::get_king_moves(bool player, int position) {
	std::vector<move> possible_moves;
	BB bitboard_move = get_king_moves_as_bitboard(position, player);
	if (player) {
		if (can_black_castle_long && (get_all_moves_as_bitboards(!player) & (~occupied & (3i64 << 61)))) {
			possible_moves.push_back(move(player, king, 60, 62, 3, 0));
		}
		if (can_black_castle_short && (get_all_moves_as_bitboards(!player) & (~occupied & (3i64 << 58)))) {
			possible_moves.push_back(move(player, king, 60, 57, 3, 0));
		}
	}
	else {
		if (can_white_castle_long && (get_all_moves_as_bitboards(!player) & (~occupied & (3i64 << 2)))) {
			possible_moves.push_back(move(player, king, 4, 2, 3, 0));
		}
		if (can_white_castle_short && (get_all_moves_as_bitboards(!player) & (~occupied & (3i64 << 5)))) {

			possible_moves.push_back(move(player, king, 4, 6, 3, 0));
		}
	}

	for (int i = 0; i < 64; i++) {
		BB shifted_moves = bitboard_move >> i;
		if (shifted_moves & 1) {
			possible_moves.push_back(move(player, king, position, i, 0, 0));
		}
	}
	return possible_moves;
}

vector<move> board::get_knights_moves(bool player, int position) {
	std::vector<move> possible_moves;
	BB bitboard_move = get_knight_moves_as_bitboard(position, player);
	for (int i = 0; i < 64; i++) {
		BB shifted_moves = bitboard_move >> i;
		if (shifted_moves & 1) {
			possible_moves.push_back(move(player, knights, position, i, 0, 0));
		}
	}
	return possible_moves;
}

vector<move> board::get_all_moves(bool player) {
	vector<move> all_possible_moves;
	for (int i = 0; i < 64; i++) {
		if (occupied_array[player] & (1i64 << i)) {
			int piece_type = support_game_board[i];
			switch (piece_type) {
			case pawns: {
				std::vector<move> a = get_pawns_moves(player, i);
				if (a.size() != 0) {
					all_possible_moves.insert(all_possible_moves.end(), a.begin(), a.end());
				}
				break;
			}
			case rooks: {
				std::vector<move> a = get_rooks_moves(player, i);
				if (a.size() != 0) {
					all_possible_moves.insert(all_possible_moves.end(), a.begin(), a.end());
				}
				break;
			}
			case knights: {
				std::vector<move> a = get_knights_moves(player, i);
				if (a.size() != 0) {
					all_possible_moves.insert(all_possible_moves.end(), a.begin(), a.end());
				}
				break;
			}
			case bishops: {
				std::vector<move> a = get_bishops_moves(player, i);
				if (a.size() != 0) {
					all_possible_moves.insert(all_possible_moves.end(), a.begin(), a.end());
				}
				break;
			}
			case queens: {
				std::vector<move> a = get_queens_moves(player, i);
				if (a.size() != 0) {
					all_possible_moves.insert(all_possible_moves.end(), a.begin(), a.end());
				}
				break;
			}
			case king: {
				std::vector<move> a = get_king_moves(player, i);
				if (a.size() != 0) {
					all_possible_moves.insert(all_possible_moves.end(), a.begin(), a.end());
				}
				break;
			}
			default:
				break;
			}
		}
	}

	for (int i = 0; i < all_possible_moves.size(); i++) {
		board copy(*this);
		copy.make_move(all_possible_moves[i]);
		if (copy.check(player)) {
			all_possible_moves[i] = all_possible_moves.back();
			all_possible_moves.pop_back();
		}
	}

	return all_possible_moves;
}

BB board::get_bitboard(int piece_type, bool color) {
	return game_board[color][piece_type];
}

void board::init_board_with_FEN(std::string positions, std::string player, std::string castling, std::string en_passants, std::string halfmoves, std::string moves) {
	for (int i = 0; i < 64; i++) {
		support_game_board[i] = -1;
	}

	int i, j = 7;

	//Piece placement
	for (char piece_placement : positions) {
		i = 0;
		if (isdigit(piece_placement))
			i += (piece_placement - 0);
		else if (piece_placement == '/')
			j++;
		else {
			int piece;
			int color;
			if (piece_placement == 'p') {
				piece = pawns;
				color = black;
			}
			else if (piece_placement == 'r') {
				piece = rooks;
				color = black;
			}
			else if (piece_placement == 'n') {
				piece = knights;
				color = black;
			}
			else if (piece_placement == 'b') {
				piece = bishops;
				color = black;
			}
			else if (piece_placement == 'q') {
				piece = queens;
				color = black;
			}
			else if (piece_placement == 'k') {
				piece = king;
				color = black;
			}
			else if (piece_placement == 'P') {
				piece = pawns;
				color = white;
			}
			else if (piece_placement == 'R') {
				piece = rooks;
				color = white;
			}
			else if (piece_placement == 'N') {
				piece = knights;
				color = white;
			}
			else if (piece_placement == 'B') {
				piece = bishops;
				color = white;
			}
			else if (piece_placement == 'Q') {
				piece = queens;
				color = white;
			}
			else if (piece_placement == 'K') {
				piece = king;
				color = white;
			}
			game_board[color][piece] = 1i64 << (i + 8 * j);
			support_game_board[i + 8 * j] = piece;
			i++;
		}
	}

	//Active turn: ignored because it's handled by protocol
	//tokens[1]

	//Castling availability
	can_white_castle_long = false;
	can_white_castle_short = false;
	can_black_castle_long = false;
	can_black_castle_short = false;

	for (char castling_availability : castling) {
		switch (castling_availability) {
		case 'Q':
			can_white_castle_long = true;
			break;
		case 'K':
			can_white_castle_short = true;
			break;
		case 'q':
			can_black_castle_long = true;
			break;
		case 'k':
			can_black_castle_short = true;
			break;
		}
	}

	//en passant
	/*
	if (tokens[3] != "-") {
		int index = 0;
		index += tokens[3].at(0) - 'a';
		index += tokens[3].at(1) - '1';
		if (tokens[1] == "b")
			en_passant[0] = 1i64 << index;
		else
			en_passant[1] = 1i64 << index;
	}*/

	//halfmoves
	halfmove_clock = std::stoi(halfmoves);

	//total moves
	total_moves = std::stoi(moves);

	occupied_array[0] |= game_board[0][0];
	occupied_array[0] |= game_board[0][1];
	occupied_array[0] |= game_board[0][2];
	occupied_array[0] |= game_board[0][3];
	occupied_array[0] |= game_board[0][4];
	occupied_array[0] |= game_board[0][5];

	occupied_array[1] |= game_board[1][0];
	occupied_array[1] |= game_board[1][1];
	occupied_array[1] |= game_board[1][2];
	occupied_array[1] |= game_board[1][3];
	occupied_array[1] |= game_board[1][4];
	occupied_array[1] |= game_board[1][5];

	occupied = occupied_array[0] | occupied_array[1];
}

int board::piece_at(int index) {
	return support_game_board[index];
}

void board::print_support() {
	for (int i = 0; i < 64; i++) {
		std::cout << support_game_board[i] << " ";
		if (i % 8 == 0) {
			std::cout << std::endl;
		}
	}
}

std::vector<std::string> board::bitboard_to_string() {
	std::vector<std::string> to_ret;
	for (int player = 0; player < 2; player++) {
		for (int piece = 0; piece < 6; piece++) {
			to_ret.push_back(std::bitset<64>(game_board[player][piece]).to_string());
		}
	}
	return to_ret;
}