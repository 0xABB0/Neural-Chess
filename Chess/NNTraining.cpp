#include "NNTraining.h"

Training::Training(OpenNN::NeuralNetwork &nn) {
	ds.set_data_file_name("data.dat");
	ds.load_data();

	li = OpenNN::LossIndex(&nn, &ds);
	li.set_error_type("MEAN_SQUARED_ERROR");

	ts = OpenNN::TrainingStrategy(&li);
}

void Training::performAndSaveTraining() {
	ts.set_initialization_type(OpenNN::TrainingStrategy::RANDOM_SEARCH);
	OpenNN::RandomSearch* rsp = ts.get_random_search_pointer();
	rsp->set_maximum_iterations_number(10);

	ts.set_main_type("GRADIENT_DESCENT");
	OpenNN::GradientDescent* gdp = ts.get_gradient_descent_pointer();
	gdp->set_maximum_iterations_number(25);

	ts.perform_training();

	ts.save("training_strategy.xml");
}

/*
Position& Position::set(const string& fenStr, bool isChess960, StateInfo* si, Thread* th) {
/*
   A FEN string defines a particular position using only the ASCII character set.
   A FEN string contains six fields separated by a space. The fields are:
   1) Piece placement (from white's perspective). Each rank is described, starting
      with rank 8 and ending with rank 1. Within each rank, the contents of each
      square are described from file A through file H. Following the Standard
      Algebraic Notation (SAN), each piece is identified by a single letter taken
      from the standard English names. White pieces are designated using upper-case
      letters ("PNBRQK") whilst Black uses lowercase ("pnbrqk"). Blank squares are
      noted using digits 1 through 8 (the number of blank squares), and "/"
      separates ranks.
   2) Active color. "w" means white moves next, "b" means black.
   3) Castling availability. If neither side can castle, this is "-". Otherwise,
      this has one or more letters: "K" (White can castle kingside), "Q" (White
      can castle queenside), "k" (Black can castle kingside), and/or "q" (Black
      can castle queenside).
   4) En passant target square (in algebraic notation). If there's no en passant
      target square, this is "-". If a pawn has just made a 2-square move, this
      is the position "behind" the pawn. This is recorded only if there is a pawn
      in position to make an en passant capture, and if there really is a pawn
      that might have advanced two squares.
   5) Halfmove clock. This is the number of halfmoves since the last pawn advance
      or capture. This is used to determine if a draw can be claimed under the
      fifty-move rule.
   6) Fullmove number. The number of the full move. It starts at 1, and is
      incremented after Black's move.


  unsigned char col, row, token;
  size_t idx;
  Square sq = SQ_A8;
  std::istringstream ss(fenStr);

  std::memset(this, 0, sizeof(Position));
  std::memset(si, 0, sizeof(StateInfo));
  std::fill_n(&pieceList[0][0], sizeof(pieceList) / sizeof(Square), SQ_NONE);
  st = si;

  ss >> std::noskipws;

  // 1. Piece placement
  while ((ss >> token) && !isspace(token))
  {
      if (isdigit(token))
          sq += (token - '0') * EAST; // Advance the given number of files

      else if (token == '/')
          sq += 2 * SOUTH;

      else if ((idx = PieceToChar.find(token)) != string::npos)
      {
          put_piece(Piece(idx), sq);
          ++sq;
      }
  }

  // 2. Active color
  ss >> token;
  sideToMove = (token == 'w' ? WHITE : BLACK);
  ss >> token;

  // 3. Castling availability. Compatible with 3 standards: Normal FEN standard,
  // Shredder-FEN that uses the letters of the columns on which the rooks began
  // the game instead of KQkq and also X-FEN standard that, in case of Chess960,
  // if an inner rook is associated with the castling right, the castling tag is
  // replaced by the file letter of the involved rook, as for the Shredder-FEN.
  while ((ss >> token) && !isspace(token))
  {
      Square rsq;
      Color c = islower(token) ? BLACK : WHITE;
      Piece rook = make_piece(c, ROOK);

      token = char(toupper(token));

      if (token == 'K')
          for (rsq = relative_square(c, SQ_H1); piece_on(rsq) != rook; --rsq) {}

      else if (token == 'Q')
          for (rsq = relative_square(c, SQ_A1); piece_on(rsq) != rook; ++rsq) {}

      else if (token >= 'A' && token <= 'H')
          rsq = make_square(File(token - 'A'), relative_rank(c, RANK_1));

      else
          continue;

      set_castling_right(c, rsq);
  }

  // 4. En passant square. Ignore if no pawn capture is possible
  if (   ((ss >> col) && (col >= 'a' && col <= 'h'))
      && ((ss >> row) && (row == '3' || row == '6')))
  {
      st->epSquare = make_square(File(col - 'a'), Rank(row - '1'));

      if (   !(attackers_to(st->epSquare) & pieces(sideToMove, PAWN))
          || !(pieces(~sideToMove, PAWN) & (st->epSquare + pawn_push(~sideToMove))))
          st->epSquare = SQ_NONE;
  }
  else
      st->epSquare = SQ_NONE;

  // 5-6. Halfmove clock and fullmove number
  ss >> std::skipws >> st->rule50 >> gamePly;

  // Convert from fullmove starting from 1 to gamePly starting from 0,
  // handle also common incorrect FEN with fullmove = 0.
  gamePly = std::max(2 * (gamePly - 1), 0) + (sideToMove == BLACK);

  chess960 = isChess960;
  thisThread = th;
  set_state(st);

  assert(pos_is_ok());

  return *this;
}


*/