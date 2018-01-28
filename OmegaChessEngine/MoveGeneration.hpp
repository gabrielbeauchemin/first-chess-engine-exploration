class BoardRepresentation;
#include "BoardRepresentation.h"
#include "Notation.h"
#include "Move.h"
#include <vector>

namespace MoveGeneration
{
	std::vector<Move> generateMoves(BoardRepresentation board);
	std::vector<Move> generateRookMoves(BoardRepresentation board, int rookCase);
	std::vector<Move> generateKnightMoves(BoardRepresentation board, int knightCase);
	std::vector<Move> generateBishopMoves(BoardRepresentation board, int bishopCase);
	std::vector<Move> generateKingMoves(BoardRepresentation board, int kingCase);
	std::vector<Move> generateQueenMoves(BoardRepresentation board, int queenCase);
	std::vector<Move> generatePawnMoves(BoardRepresentation board, int pawnCase);

	bool isKingCheck(BoardRepresentation board, int pawnCase);

    
};
