#ifndef BoardRepresentation_H
#define BoardRepresentation_H

#include <utility>  //For pair and make_pair
#include "Piece.h"
#include "Notation.h"
#include <String>

class BoardRepresentation
{
public:
	BoardRepresentation();
	
	Piece board[64];
	bool isWhiteTurn;
	std::pair<bool, Piece> isEnPensantPossible;
	bool canBlackRook;
	bool canWhiteRook;
	int reversibleMovesInRow; //For the 50 moves rules
	
	bool move(BoardRepresentation board, Notation move);
	std::string toString();
};

#endif
