#ifndef BoardRepresentation_H
#define BoardRepresentation_H

#include <utility>  //For pair and make_pair
#include "Piece.h"
#include "Notation.h"
#include <String>
#include "MoveResult.h"
#include "MoveGeneration.hpp"

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
	
	MoveResult move(Notation move);
	std::string toString();

private: 
	template<class T>
	void swap(T array[], int i, int j);
	bool isMoveARook(Notation move);
	bool isMoveEnPassant(Notation move);
};

#endif


