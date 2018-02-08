#include "BoardRepresentation.h"
#include "IlegalMoveException.h"

BoardRepresentation::BoardRepresentation()
	: isWhiteTurn{ true },
	canBlackCastle{ true },
	canWhiteCastle{ true },
	reversibleMovesInRow{ 0 }
{
	Piece emptyCase = Piece{ PieceType::none, false };
	this->isEnPensantPossible = std::pair<bool, int>(false, -1);

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

BoardRepresentation::BoardRepresentation(std::vector<std::pair<int, Piece>> piecesToPlace)
	: isWhiteTurn{ true },
	canBlackCastle{ true },
	canWhiteCastle{ true },
	reversibleMovesInRow{ 0 }
{
	Piece emptyCase = Piece{ PieceType::none, false };
	this->isEnPensantPossible = std::pair<bool, int>(false, -1);

	//Put an empty board
	for (int i = 0; i < 64; ++i)
		this->board[i] = emptyCase;

	//Place the pieces requested
	for (auto& pieceToPlace : piecesToPlace)
		this->board[pieceToPlace.first] = pieceToPlace.second;
}

//Only move the piece requested without checking for the
//Legality of it (MoveGeneration will do it)
void BoardRepresentation::move(Notation move)
{  
	if (this->board[move.from].isWhite != this->isWhiteTurn)
	{
		throw IlegalMoveException{};
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

		isWhiteTurn = !this->isWhiteTurn;
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

		//In case of Capture. Dont validate the color of the captured piece, generate Move should generate legal moves
		if (board[move.to].type != PieceType::none)
		{
			board[move.to].type = PieceType::none;
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
	this->isEnPensantPossible = std::pair<bool, int>(false, -1);
	//Check if En passant in possible for the next move
	if (movesPermitEnPassant(move))
	{
		this->isEnPensantPossible = std::pair<bool, int>(true, move.to);
	};
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
	Notation allCastlingMoves[]{ Notation{4,6}, Notation{ 4,2 }, Notation{ 60,62 }, Notation{ 60,58 } };
	for (auto& m : allCastlingMoves)
		if (m == move)
			return true;
	return false;
}

bool BoardRepresentation::movesPermitEnPassant(Notation lastMove)
{
	if (board[lastMove.to].type != PieceType::pawn)
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
		return board[lastMove.to + 1].type == PieceType::pawn && 
			   board[lastMove.to + 1].isWhite != this->isWhiteTurn; //Pawn to the side must be from opposite camp
	}
	else if(isLastPawn)
	{
		return board[lastMove.to - 1].type == PieceType::pawn &&
			   board[lastMove.to - 1].isWhite != this->isWhiteTurn; //Pawn to the side must be from opposite camp
	}
	else //Pawn in the middles, check both side of them
	{
		bool oppositePawnLeftSide = (board[lastMove.to + 1].type == PieceType::pawn &&
			                        board[lastMove.to + 1].isWhite != board[lastMove.to].isWhite); //Pawn to the side must be from opposite camp
		bool oppositePawnRightSide = (board[lastMove.to - 1].type == PieceType::pawn &&
			                         board[lastMove.to - 1].isWhite != board[lastMove.to].isWhite); //Pawn to the side must be from opposite camp
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
