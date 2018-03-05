#include "BoardRepresentation.h"
#include "IlegalMoveException.h"
#include <assert.h>

BoardRepresentation::BoardRepresentation()
	: isWhiteTurn{ true },
	canBlackCastle{ true },
	canWhiteCastle{ true },
	reversibleMovesInRow{ 0 }
{
	this->isEnPensantPossible = std::pair<bool, int>(false, -1);

	//Initialize board with beginning position
	board[0] = Piece::whiteRook; board[1] = Piece::whiteKnight; board[2] = Piece::whiteBishop; board[3] = Piece::whiteQueen;
	board[4] = Piece::whiteKing; board[5] = Piece::whiteBishop; board[6] = Piece::whiteKnight; board[7] = Piece::whiteRook;
	for (int i = 8; i != 16; ++i) board[i] = Piece::whitePawn;
	for (int i = 16; i != 48; ++i) board[i] = Piece::none;
	for (int i = 48; i != 56; ++i) board[i] = Piece::blackPawn;
	board[56] = Piece::blackRook; board[57] = Piece::blackKnight; board[58] = Piece::blackBishop; board[59] = Piece::blackQueen;
	board[60] = Piece::blackKing; board[61] = Piece::blackBishop; board[62] = Piece::blackKnight; board[63] = Piece::blackRook;
}

BoardRepresentation::BoardRepresentation(std::vector<std::pair<int, Piece>> piecesToPlace)
	: isWhiteTurn{ true },
	canBlackCastle{ true },
	canWhiteCastle{ true },
	reversibleMovesInRow{ 0 }
{
	this->isEnPensantPossible = std::pair<bool, int>(false, -1);

	//Put an empty board
	for (int i = 0; i < 64; ++i)
		this->board[i] = Piece::none;

	//Place the pieces requested
	for (auto& pieceToPlace : piecesToPlace)
		this->board[pieceToPlace.first] = pieceToPlace.second;
}

bool BoardRepresentation::operator==(const BoardRepresentation & other)
{
	for (int i = 0; i < 64; ++i)
	{
		if (this->board[i] != other.board[i])
			return false;
	}

	return true;
}

//Only move the piece requested without checking for the
//Legality of it (MoveGeneration will do it)
void BoardRepresentation::move(Move move)
{  
	assert(isPieceWhite(this->board[move.from]) == this->isWhiteTurn);
	assert(!isPieceNone(this->board[move.from]));
	if (!isPieceNone(this->board[move.to]) &&
		(isPieceWhite(this->board[move.to]) == this->isWhiteTurn))
	{
		std::string t = this->toString();
		assert(false);
	}

	if (isMoveCastling(move)) //Case Castling:
	{
		swap<Piece>(board, move.from, move.to); //Swap King
		bool isKingSideCastling = (move.from - move.to < 0) ? true : false;
		int rookPos;
		if (this->isWhiteTurn && isKingSideCastling) rookPos = 7;
		else if (this->isWhiteTurn && !isKingSideCastling) rookPos = 0;
		else if (!this->isWhiteTurn && isKingSideCastling) rookPos = 63;
		else if (!this->isWhiteTurn && !isKingSideCastling) rookPos = 56;
		int translationRook = (isKingSideCastling) ? -2 : 3;
		swap<Piece>(board, rookPos, rookPos + translationRook); //Swap Rook

		if (isWhiteTurn)
			this->canWhiteCastle = false;
		else
			this->canBlackCastle = false;
	}
	else //Not Castling, normal move
	{
		//King moves looses the right to castle
		if (isPieceKing(board[move.from]))
		{
			if (isWhiteTurn && this->canWhiteCastle == true)
			{
				this->canWhiteCastle = false;
				this->justLooseRightCastle = true; //Help for unmakeMove
			}
			else if (!isWhiteTurn && this->canBlackCastle == true)
			{
				this->canBlackCastle = false;
				this->justLooseRightCastle = true; //Help for unmakeMove
			}
		}

		//In case of Capture
		if ( !isPieceNone(board[move.to]))
		{
			this->lastCaptures[nbrMovesDone] = board[move.to]; //Help for unmakeMove
			board[move.to] = Piece::none;
		}

		//Move the piece
		swap<Piece>(board, move.from, move.to);

		//Promotion
		if (!isPieceNone(move.promotion))
			board[move.to] = move.promotion;
	}
	
	isWhiteTurn = !this->isWhiteTurn;
	if (!isPiecePawn(board[move.to]))
	{
		++this->reversibleMovesInRow;
	}
	else
	{
		this->lastReversibleMovesinRow = this->reversibleMovesInRow;
		this->reversibleMovesInRow = 0;
	}

	//En passant is only good for one move
	this->isEnPensantPossible = std::pair<bool, int>(false, -1);
	//Check if En passant in possible for the next move
	if (movesPermitEnPassant(move))
	{
		this->isEnPensantPossible = std::pair<bool, int>(true, move.to);
	};

	++nbrMovesDone;
}

