#include "BoardRepresentation.h"
#include "IlegalMoveException.h"
//#define NDEBUG 
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
	static std::map<int, Piece> charToPiece{ {'r', Piece::blackRook},{'n', Piece::blackKnight },{ 'b', Piece::blackBishop },{ 'q', Piece::blackQueen },{ 'k', Piece::blackKing },{ 'p', Piece::blackPawn },{ 'R', Piece::whiteRook },{ 'N', Piece::whiteKnight },{ 'B', Piece::whiteBishop },{ 'Q', Piece::whiteQueen },{ 'K', Piece::whiteKing },{ 'P', Piece::whitePawn } };
	std::vector<std::string> rowPieces = split(fenSplitted[0], '/');
	for(int indexRow =0; indexRow < 8; ++indexRow)
	{
		std::string row = rowPieces[indexRow];
		int indexBoard = (7 - indexRow) * 8; //Begin by the row at the end
		for (int indexCase = 0; indexCase < row.size(); )
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
				}
				++indexCase;
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
		this->canWhiteKingCastle = false;
		this->canWhiteQueenCastle = false;
		this->canBlackKingCastle = false;
		this->canBlackQueenCastle = false;
	}
	else
	{
		this->canWhiteKingCastle = false;
		this->canWhiteQueenCastle = false;
		this->canBlackKingCastle = false;
		this->canBlackQueenCastle = false;

		if (fenSplitted[2].find('K') != std::string::npos)
			this->canWhiteKingCastle = true;
		if (fenSplitted[2].find('Q') != std::string::npos)
			this->canWhiteQueenCastle = true;
		if (fenSplitted[2].find('k') != std::string::npos)
			this->canBlackKingCastle = true;
		if (fenSplitted[2].find('q') != std::string::npos)
			this->canBlackQueenCastle = true;
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
	canBlackKingCastle{ true },
	canBlackQueenCastle{ true },
	canWhiteKingCastle{ true },
	canWhiteQueenCastle{ true },
	reversibleMovesInRow{ 0 },
	currentDepth{0}
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
	canBlackKingCastle{ true },
	canBlackQueenCastle{ true },
	canWhiteKingCastle{ true },
	canWhiteQueenCastle{ true },
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
	this->canBlackKingCastle = other.canBlackKingCastle;
	this->canBlackQueenCastle = other.canBlackQueenCastle;
	this->canWhiteKingCastle = other.canWhiteKingCastle;
	this->canWhiteQueenCastle = other.canWhiteQueenCastle;
	this->isEnPensantPossible = other.isEnPensantPossible;
	this->isWhiteTurn = other.isWhiteTurn;
	this->justLooseRightKingCastle = other.justLooseRightKingCastle;
	this->justLooseRightQueenCastle = other.justLooseRightQueenCastle;
	this->lastCaptures = other.lastCaptures;
	this->lastReversibleMovesinRow = other.lastReversibleMovesinRow;
	this->currentDepth = currentDepth;
	this->reversibleMovesInRow = other.reversibleMovesInRow;

	return *this;
}

BoardRepresentation::BoardRepresentation(const BoardRepresentation & other)
{
	for (int i = 0; i<64; ++i)
		this->board[i] = other.board[i];
	this->canBlackKingCastle = other.canBlackKingCastle;
	this->canBlackQueenCastle = other.canBlackQueenCastle;
	this->canWhiteKingCastle = other.canWhiteKingCastle;
	this->canWhiteQueenCastle = other.canWhiteQueenCastle;
	this->isEnPensantPossible = other.isEnPensantPossible;
	this->isWhiteTurn = other.isWhiteTurn;
	this->justLooseRightKingCastle = other.justLooseRightKingCastle;
	this->justLooseRightQueenCastle = other.justLooseRightQueenCastle;
	this->lastCaptures = other.lastCaptures;
	this->lastReversibleMovesinRow = other.lastReversibleMovesinRow;
	this->currentDepth = currentDepth;
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
	assert(!isPieceNone(this->board[move.from]));
	assert(isPieceWhite(this->board[move.from]) == this->isWhiteTurn);
	assert(isPieceNone(this->board[move.to]) ||
		(isPieceWhite(this->board[move.to]) != this->isWhiteTurn));
	
	this->lastEnPassantMoves[currentDepth] = isEnPensantPossible; //Help for unmake move

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
		{
			this->canWhiteQueenCastle = false;
			this->canWhiteKingCastle = false;
		}
		else
		{
			this->canBlackQueenCastle = false;
			this->canBlackKingCastle = false;
		}
			
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
			
			if (isWhiteTurn)
			{
				this->canWhiteQueenCastle = false;
				this->canWhiteKingCastle = false;
				//Help for unmakeMove
				this->justLooseRightKingCastle = true;
				this->justLooseRightQueenCastle = true;
			}
			else if (!isWhiteTurn)
			{
				this->canBlackQueenCastle = false;
				this->canBlackKingCastle = false;
				//Help for unmakeMove
				this->justLooseRightKingCastle = true;
				this->justLooseRightQueenCastle = true;
			}
		}

		//Move a rook looses the right to castle from the side of the rook
		if (isPieceRook(board[move.from]))
		{
			bool isKingRook = move.from < move.to;
			if (isWhiteTurn)
			{
				if (isKingRook)
				{
					this->canWhiteKingCastle = false;
					this->justLooseRightKingCastle = true;
				}
				else
				{
					this->canWhiteQueenCastle = false;
					this->justLooseRightQueenCastle = true; //Help for unmakeMove
				}
			}
			else if (!isWhiteTurn)
			{
				if (isKingRook)
				{
					this->canBlackKingCastle = false;
					this->justLooseRightKingCastle = true;
				}
				else
				{
					this->canBlackQueenCastle = false;
					this->justLooseRightQueenCastle = true; //Help for unmakeMove
				}
			}
		}

		//In case of Capture
		if ( !isPieceNone(board[move.to]))
		{
			this->lastCaptures[currentDepth] = board[move.to]; //Help for unmakeMove
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

	++currentDepth;
}

