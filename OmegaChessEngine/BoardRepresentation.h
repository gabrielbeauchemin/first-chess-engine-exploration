#ifndef BoardRepresentation_H
#define BoardRepresentation_H

#include <utility>  //For pair and make_pair
#include "Piece.h"
#include "Move.h"
#include <String>
#include "MoveGeneration.hpp"
#include <map>

class BoardRepresentation
{
public:
	BoardRepresentation();
	BoardRepresentation(std::string fen);
	BoardRepresentation(std::vector<std::pair<int, Piece>> piecesToPlace);
	BoardRepresentation & operator= (const BoardRepresentation& boardRepresentation);
	BoardRepresentation(const BoardRepresentation& other);

	bool operator==(const BoardRepresentation& other);

	Piece board[64];
	bool isWhiteTurn;
	std::pair<bool, int> isEnPensantPossible; //is it possible and if yes which pawn position can be eaten en passant
	bool canBlackKingCastle;
	bool canBlackQueenCastle;
	bool canWhiteKingCastle;
	bool canWhiteQueenCastle;
	int reversibleMovesInRow; //For the 50 moves rules
	
	void makeMove(Move move);
	void unmakeMove(Move move);
	std::string toString();
	int getCurrentDepth();
	void setCurrentDepth(int depth);

private: 
	/*Help for unmakeMove*/
	std::map<int,Piece> lastCaptures; //nbrMove:Piece
	std::map<int, std::pair<bool, int>> lastEnPassantMoves; //nbrMove: isEnPensantPossible
	bool justLooseRightKingCastle = false;
	bool justLooseRightQueenCastle = false;
	int lastReversibleMovesinRow;
	int currentDepth = 0;
	template<class T>
	void swap(T array[], int i, int j);
	bool isMoveCastling(Move move);
	bool movesPermitEnPassant(Move move);
};

#endif


