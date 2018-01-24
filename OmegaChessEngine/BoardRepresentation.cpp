#include "BoardRepresentation.h"

BoardRepresentation::BoardRepresentation()
	: isWhiteTurn{false},
	  canBlackRook{true},
	  canWhiteRook{true},
	  reversibleMovesInRow{0}

	  
{
	this->isEnPensantPossible = std::make_pair<bool, PieceType>(false, PieceType::none);

	//Initialize board with beginning position
	Piece whiteQueen = Piece{ PieceType::queen, true };
	Piece whiteKing = Piece{ PieceType::king, true };
	Piece whiteRook = Piece{ PieceType::rook, true };
	Piece whiteBishop = Piece{ PieceType::bishop, true };
	Piece whiteKnight = Piece{ PieceType::knight, true };
	Piece whitePawn = Piece{ PieceType::pawn, true };
	Piece blackQueen = Piece{ PieceType::queen, false };
	Piece blackKing = Piece{ PieceType::king, false };
	Piece blackRook = Piece{ PieceType::rook, false };
	Piece blackBishop = Piece{ PieceType::bishop, false };
	Piece blackKnight = Piece{ PieceType::knight, false };
	Piece blackPawn = Piece{ PieceType::pawn, false };
	Piece emptyCase = Piece{ PieceType::none, false };



	this->board{emptyCase, emptyCase, emptyCase, emptyCase, emptyCase, emptyCase, emptyCase, emptyCase,
		emptyCase, emptyCase, emptyCase, emptyCase, emptyCase, emptyCase, emptyCase, emptyCase,
		emptyCase, emptyCase, emptyCase, emptyCase, emptyCase, emptyCase, emptyCase, emptyCase,
		emptyCase, emptyCase, emptyCase, emptyCase, emptyCase, emptyCase, emptyCase, emptyCase,
		emptyCase, emptyCase, emptyCase, emptyCase, emptyCase, emptyCase, emptyCase, emptyCase,
		emptyCase, emptyCase, emptyCase, emptyCase, emptyCase, emptyCase, emptyCase, emptyCase,
		emptyCase, emptyCase, emptyCase, emptyCase, emptyCase, emptyCase, emptyCase, emptyCase,
		emptyCase, emptyCase, emptyCase, emptyCase, emptyCase, emptyCase, emptyCase, emptyCase};
}

BoardRepresentation BoardRepresentation::move(BoardRepresentation board, Notation move)
{
	return BoardRepresentation();
}

std::string BoardRepresentation::toString()
{
	return std::string();
}
