#include "macros.h"
#include "game_board.h"
#include "player.h"
#include "winboard.h"


int main() {
	std::string command;
	board b;
	b.init_standard_game();
	player p;
	std::getline(std::cin, command);
	if (command == "xboard") {
		winboard_loop(b, p, command);
	}
	return 0;
}