void BoardRepresentation::unmakeMove(Move move)
{
	assert(isPieceWhite(this->board[move.to]) != this->isWhiteTurn);	

	if (isMoveCastling(move)) //Case Castling:
	{
		swap<Piece>(board, move.to, move.from); //Swap King
		bool isKingSideCastling = (move.from - move.to < 0) ? true : false;
		int initialRookPos;
		if (this->isWhiteTurn && isKingSideCastling) initialRookPos = 7;
		else if (this->isWhiteTurn && !isKingSideCastling) initialRookPos = 0;
		else if (!this->isWhiteTurn && isKingSideCastling) initialRookPos = 63;
		else if (!this->isWhiteTurn && !isKingSideCastling) initialRookPos = 56;
		int translationRook = (isKingSideCastling) ? -2 : 3;
		swap<Piece>(board, initialRookPos + translationRook, initialRookPos); //Swap Rook

		if (isWhiteTurn)
			this->canBlackCastle = true;
		else
			this->canWhiteCastle = true;
	}
	else //Not Castling, normal move
	{
		assert(isPieceNone(this->board[move.from]));

		//King moves looses the right to castle
		if (isPieceKing(board[move.from]) && this->justLooseRightCastle == true)
		{
			if (isWhiteTurn)
				this->canBlackCastle = true;
			else
				this->canWhiteCastle = true;
			this->justLooseRightCastle == false;
		}

		//Move the piece
		swap<Piece>(board, move.to, move.from);

		//In case that a piece got Captured the move before
		auto lastCapture = this->lastCaptures.find(nbrMovesDone - 1);
		if (lastCaptures.end() != lastCapture)
		{
			assert(this->isWhiteTurn == isPieceWhite(lastCapture->second));
			board[move.to] = lastCapture->second;
			this->lastCaptures.erase(lastCapture);
		}
	}

	isWhiteTurn = !this->isWhiteTurn;
	if (!isPiecePawn(board[move.from]))
	{
		--this->reversibleMovesInRow;
	}
	else this->reversibleMovesInRow = lastReversibleMovesinRow;

	//Unmaking the move can only remove the possibility of an En passant, not create one
	this->isEnPensantPossible = std::pair<bool, int>(false, -1);

	--nbrMovesDone;
}

std::string BoardRepresentation::toString()
{
	std::string stringBoard = "";
	auto pieceToChar = [](Piece p) ->std::string
	{
		if (isPieceBishop(p))
			return "b";
		else if (isPieceKing(p))
			return "k";
		else if (isPieceKnight(p))
			return "n";
		else if (isPieceNone(p))
			return " ";
		else if (isPiecePawn(p))
			return "p";
		else if (isPieceQueen(p))
			return "q";
		else if (isPieceRook(p))
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

bool BoardRepresentation::isMoveCastling(Move move)
{
	static Move allCastlingMoves[]{ Move{4,6}, Move{ 4,2 }, Move{ 60,62 }, Move{ 60,58 } };
	for (auto& m : allCastlingMoves)
		if (m == move)
			return true;
	return false;
}

bool BoardRepresentation::movesPermitEnPassant(Move lastMove)
{
	if (!isPiecePawn(board[lastMove.to]))
		return false;

	//If the move permit En Passant, that means that its a pawn 
	//that advance two cases and a pawn from the opposite camp
	//Is one case to its side

	//Pawn at the beginning and the end of the board has only one case next to it
	//Which are Cases 8 and 15 (white) and 48 and 55 
	bool isFirstPawn = lastMove.from % 8 == 0;
	bool isLastPawn = lastMove.from % 8 == 7;

	if (isFirstPawn)
	{
		return isPiecePawn(board[lastMove.to + 1]) && 
			   isPieceWhite(board[lastMove.to + 1]) != this->isWhiteTurn; //Pawn to the side must be from opposite camp
	}
	else if(isLastPawn)
	{
		return isPiecePawn(board[lastMove.to - 1]) &&
			   isPieceWhite(board[lastMove.to - 1]) != this->isWhiteTurn; //Pawn to the side must be from opposite camp
	}
	else //Pawn in the middles, check both side of them
	{
		bool oppositePawnLeftSide = (isPiecePawn(board[lastMove.to + 1]) &&
			                        isPieceWhite(board[lastMove.to + 1]) != isPieceWhite(board[lastMove.to])); //Pawn to the side must be from opposite camp
		bool oppositePawnRightSide = (isPiecePawn(board[lastMove.to - 1]) &&
			                         isPieceWhite(board[lastMove.to - 1]) != isPieceWhite(board[lastMove.to])); //Pawn to the side must be from opposite camp
		//At least one pawn from opposite camp should be to its side to permit en passant
		return oppositePawnLeftSide || oppositePawnRightSide;
	}
}

template<class T>
void BoardRepresentation::swap(T array[], int i, int j)
{
	T temp = array[i];
	array[i] = array[j];
	array[j] = temp;
}
