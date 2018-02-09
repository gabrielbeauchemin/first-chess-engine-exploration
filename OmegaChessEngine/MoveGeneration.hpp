class BoardRepresentation;
#include "BoardRepresentation.h"
#include "Notation.h"
#include <vector>

namespace MoveGeneration
{
	std::vector<Notation> generateMoves(const BoardRepresentation& board);
	std::vector<Notation> generateRookMoves(const BoardRepresentation& board, int rookCase);
	std::vector<Notation> generateKnightMoves(const BoardRepresentation& board, int knightCase);
	std::vector<Notation> generateBishopMoves(const BoardRepresentation& board, int bishopCase);
	std::vector<Notation> generateKingMoves(const BoardRepresentation& board, int kingCase);
	std::vector<Notation> generateCastlingMoves(const BoardRepresentation& board, int kingCase);
	std::vector<Notation> generateQueenMoves(const BoardRepresentation& board, int queenCase);
	std::vector<Notation> generatePawnMoves(const BoardRepresentation& board, int pawnCase);
	Notation generatePawnMoves(const BoardRepresentation& board, int pawnCase, PieceType promotion);

	bool isPieceAttacked(const BoardRepresentation& board, int pieceCase);
	bool isPieceInAbsolutePin(BoardRepresentation board, int pieceCase, int kingCase);
	bool isPawnMovePromotion(const BoardRepresentation& boardRepresentation, int pawnCase);
};
