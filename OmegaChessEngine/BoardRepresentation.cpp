#include "BoardRepresentation.h"

BoardRepresentation::BoardRepresentation()
	: isWhiteTurn{ false },
	canBlackRook{ true },
	canWhiteRook{ true },
	reversibleMovesInRow{ 0 }
{
	Piece emptyCase = Piece{ PieceType::none, false };
	this->isEnPensantPossible = std::pair<bool, Piece>(false, emptyCase);

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
	

	board[0] = whiteRook; board[1] = whiteKnight; board[2] = whiteBishop; board[3] = whiteKing;
	board[4] = whiteQueen; board[5] = whiteBishop; board[6] = whiteKnight; board[7] = whiteRook;
	for (int i = 8; i != 16; ++i) board[i] = whitePawn;
	for (int i = 16; i != 48; ++i) board[i] = emptyCase;
	for (int i = 48; i != 56; ++i) board[i] = blackPawn;
	board[56] = blackRook; board[57] = blackKnight; board[58] = blackBishop; board[59] = blackKing;
	board[60] = blackQueen; board[61] = blackBishop; board[62] = blackKnight; board[63] = blackRook;
}




bool BoardRepresentation::move(BoardRepresentation board, Notation move)
{
	return false;
}

std::string BoardRepresentation::toString()
{
	std::string stringBoard = "";
	auto pieceToChar = [](Piece p) ->std::string
	{
		if (p.type == PieceType::bishop)
			return "b";
		else if (p.type == PieceType::king)
			return "k";
		else if (p.type == PieceType::knight)
			return "n";
		else if (p.type == PieceType::none)
			return " ";
		else if (p.type == PieceType::pawn)
			return "p";
		else if (p.type == PieceType::queen)
			return "q";
		else if (p.type == PieceType::rook)
			return "r";
	};

	std::string charPieces[64];
	for (int i = 0; i < 64; ++i)
		charPieces[i] = pieceToChar(this->board[i]);

	int nextPiece = 64;
	auto getNextPiece = [&nextPiece, &charPieces] {return charPieces[--nextPiece]; };
	for (int row = 0; row < 8; ++row)
	{
		stringBoard += "#################################################\r\n";
		stringBoard += "#     #     #     #     #     #     #     #     #\r\n";
		stringBoard += "#  " + getNextPiece() + "  #  " + getNextPiece() + "  #  " + getNextPiece() + "  #  " + getNextPiece() + "  #  " + getNextPiece() + "  #  " + getNextPiece() + "  #  " + getNextPiece() + "  #  " + getNextPiece() + "  #\r\n";
		stringBoard += "#     #     #     #     #     #     #     #     #\r\n";
	}
	stringBoard += "#################################################\r\n";

	return stringBoard;
}
