#include "BoardRepresentation.h"
#include "IlegalMoveException.h"
#include <assert.h>
#include <iostream>
#include <string>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>


//init board from FEN notation: https://fr.wikipedia.org/wiki/Notation_Forsyth-Edwards
//Used also during UTC protocol
BoardRepresentation::BoardRepresentation(std::string fen)
{
	auto split = [](const std::string& s, char delimiter)
	{
		std::vector<std::string> tokens;
		std::string token;
		std::istringstream tokenStream(s);
		while (std::getline(tokenStream, token, delimiter))
		{
			tokens.push_back(token);
		}
		return tokens;
	};

	
	std::vector<std::string> fenSplitted = split(fen, ' ');

	//Fen Notation has 6 parts:

	// 1)Pieces Position
	//rook(tour), knight(cavalier), bishop(fou), queen(dame), king(roi) et pawn(pion).
	static std::map<int, Piece> charToPiece{ {'r', Piece::whiteRook},{'n', Piece::whiteKnight },{ 'b', Piece::whiteBishop },{ 'q', Piece::whiteQueen },{ 'k', Piece::whiteKing },{ 'p', Piece::whitePawn },{ 'R', Piece::blackRook },{ 'N', Piece::blackKnight },{ 'B', Piece::blackBishop },{ 'Q', Piece::blackQueen },{ 'K', Piece::blackKing },{ 'P', Piece::blackPawn } };
	int indexBoard = 0;
	std::vector<std::string> rowPieces = split(fenSplitted[0], '/');
	for (std::string& row : rowPieces)
	{
		for (int indexCase = 0; indexCase < 8; )
		{
			char currentCase = row[indexCase];
			//Number between 1 to 8 indicated the number of empty cases
			if (isdigit(currentCase))
			{
				char nbrEmtyCases = atoi(&currentCase);
				assert(nbrEmtyCases > 0 && nbrEmtyCases < 9);
				for (int temp = 0; temp < nbrEmtyCases; ++temp)
				{
					this->board[indexBoard] = Piece::none;
					++indexBoard;
					++indexCase;
				}
			}
			else
			{
				Piece currentPiece = charToPiece.find(currentCase)->second;
				this->board[indexBoard] = currentPiece;
				++indexBoard;
				++indexCase;
			}
		}
	}
	// 2)Who is turn to play
	if (fenSplitted[1][0] == 'w')
		this->isWhiteTurn = true;
	else this->isWhiteTurn = false;

	// 3)Possible rook
	if (fenSplitted[2][0] == '-')
	{
		this->canWhiteCastle = false;
		if (fenSplitted[2].substr(1).find('-') != std::string::npos) 
			this->canBlackCastle = false;
		else this->canBlackCastle = true;
	}
	else
	{
		this->canWhiteCastle = true;
		if (fenSplitted[2].substr(1).find('-') != std::string::npos)
			this->canBlackCastle = false;
		else this->canBlackCastle = true;
	}
	
	// 4)Possible en Passant
	if (fenSplitted[3][0] != '-')
	{
		static std::map<char, int> algebraicToColumnIndex{ { 'a',0 },{ 'b',1 },{ 'c',2 },{ 'd',3 },{ 'e',4 },{ 'f',5 },{ 'g',6 },{ 'h',7 } };
		int columnIndex = algebraicToColumnIndex.find(fenSplitted[3][0])->second;
		int rowIndex = atoi(&fenSplitted[3][1]) -1;
		int enPassantchessIndex = (rowIndex * 8) + columnIndex;
		this->isEnPensantPossible = std::pair<bool, int>{ true, enPassantchessIndex };
	}
	// 5)Number of reversible moves
	this->reversibleMovesInRow = std::stoi(fenSplitted[4]);

	// 6)Number of total complete moves
	//Not used for now

}

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

BoardRepresentation & BoardRepresentation::operator=(const BoardRepresentation& other)
{
	for (int i = 0; i<64; ++i)
		this->board[i] = other.board[i];
	this->canBlackCastle = other.canBlackCastle;
	this->canWhiteCastle = other.canWhiteCastle;
	this->isEnPensantPossible = other.isEnPensantPossible;
	this->isWhiteTurn = other.isWhiteTurn;
	this->justLooseRightCastle = other.justLooseRightCastle;
	this->lastCaptures = other.lastCaptures;
	this->lastReversibleMovesinRow = other.lastReversibleMovesinRow;
	this->nbrMovesDone = nbrMovesDone;
	this->reversibleMovesInRow = other.reversibleMovesInRow;

	return *this;
}

BoardRepresentation::BoardRepresentation(const BoardRepresentation & other)
{
	for (int i = 0; i<64; ++i)
		this->board[i] = other.board[i];
	this->canBlackCastle = other.canBlackCastle;
	this->canWhiteCastle = other.canWhiteCastle;
	this->isEnPensantPossible = other.isEnPensantPossible;
	this->isWhiteTurn = other.isWhiteTurn;
	this->justLooseRightCastle = other.justLooseRightCastle;
	this->lastCaptures = other.lastCaptures;
	this->lastReversibleMovesinRow = other.lastReversibleMovesinRow;
	this->nbrMovesDone = nbrMovesDone;
	this->reversibleMovesInRow = other.reversibleMovesInRow;
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
void BoardRepresentation::makeMove(Move move)
{  
	assert(isPieceWhite(this->board[move.from]) == this->isWhiteTurn);
	assert(!isPieceNone(this->board[move.from]));
	assert(isPieceNone(this->board[move.to]) ||
		(isPieceWhite(this->board[move.to]) != this->isWhiteTurn));
	
	this->lastEnPassantMoves[nbrMovesDone] = isEnPensantPossible; //Help for unmake move

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
	else if(MoveGeneration::isEnPassant(*this,move))
	{
		//Check that the en passant is legal. 
		//To check if the opposite pawn is really at the side of the pawn, we should have the special cases of the pawns at the limite of the board. We wont do it. Anyway, Move Generation should have already checked it.
		assert(this->isEnPensantPossible.first == true);
		assert(this->isEnPensantPossible.second + 1 == move.from || this->isEnPensantPossible.second + -1 == move.from);
		swap<Piece>(board, move.from, move.to); //Move the piece
		this->board[this->isEnPensantPossible.second] = Piece::none; //Remove captured piece
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
	else if (MoveGeneration::isEnPassant(*this, move))
	{
		assert(this->isEnPensantPossible.first == false);
		swap<Piece>(board, move.from, move.to); //Move the piece
		//Put back the pawn captured last move by en passant
		Piece capturedEnPassantPawn = this->isWhiteTurn ? Piece::whitePawn : Piece::blackPawn;
		this->board[this->isEnPensantPossible.second] = capturedEnPassantPawn;
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

	auto lastEnPassant = this->lastEnPassantMoves.find(nbrMovesDone - 1);
	this->isEnPensantPossible = lastEnPassant->second;
	this->lastEnPassantMoves.erase(lastEnPassant);

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
