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

	BoardRepresentation(std::vector<std::pair<int, Piece>> piecesToPlace);
	
	Piece board[64];
	bool isWhiteTurn;
	std::pair<bool, int> isEnPensantPossible; //is it possible and if yes which pawn position can be eaten en passant
	bool canBlackCastle;
	bool canWhiteCastle;
	int reversibleMovesInRow; //For the 50 moves rules
	
	void move(Notation move);
	std::string toString();

private: 

	template<class T>
	void swap(T array[], int i, int j);
	bool isMoveCastling(Notation move);
	bool movesPermitEnPassant(Notation move);
};

#endif


