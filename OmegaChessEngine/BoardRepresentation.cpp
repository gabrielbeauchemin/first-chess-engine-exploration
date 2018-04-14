#include "BoardRepresentation.h"
//#define NDEBUG 
#include <cassert>
#include <iostream>
#include <string>
#include <sstream>
#include <cstdio>
#include <cstdlib>
#include <cctype>
#include <fstream>
#include <algorithm>

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
	for(size_t indexRow =0; indexRow < 8; ++indexRow)
	{
		std::string row = rowPieces[indexRow];
		int indexBoard = (7 - indexRow) * 8; //Begin by the row at the end
		std::clog << " row: " << row; //To do: remove Hack: In release, optimiser is too aggressive on the variable row and destroy the validity of the function. This line does allow optimisation on variable row
		for (size_t indexCase = 0; indexCase < row.size(); )
		{
			char currentCase = row[indexCase];
			//Number between 1 to 8 indicated the number of empty cases
			if (isdigit(currentCase) != 0)
			{
				int nbrEmptyCases = atoi(&currentCase);
				assert(nbrEmptyCases > 0 && nbrEmptyCases < 9);
				for (int temp = 0; temp < nbrEmptyCases; ++temp)
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
	{
		this->isWhiteTurn = true;
	}
	else
	{
		this->isWhiteTurn = false;
	}

	// 3)Possible rook
	this->canWhiteKingCastle = false;
	this->canWhiteQueenCastle = false;
	this->canBlackKingCastle = false;
	this->canBlackQueenCastle = false;
	if (fenSplitted[2][0] != '-')
	{
		if (fenSplitted[2].find('K') != std::string::npos)
		{
			this->canWhiteKingCastle = true;
		}
		if (fenSplitted[2].find('Q') != std::string::npos)
		{
			this->canWhiteQueenCastle = true;
		}
		if (fenSplitted[2].find('k') != std::string::npos)
		{
			this->canBlackKingCastle = true;
		}
		if (fenSplitted[2].find('q') != std::string::npos)
		{
			this->canBlackQueenCastle = true;
		}
	}
	
	// 4)Possible en Passant
	this->isEnPensantPossible = std::pair<bool, int>{ false, -1 };
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

std::string BoardRepresentation::toFen()
{
	std::string fen{};
	//Fen Notation has 6 parts:
	// 1)Pieces Position
	//rook(tour), knight(cavalier), bishop(fou), queen(dame), king(roi) et pawn(pion).
	static std::map<Piece, char> pieceToChar{ { Piece::blackRook,'r' },{ Piece::blackKnight, 'n' },{ Piece::blackBishop,  'b' },{ Piece::blackQueen, 'q' },{ Piece::blackKing, 'k' },{Piece::blackPawn,  'p' },{ Piece::whiteRook, 'R' },{ Piece::whiteKnight, 'N' },{ Piece::whiteBishop, 'B' },{ Piece::whiteQueen, 'Q' },{ Piece::whiteKing, 'K' },{ Piece::whitePawn, 'P' } };
	for (int indexRow = 7; indexRow >= 0; --indexRow)
	{
		//For each row
		for (size_t indexCase = indexRow * 8; indexCase < (indexRow * 8) +8;)
		{
			//Current Case is an empty case
			if (isPieceNone(this->board[indexCase]))
			{
				size_t nbrEmptyCases = 0;
				for (; indexCase < (indexRow * 8) + 8; ++indexCase)
				{
					if (isPieceNone(this->board[indexCase]))
					{
						nbrEmptyCases++;
					}
					else break;
				}
				fen += std::to_string(nbrEmptyCases);
			}
			//Current Case is a piece
			else
			{
				fen += pieceToChar[this->board[indexCase]];
				indexCase++;
			}
		}
		if(indexRow != 0)
		   fen += "/"; //One row done
	}
	// 2)Who is turn to play
	if (this->isWhiteTurn)
	{
		fen += " w";
	}
	else
	{
		fen += " b";
	}

	// 3)Possible rook
	if (this->canWhiteKingCastle == false && this->canWhiteQueenCastle == false &&
		this->canBlackKingCastle == false && this->canBlackQueenCastle == false)
	{
		fen += " -";
	}
	else
	{
		fen += " ";
		if (this->canWhiteKingCastle)
		{
			fen += "K";
		}
		if (this->canWhiteQueenCastle)
		{
			fen += "Q";
		}
		if (this->canBlackKingCastle)
		{
			fen += "k";
		}
		if (this->canBlackQueenCastle)
		{
			fen += "q";
		}
	}

	// 4)Possible en Passant
	if (this->isEnPensantPossible.first)
	{
		static std::unordered_map<int, std::string> rowIndexToAlgebraic{ { 0,"a" },{ 1,"b" },{ 2,"c" },{ 3,"d" },{ 4,"e" },{ 5,"f" },{ 6,"g" },{ 7,"h" } };
		std::string algebraicNotation;
		const int COLUMN_SIZE = 8;
		int columnIndex = this->isEnPensantPossible.second % COLUMN_SIZE;
		int rowIndex = this->isEnPensantPossible.second / COLUMN_SIZE;
		algebraicNotation = rowIndexToAlgebraic[columnIndex];
		algebraicNotation += std::to_string(rowIndex + 1);
		fen += " " + algebraicNotation;
	}
	else
	{
		fen += " -";
	}

	// 5)Number of reversible moves
	fen += " " + std::to_string(this->reversibleMovesInRow);

	// 6)Number of total complete moves
	//Not used for now

	return fen;
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
	for (int i = 8; i != 16; ++i)
	{
		board[i] = Piece::whitePawn;
	}
	for (int i = 16; i != 48; ++i)
	{
		board[i] = Piece::none;
	}
	for (int i = 48; i != 56; ++i)
	{
		board[i] = Piece::blackPawn;
	}

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
	for (Piece& p : this->board)
	{
		p = Piece::none;
	}

	//Place the pieces requested
	for (auto& pieceToPlace : piecesToPlace)
	{
		this->board[pieceToPlace.first] = pieceToPlace.second;
	}
}

BoardRepresentation& BoardRepresentation::operator=(const BoardRepresentation& other)
{
	for (int i = 0; i < 64; ++i)
	{
		this->board[i] = other.board[i];
	}
	this->canBlackKingCastle = other.canBlackKingCastle;
	this->canBlackQueenCastle = other.canBlackQueenCastle;
	this->canWhiteKingCastle = other.canWhiteKingCastle;
	this->canWhiteQueenCastle = other.canWhiteQueenCastle;
	this->isEnPensantPossible = other.isEnPensantPossible;
	this->isWhiteTurn = other.isWhiteTurn;
	this->lastReversibleMovesinRow = other.lastReversibleMovesinRow;
	this->currentDepth = other.currentDepth;
	this->reversibleMovesInRow = other.reversibleMovesInRow;

	return *this;
}

BoardRepresentation::BoardRepresentation(const BoardRepresentation & other)
{
	for (int i = 0; i < 64; ++i)
	{
		this->board[i] = other.board[i];
	}
	this->canBlackKingCastle = other.canBlackKingCastle;
	this->canBlackQueenCastle = other.canBlackQueenCastle;
	this->canWhiteKingCastle = other.canWhiteKingCastle;
	this->canWhiteQueenCastle = other.canWhiteQueenCastle;
	this->isEnPensantPossible = other.isEnPensantPossible;
	this->isWhiteTurn = other.isWhiteTurn;
	this->lastReversibleMovesinRow = other.lastReversibleMovesinRow;
	this->currentDepth = other.currentDepth;
	this->reversibleMovesInRow = other.reversibleMovesInRow;
}

bool BoardRepresentation::operator==(const BoardRepresentation & other)
{
	for (int i = 0; i < 64; ++i)
	{
		if (this->board[i] != other.board[i])
		{
			return false;
		}
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
	
	if (isMoveCastling(move)) //Case Castling:
	{
		this->lastBoardPos[this->currentDepth] = this->toFen(); //Help for unmakeMove
		swap<Piece>(board, move.from, move.to); //Swap King
		bool isKingSideCastling = (move.from - move.to < 0);
		int rookPos = 1;
		if (this->isWhiteTurn && isKingSideCastling)
		{
			rookPos = 7;
		}
		else if (this->isWhiteTurn && !isKingSideCastling)
		{
			rookPos = 0;
		}
		else if (!this->isWhiteTurn && isKingSideCastling)
		{
			rookPos = 63;
		}
		else if (!this->isWhiteTurn && !isKingSideCastling)
		{
			rookPos = 56;
		}
		assert(rookPos != -1);

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
		//To check if the opposite pawn is really at the side of the pawn, we should have the special cases of the pawns at the limit of the board. We wont do it. Anyway, Move Generation should have already checked it.
		if (this->isEnPensantPossible.first == true)
		{
			assert(this->isEnPensantPossible.second + 1 == move.from || this->isEnPensantPossible.second + -1 == move.from);
			this->lastBoardPos[this->currentDepth] = this->toFen(); //Help for unmakeMove
			swap<Piece>(board, move.from, move.to); //Move the piece
			this->board[this->isEnPensantPossible.second] = Piece::none; //Remove captured piece
		}
	}
	else //Not Castling, normal move
	{
		//King moves looses the right to castle
		if (isPieceKing(board[move.from]))
		{
			if (isWhiteTurn)
			{
				this->lastBoardPos[this->currentDepth] = this->toFen(); //Help for unmakeMove
				if (this->canWhiteQueenCastle)
				{
					this->canWhiteQueenCastle = false;
				}
				if (this->canWhiteKingCastle)
				{
					this->canWhiteKingCastle = false;
				}
			}
			else
			{
				this->lastBoardPos[this->currentDepth] = this->toFen(); //Help for unmakeMove
				if (this->canBlackQueenCastle)
				{
					this->canBlackQueenCastle = false;
				}
				if (this->canBlackKingCastle)
				{
					this->canBlackKingCastle = false;
				}
			}
		}

		//Move a rook looses the right to castle from the side of the rook
		if (isPieceRook(board[move.from]))
		{
			bool isKingRook = move.from < move.to;
			if (isWhiteTurn)
			{
				this->lastBoardPos[this->currentDepth] = this->toFen(); //Help for unmakeMove
				if (isKingRook && this->canWhiteKingCastle)
				{
					this->canWhiteKingCastle = false;
				}
				else if(this->canWhiteQueenCastle)
				{
					this->canWhiteQueenCastle = false;
				}
			}
			else if (!isWhiteTurn)
			{
				this->lastBoardPos[this->currentDepth] = this->toFen(); //Help for unmakeMove
				if (isKingRook)
				{
					this->canBlackKingCastle = false;
				}
				else
				{
					this->canBlackQueenCastle = false;
				}
			}
		}

		//In case of Capture
		if ( !isPieceNone(board[move.to]))
		{
			this->lastBoardPos[this->currentDepth] = this->toFen(); //Help for unmakeMove
			board[move.to] = Piece::none;
		}

		//Move the piece
		swap<Piece>(board, move.from, move.to);

		//Promotion
		if (!isPieceNone(move.promotion))
		{
			board[move.to] = move.promotion;
		}
	}
	
	isWhiteTurn = !this->isWhiteTurn;
	if (!isPiecePawn(board[move.to]))
	{
		++this->reversibleMovesInRow;
	}
	else
	{
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
	assert(isPieceWhite(this->board[move.to]) != this->isWhiteTurn);
	
	//If the past move remove castling rights, did en passant or captured a piece, 
	//Restore the board before the move
	auto lastProblematicBoardPos = this->lastBoardPos.find(currentDepth-1);
	if (lastProblematicBoardPos != this->lastBoardPos.end())
	{
		BoardRepresentation boardBeforeMove = BoardRepresentation(lastProblematicBoardPos->second);
		for (int i = 0; i < 64; ++i)
		{
			this->board[i] = boardBeforeMove.board[i];
		}
		this->canBlackKingCastle = boardBeforeMove.canBlackKingCastle;
		this->canBlackQueenCastle = boardBeforeMove.canBlackQueenCastle;
		this->canWhiteKingCastle = boardBeforeMove.canWhiteKingCastle;
		this->canWhiteQueenCastle = boardBeforeMove.canWhiteQueenCastle;
		this->reversibleMovesInRow = boardBeforeMove.reversibleMovesInRow;
		this->isEnPensantPossible = boardBeforeMove.isEnPensantPossible;
		this->currentDepth = currentDepth - 1;
		this->isWhiteTurn = !this->isWhiteTurn;
		lastBoardPos.erase(lastProblematicBoardPos);
		return;
	}
	else //Normal move
	{
		assert(isPieceNone(this->board[move.from]));

		//King moves looses the right to castle
		if (isPieceKing(board[move.from]))
		{
			if (isWhiteTurn)
			{
				this->canWhiteKingCastle = true;
				this->canWhiteQueenCastle = true;
			}
			else
			{
				this->canBlackKingCastle = true;
				this->canBlackQueenCastle = true;
			}
		}
		else if (isPieceRook(board[move.from]))
		{
			if (isWhiteTurn)
			{
			    this->canWhiteKingCastle = true;
				this->canWhiteQueenCastle = true;
			}
			else
			{
				this->canBlackKingCastle = true;
				this->canBlackQueenCastle = true;
			}
		}

		//Move the piece
		swap<Piece>(board, move.to, move.from);


		//In case of Promotion
		if (!isPieceNone(move.promotion))
		{
			if (isPieceWhite(move.promotion))
			{
				board[move.from] = Piece::whitePawn;
			}
			else
			{
				board[move.from] = Piece::blackPawn;
			}
		}
	}

	isWhiteTurn = !this->isWhiteTurn;
	if (!isPiecePawn(board[move.from]))
	{
		--this->reversibleMovesInRow;
	}
	else
	{
		this->reversibleMovesInRow = lastReversibleMovesinRow;
	}

	--currentDepth;
}

std::string BoardRepresentation::toString()
{
	std::string stringBoard;
	auto pieceToChar = [](Piece p) ->char
	{
		char charPiece;
		if (isPieceBishop(p))
		{
			charPiece = 'b';
		}
		else if (isPieceKing(p))
		{
			charPiece = 'k';
		}
		else if (isPieceKnight(p))
		{
			charPiece = 'n';
		}
		else if (isPieceNone(p))
		{
			charPiece = ' ';
		}
		else if (isPiecePawn(p))
		{
			charPiece = 'p';
		}
		else if (isPieceQueen(p))
		{
			charPiece = 'q';

		}
		else if (isPieceRook(p))
		{
			charPiece = 'r';
		}
		else
		{
			assert(false);
		}

		if (isPieceWhite(p))
		{
			return toupper(charPiece);
		}
		
		return charPiece;
	};

	char charPieces[64];
	for (int i = 0; i < 64; ++i)
	{
		charPieces[i] = pieceToChar(this->board[i]);
	}

	int nextPiece = 64;
	auto getNextPiece = [&nextPiece, &charPieces] {return std::string(1,charPieces[--nextPiece]); };
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

void BoardRepresentation::clearLastMovesMetaData()
{
	this->currentDepth = 0;
	this->lastBoardPos.clear();
}

bool BoardRepresentation::isMoveCastling(Move move)
{
	static Move allCastlingMoves[]{ Move{4,6}, Move{ 4,2 }, Move{ 60,62 }, Move{ 60,58 } };
	for (auto& m : allCastlingMoves)
	{
		if (m == move)
		{
			return true;
		}
	}
	
	return false;
}

bool BoardRepresentation::movesPermitEnPassant(Move lastMove)
{
	//En passant only if pawn go forward of two cases
	if (abs(lastMove.from - lastMove.to) != 16)
	{
		return false;
	}
	if (!isPiecePawn(board[lastMove.to]))
	{
		return false;
	}

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

	//Else: Pawn in the middles, check both side of them
	bool oppositePawnLeftSide = (isPiecePawn(board[lastMove.to + 1]) &&
			                    isPieceWhite(board[lastMove.to + 1]) != isPieceWhite(board[lastMove.to])); //Pawn to the side must be from opposite camp
	bool oppositePawnRightSide = (isPiecePawn(board[lastMove.to - 1]) &&
			                        isPieceWhite(board[lastMove.to - 1]) != isPieceWhite(board[lastMove.to])); //Pawn to the side must be from opposite camp
	//At least one pawn from opposite camp should be to its side to permit en passant
	return oppositePawnLeftSide || oppositePawnRightSide;
	
}

template<class T>
void BoardRepresentation::swap(T array[], int i, int j)
{
	T temp = array[i];
	array[i] = array[j];
	array[j] = temp;
}

