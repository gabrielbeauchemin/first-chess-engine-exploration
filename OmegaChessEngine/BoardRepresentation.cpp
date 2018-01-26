#include "BoardRepresentation.h"
#include "InvalidChessCaseException.h"

BoardRepresentation::BoardRepresentation()
	: isWhiteTurn{ true },
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


MoveResult BoardRepresentation::move(Notation move)
{  
	//Assert move out of bound
	if (move.from < 0 || move.from > 63 || move.to < 0 || move.to > 63)
		throw InvalidChessCaseException{};

	//Move the Piece
	swap<Piece>(board, move.from, move.to);

    //Update Information about the board representation
	if (board[move.from].type != PieceType::pawn)
		++this->reversibleMovesInRow;

	if (isMoveARook(move))
	{
		if (isWhiteTurn)
			this->canWhiteRook = false;
		else
			this->canBlackRook = false;
	}

	isWhiteTurn = !this->isWhiteTurn; 

	if (isMoveEnPassant(move))
	{
		this->isEnPensantPossible =
			std::pair<bool, Piece>(false, this->board[move.from]);
	};
	
	//Build MoveResult
	if (!MoveGeneration::isStateLegal(*this, move)) //move is Now the last move
	{
		return MoveResult{ false };
	}

	//If the game is ended (only checkmate)
	//Checking for StealMate is not necessary for the chess engine
	Piece kings;
	for (int i = 0; i < 64; ++i)
	{
		if (board[i].type == PieceType::king)
		{
			//Checkmate
			if (MoveGeneration::isKingCheck(*this, i) &&
				MoveGeneration::generateKingMoves(*this, i).size == 0)
				return MoveResult{ true, true, !board[i].isWhite };
		}
	}
		
	return MoveResult{ true };
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
		else throw std::exception{};
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

bool BoardRepresentation::isMoveARook(Notation move)
{
	if(isWhiteTurn)
	{
		if(!canWhiteRook)
		{
			return false;
		}
	}
	else
	{
		if (!canBlackRook)
		{
			return false;
		}
	}

	bool isPieceMovedAKing = board[move.from].type == PieceType::king;
	bool isRookTraveling = move.from + 2 == move.to || move.from - 2 == move.to;
	return isPieceMovedAKing & isRookTraveling;
}

bool BoardRepresentation::isMoveEnPassant(Notation move)
{
	//TO DO
	return false;
}
template<class T>
void BoardRepresentation::swap(T array[], int i, int j)
{
	T temp = array[i];
	array[i] = array[j];
	array[j] = temp;
}
