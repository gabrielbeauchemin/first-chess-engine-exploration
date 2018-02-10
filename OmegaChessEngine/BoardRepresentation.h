#ifndef BoardRepresentation_H
#define BoardRepresentation_H

#include <utility>  //For pair and make_pair
#include "Piece.h"
#include "Move.h"
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
	
	void move(Move move);
	std::string toString();

private: 
	template<class T>
	void swap(T array[], int i, int j);
	bool isMoveCastling(Move move);
	bool movesPermitEnPassant(Move move);
};

#endif


