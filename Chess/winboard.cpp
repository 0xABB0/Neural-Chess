#include "winboard.h"

std::string parse_command(std::string& command, std::vector<std::string>& command_tokens) {
	boost::algorithm::to_lower(command);

	boost::split(command_tokens, command, [](char c) {return c == ' '; });
	return command_tokens[0];
}

void winboard_loop(board &b, player engine, std::string& command) {
	enum { WHITE, BLACK, FORCE_MODE, ANALYZE } engine_mode;
	enum { WHITETOMOVE, BLACKTOMOVE } game_state;
	int last_game_result;
	std::string results[3] = { "1-0", "0-1", "1/2-1/2" };

	engine_mode = BLACK;
	do {
		if ((engine_mode == BLACK && game_state == BLACKTOMOVE) || (engine_mode == WHITE && game_state == WHITETOMOVE)) {
			std::vector<move> possible_moves = b.get_all_moves(engine.getSide());
			if (possible_moves.size() == 0) {
				std::string result;
				if (engine_mode == WHITE) {
					result = "0-1";
				}
				else {
					result = "1-0";
				}
				std::cout << result << std::endl << "resign" << std::endl;
			}
			else {
				move move = engine.choose_move(b, possible_moves);
				b.make_move(move);
				if (game_state == WHITETOMOVE) {
					game_state = BLACKTOMOVE;
				}
				else {
					game_state = WHITETOMOVE;
				}
				std::cout << "move " << move.to_long_algebraic_notation() << std::endl;
			}
		}
		std::getline(std::cin, command);

		std::vector<std::string> command_tokens;

		command = parse_command(command, command_tokens);

		if (command == "force") {
			engine_mode = FORCE_MODE;
		}
		else if (command == "ping") {
			std::cout << "pong " << command_tokens[1] << std::endl;
		}
		else if (command == "new") {
			engine_mode = BLACK;
			engine.setSide(black);
			game_state = WHITETOMOVE;
			b.init_standard_game();
		}
		else if (command == "white") {
			engine_mode = WHITE;
			engine.setSide(white);
		}
		else if (command == "black") {
			engine_mode = BLACK;
			engine.setSide(black);
		}
		else if (command == "go") {
			// answer with "move LONGALGEBRAICNOTATION"
		}
		else if (command == "playother") {
			// answer with "move LONGALGEBRAICNOTATION"
		}
		else if (command == "usermove") {

			if (command_tokens.size() > 1) {
				command = command_tokens.at(1);
				int starting_file = command.at(0) - 'a';
				int starting_rank = command.at(1) - '1';
				int ending_file = command.at(2) - 'a';
				int ending_rank = command.at(3) - '1';
				int move_flag = 0;
				int promotion_type = 0;

				if (command == "e1g1" || command == "e1c1" || command == "e8g8" || command == "e8c8") {
					move_flag = 3;
				}
				if (command.size() == 5) {
					if (command.at(4) == 'q') {
						promotion_type = 4;
					}
					else if (command.at(4) == 'r') {
						promotion_type = 1;
					}
					else if (command.at(4) == 'n') {
						promotion_type = 2;
					}
					else if (command.at(4) == 'b') {
						promotion_type = 3;
					}
				}

				int piece = b.piece_at(starting_file + starting_rank * 8);
				move move(game_state == WHITETOMOVE ? false : true,
					piece, starting_file + starting_rank * 8,
					ending_file + ending_rank * 8, move_flag, promotion_type);

				b.make_move(move);

				if (game_state == WHITETOMOVE)
					game_state = BLACKTOMOVE;
				else
					game_state = WHITETOMOVE;
			}
		}
		else if (command == "setboard") {
			// FEN STRING in arrivo
			if (command_tokens.size() > 1) {
				b.init_board_with_FEN(command_tokens[1], command_tokens[2], command_tokens[3], command_tokens[4], command_tokens[5], command_tokens[6]);
			}
			if (command_tokens[2] == "b") {
				game_state = BLACKTOMOVE;
			}
			else {
				game_state = WHITETOMOVE;
			}
		}
		else if (command == "result") {
			// return results from last game 1-0 (white wins), 0-1 (black wins), 1/2-1/2 (draw)
		}
		else if (command == "edit") {
			/*
			first wihte. capital letters for pieces and lower capital for position
			# clears the board
			c change colors
			. ends this command
			*/
		}
		else if (command == "protover") {
			std::vector<std::string> supported_features = { "ping", "colors", "usermove", "setboard", "debug" };
			std::vector<std::string> unsupported_features = { "time", "smp", "analyze", "playother", "san", "draw", "sigint", "sigterm", "ics", "name", "pause", "nps", "memory" };

			for (std::string feat : supported_features)
				std::cout << "feature " << feat << "=1" << std::endl;

			for (std::string uns_feat : unsupported_features)
				std::cout << "feature " << uns_feat << "=0" << std::endl;

			std::cout << "feature " << "done=1" << std::endl;
		}
	} while (command != "quit");
}