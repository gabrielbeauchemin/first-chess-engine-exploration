#include "BoardRepresentation.h"

BoardRepresentation::BoardRepresentation()
	: isWhiteTurn{ true },
	canBlackCastle{ true },
	canWhiteCastle{ true },
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

	board[0] = whiteRook; board[1] = whiteKnight; board[2] = whiteBishop; board[3] = whiteQueen;
	board[4] = whiteKing; board[5] = whiteBishop; board[6] = whiteKnight; board[7] = whiteRook;
	for (int i = 8; i != 16; ++i) board[i] = whitePawn;
	for (int i = 16; i != 48; ++i) board[i] = emptyCase;
	for (int i = 48; i != 56; ++i) board[i] = blackPawn;
	board[56] = blackRook; board[57] = blackKnight; board[58] = blackBishop; board[59] = blackQueen;
	board[60] = blackKing; board[61] = blackBishop; board[62] = blackKnight; board[63] = blackRook;
}

//Only move the piece requested without checking for the
//Legality of it (MoveGeneration will do it)
void BoardRepresentation::move(Notation move)
{  
	if (isMoveCastling(move)) //Case Castling:
	{
		swap<Piece>(board, move.from, move.to); //Swap King
		bool isKingSideCastling = (move.from - move.to < 0) ? true : false;
		int rookPos = (isKingSideCastling) ? 7 : 0;
		int newRookPos = (isKingSideCastling) ? 5 : 3;
		swap<Piece>(board, rookPos, newRookPos); //Swap Rook

		if (isWhiteTurn)
			this->canWhiteCastle = false;
		else
			this->canBlackCastle = false;
	}
	else //Not Castling, normal move
	{
		//If king moves, he looses the right to castle
		if (board[move.from].type == PieceType::king)
		{
			if (isWhiteTurn)
				this->canWhiteCastle = false;
			else
				this->canBlackCastle = false;
		}

		//Move the piece
		swap<Piece>(board, move.from, move.to);
		isWhiteTurn = !this->isWhiteTurn;

		//Promotion
		if (move.promotion != PieceType::none)
			board[move.to].type = move.promotion;
	}
	
	if (board[move.from].type != PieceType::pawn)
	{
		++this->reversibleMovesInRow;
	}

	//En passant is only good for one move
	this->isEnPensantPossible = std::pair<bool, Piece>(false, Piece{});
	//Check if En passant in possible for the next move
	if (doesMovePermitEnPassant(move))
	{
		this->isEnPensantPossible =
			std::pair<bool, Piece>(true, this->board[move.from]);
	};
	
	//Build MoveResult
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

bool BoardRepresentation::isMoveCastling(Notation move)
{
	if(isWhiteTurn)
	{
		if(!canWhiteCastle)
		{
			return false;
		}
	}
	else
	{
		if (!canBlackCastle)
		{
			return false;
		}
	}

	bool isPieceMovedAKing = board[move.from].type == PieceType::king;
	bool isCastleTraveling = move.from + 2 == move.to || move.from - 2 == move.to;
	return isPieceMovedAKing & isCastleTraveling;
}

bool BoardRepresentation::doesMovePermitEnPassant(Notation lastMove)
{
	if (board[lastMove.to].type != PieceType::pawn)
		return false;

	//If the move permit En Passant, that means that its a pawn 
	//that advance two cases and a pawn from the opposite camp
	//Is one move next to it (in the other column)

	//Exception Pawn at the beginning and the end of the board has only one case next to it
	//Which are Cases 8 and 15 (white) and 48 and 55 
	bool isFirstPawn = lastMove.from % 8 == 0;
	bool isLastPawn = lastMove.from % 8 == 7;

	if (isFirstPawn)
	{
		return board[lastMove.to + 1].type == PieceType::pawn && 
			   board[lastMove.to + 1].isWhite == this->isWhiteTurn; //isWhiteTurn represent the turn of the opposite camp, so the pawn should be the same color
	}
	else if(isLastPawn)
	{
		return board[lastMove.to - 1].type == PieceType::pawn &&
			   board[lastMove.to - 1].isWhite == this->isWhiteTurn; //isWhiteTurn represent the turn of the opposite camp, so the pawn should be the same color
	}
	else 
	{
		return (board[lastMove.to + 1].type == PieceType::pawn &&
			    board[lastMove.to + 1].isWhite == this->isWhiteTurn) || //isWhiteTurn represent the turn of the opposite camp, so the pawn should be the same color
			   (board[lastMove.to - 1].type == PieceType::pawn &&
			    board[lastMove.to - 1].isWhite == this->isWhiteTurn); //isWhiteTurn represent the turn of the opposite camp, so the pawn should be the same color
	}
}
template<class T>
void BoardRepresentation::swap(T array[], int i, int j)
{
	T temp = array[i];
	array[i] = array[j];
	array[j] = temp;
}
