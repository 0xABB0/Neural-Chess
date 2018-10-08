#pragma once
#include "macros.h"

#include "slider_attacks.h"
#include "data.h"
#include "moves.h"

class board {
private:
	/*
	* [0] = white
	* [1] = black
	*
	* [0] = pawns
	* [1] = rooks
	* [2] = knights
	* [3] = bishops
	* [4] = queen
	* [5] = king
	*
	*/
	BB game_board[2][6];

	BB occupied_array[2];

	BB occupied;

	BB en_passant[2] = { 0,0 };

	short int support_game_board[64];

	int promote_pawn(bool, int);

	bool can_white_castle_long;
	bool can_white_castle_short;
	bool can_black_castle_long;
	bool can_black_castle_short;

	int halfmove_clock;

	int total_moves;

	SliderAttacks sliders;


public:
	board();
	board(const board&);

	BB get_pawn_moves_as_bitboard(unsigned short, bool);
	BB get_rook_moves_as_bitboard(unsigned short, bool);
	BB get_knight_moves_as_bitboard(unsigned short, bool);
	BB get_bishop_moves_as_bitboard(unsigned short, bool);
	BB get_king_moves_as_bitboard(unsigned short, bool);
	BB get_queen_moves_as_bitboard(unsigned short, bool);

	BB get_all_moves_as_bitboards(bool);

	std::vector<Move> get_pawns_moves(bool, int);
	std::vector<Move> get_rooks_moves(bool, int);
	std::vector<Move> get_knights_moves(bool, int);
	std::vector<Move> get_bishops_moves(bool, int);
	std::vector<Move> get_queens_moves(bool, int);
	std::vector<Move> get_king_moves(bool, int);

	std::vector<Move> get_all_moves(bool);

	BB get_pawns(bool player) { return game_board[player][pawns]; }
	BB get_rooks(bool player) { return game_board[player][rooks]; }
	BB get_knights(bool player) { return game_board[player][knights]; }
	BB get_bishops(bool player) { return game_board[player][bishops]; }
	BB get_queens(bool player) { return game_board[player][queens]; }
	BB get_kings(bool player) { return game_board[player][king]; }

	BB get_bitboard(int, bool);

	bool check(bool);
	bool checkmate(bool);

	void make_move(Move);

	void print_board_letters();

	void print_occupied();

	void init_standard_game();

	void init_board_with_FEN(std::string, std::string, std::string, std::string, std::string, std::string);

	int piece_at(int);

	void print_support();

	std::vector<std::string> bitboard_to_string();
};

void print_bitboard(const BB);

