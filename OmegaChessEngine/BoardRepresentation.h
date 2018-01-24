#ifndef BoardRepresentation_H
#define BoardRepresentation_H

#include <utility>  //For pair and make_pair
#include "Piece.h"
#include "Notation.h"

class BoardRepresentation
{
public:
	BoardRepresentation();
	
	bool isWhiteTurn;
	std::pair<bool, Piece> isEnPensantPossible;
	bool canBlackRook;
	bool canWhiteRook;
	int reversibleMovesInRow; //For the 50 moves rules

	BoardRepresentation move(BoardRepresentation board, Notation move);
	

};

#endif
