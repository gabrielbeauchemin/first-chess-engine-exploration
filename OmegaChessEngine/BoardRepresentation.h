#ifndef BoardRepresentation_H
#define BoardRepresentation_H

#include <utility>  //For pair and make_pair
#include "Piece.h"
#include "Move.h"
#include <String>
#include "MoveGeneration.hpp"

class BoardRepresentation
{
public:
	BoardRepresentation();

	BoardRepresentation(std::vector<std::pair<int, Piece>> piecesToPlace);

	//Should not usde assignement because it is costly to do an memory access. 
	//Use unmakeMove method instead
	BoardRepresentation & operator= (const BoardRepresentation) = delete;
	BoardRepresentation(const BoardRepresentation&) = delete;

	Piece board[64];
	bool isWhiteTurn;
	std::pair<bool, int> isEnPensantPossible; //is it possible and if yes which pawn position can be eaten en passant
	bool canBlackCastle;
	bool canWhiteCastle;
	int reversibleMovesInRow; //For the 50 moves rules
	
	void move(Move move);
	void unmakeMove(Move move);
	std::string toString();

private: 
	/*Help for unmakeMove*/
	Piece lastCapture = Piece::none; 
	bool justLooseRightCastle = false;
	int lastReversibleMovesinRow;

	template<class T>
	void swap(T array[], int i, int j);
	bool isMoveCastling(Move move);
	bool movesPermitEnPassant(Move move);
};

#endif