void BoardRepresentation::unmakeMove(Move move)
{
	assert(!isPieceNone(board[move.to]));
	assert(isPieceWhite(this->board[move.to]) != this->isWhiteTurn);
		
	if (isMoveCastling(move)) //Case Castling:
	{
		swap<Piece>(board, move.to, move.from); //Swap King
		bool isKingSideCastling = (move.from - move.to < 0) ? true : false;
		int initialRookPos;
		if (this->isWhiteTurn && isKingSideCastling) initialRookPos = 63;
		else if (this->isWhiteTurn && !isKingSideCastling) initialRookPos = 56;
		else if (!this->isWhiteTurn && isKingSideCastling) initialRookPos = 7;
		else if (!this->isWhiteTurn && !isKingSideCastling) initialRookPos = 0;
		int translationRook = (isKingSideCastling) ? -2 : 3;
		swap<Piece>(board, initialRookPos + translationRook, initialRookPos); //Swap Rook

		if (isWhiteTurn)
		{
			this->canBlackKingCastle = true;
			this->canBlackQueenCastle = true;
			this->justLooseRightKingCastle = false;
			this->justLooseRightQueenCastle = false;
		}
		else
		{
			this->canWhiteKingCastle = true;
			this->canWhiteQueenCastle = true;
			this->justLooseRightKingCastle = false;
			this->justLooseRightQueenCastle = false;
		}
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
		if (isPieceKing(board[move.from]))
		{
			if (isWhiteTurn)
			{
				this->canWhiteKingCastle = true;
				this->canWhiteQueenCastle = true;
				this->justLooseRightKingCastle = false;
				this->justLooseRightQueenCastle = false;
			}
			else
			{
				this->canBlackKingCastle = true;
				this->canBlackQueenCastle = true;
				this->justLooseRightKingCastle = false;
				this->justLooseRightQueenCastle = false;
			}
		}
		else if (isPieceRook(board[move.from]))
		{
			if (isWhiteTurn)
			{
				if (this->justLooseRightKingCastle)
				{
					this->canWhiteKingCastle = true;
					this->justLooseRightKingCastle = false;
				}
				else if (this->justLooseRightKingCastle)
				{
					this->canWhiteQueenCastle = true;
					this->justLooseRightQueenCastle = false;
				}

			}
			else
			{
				if (this->justLooseRightKingCastle)
				{
					this->canBlackKingCastle = true;
					this->justLooseRightKingCastle = false;
				}
				else if (this->justLooseRightKingCastle)
				{
					this->canBlackQueenCastle = true;
					this->justLooseRightQueenCastle = false;
				}

			}
		}

		//Move the piece
		swap<Piece>(board, move.to, move.from);

		//In case of Promotion
		if (!isPieceNone(move.promotion))
		{
			if (isWhiteTurn)
				board[move.from] = Piece::whitePawn;
			else board[move.from] = Piece::blackPawn;

		}

		//In case that a piece got Captured the move before
		auto lastCapture = this->lastCaptures.find(currentDepth - 1);
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

	auto lastEnPassant = this->lastEnPassantMoves.find(currentDepth - 1);
	this->isEnPensantPossible = lastEnPassant->second;
	this->lastEnPassantMoves.erase(lastEnPassant);

	--currentDepth;
}

std::string BoardRepresentation::toString()
{
	std::string stringBoard = "";
	auto pieceToChar = [](Piece p) ->char
	{
		char charPiece;
		if (isPieceBishop(p))
			charPiece =  'b';
		else if (isPieceKing(p))
			charPiece = 'k';
		else if (isPieceKnight(p))
			charPiece = 'n';
		else if (isPieceNone(p))
			charPiece = ' ';
		else if (isPiecePawn(p))
			charPiece = 'p';
		else if (isPieceQueen(p))
			charPiece = 'q';
		else if (isPieceRook(p))
			charPiece = 'r';
		else throw std::exception{};

		if (isPieceWhite(p)) return toupper(charPiece);
		else return charPiece;
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

int BoardRepresentation::getCurrentDepth()
{
	return this->currentDepth;
}

void BoardRepresentation::setCurrentDepth(int depth)
{
	this->currentDepth = depth;
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

