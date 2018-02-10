class BoardRepresentation;
#include "BoardRepresentation.h"
#include "Move.h"
#include <vector>

namespace MoveGeneration
{
	std::vector<Move> generateMoves(const BoardRepresentation& board);
	std::vector<Move> generateRookMoves(const BoardRepresentation& board, int rookCase);
	std::vector<Move> generateKnightMoves(const BoardRepresentation& board, int knightCase);
	std::vector<Move> generateBishopMoves(const BoardRepresentation& board, int bishopCase);
	std::vector<Move> generateKingMoves(const BoardRepresentation& board, int kingCase);
	std::vector<Move> generateCastlingMoves(const BoardRepresentation& board, int kingCase);
	std::vector<Move> generateQueenMoves(const BoardRepresentation& board, int queenCase);
	std::vector<Move> generatePawnMoves(const BoardRepresentation& board, int pawnCase);
	Move generatePawnMoves(const BoardRepresentation& board, int pawnCase, PieceType promotion);

	bool isPieceAttacked(const BoardRepresentation& board, int pieceCase);
	bool isPieceInAbsolutePin(BoardRepresentation board, int pieceCase, int kingCase);
	bool isPawnMovePromotion(const BoardRepresentation& boardRepresentation, int pawnCase);
};
