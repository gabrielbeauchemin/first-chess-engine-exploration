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


MoveResult BoardRepresentation::move(Notation move)
{  
	//Invalid to go to a case that is already occupied
	if(board[move.to].type != PieceType::none)
		return MoveResult{ false };

	//Move the Piece
	if (isMoveCastling(move))
	{
		//Check if the cases between the Castle and the king are empty
		for(int caseInTheWay = move.from + 1; caseInTheWay < move.to; ++ caseInTheWay)
			if(board[caseInTheWay].type != PieceType::none)
				return MoveResult{ false };

		//Valid if the king can Castle
		bool isKingSideCastle = move.from < move.to; //If false, we know its a queen sine castle
		int translationKingRook = (isKingSideCastle) ? 2 : -3;
		bool isRookPresent = board[move.from + translationKingRook].type == PieceType::rook;
		bool canKingCastle = (this->isWhiteTurn) ? canWhiteCastle : canBlackCastle;
		if (!isRookPresent || !canKingCastle)
		{
			return MoveResult{ false }; //King is not allowed to castle
		}

		//Castle
		swap<Piece>(board, move.from, move.to); //Swap King
		int translationNewRookPos = (canKingCastle) ? 1 : -1;
		swap<Piece>(board, move.from + translationKingRook, move.from + translationNewRookPos); //Swap Rook

		if (isWhiteTurn)
			this->canWhiteCastle = false;
		else
			this->canBlackCastle = false;
	}
	//TO DO: Promotion 
	//else if
	else
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
	}
	
	

	if (board[move.from].type != PieceType::pawn)
	{
		++this->reversibleMovesInRow;
		//Rules of 50 moves: Its stealMate
		if (this->reversibleMovesInRow > 49) 
			return MoveResult{ true, true, false, true }; 
	}
		

	

	isWhiteTurn = !this->isWhiteTurn; 

	//En passant is only good for one move
	this->isEnPensantPossible = std::pair<bool, Piece>(false, Piece{});
	//Check if En passant in possible for the next move
	if (doesMovePermitEnPassant(move))
	{
		this->isEnPensantPossible =
			std::pair<bool, Piece>(true, this->board[move.from]);
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
				MoveGeneration::generateKingMoves(*this, i).size() == 0)
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

bool BoardRepresentation::doesMovePermitEnPassant(Notation move)
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
