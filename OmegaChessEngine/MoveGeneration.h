#ifndef MoveGeneration_H
#define MoveGeneratiom_H

#include "BoardRepresentation.h"
#include "Notation.h"
#include "Move.h"
#include <vector>

class MoveGeneration
{
public:
	
	static bool isStateLegal(BoardRepresentation state, Notation lastMove);
	static std::vector<Move> generateMoves(BoardRepresentation board);
	static std::vector<Move> generateRookMoves(BoardRepresentation board, int rookCase);
	static std::vector<Move> generateKnightMoves(BoardRepresentation board, int knightCase);
	static std::vector<Move> generateBishopMoves(BoardRepresentation board, int bishopCase);
	static std::vector<Move> generateKingMoves(BoardRepresentation board, int kingCase);
	static std::vector<Move> generateQueenMoves(BoardRepresentation board, int queenCase);
	static std::vector<Move> generatePawnMoves(BoardRepresentation board, int pawnCase);


};

#endif
