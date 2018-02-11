class BoardRepresentation;
#include "BoardRepresentation.h"
#include "Move.h"
#include <vector>

namespace MoveGeneration
{
	std::vector<Move> generateMoves(const BoardRepresentation& boardRepresentation);
	std::vector<Move> generateRookMoves(const BoardRepresentation& boardRepresentation, int rookCase);
	std::vector<Move> generateKnightMoves(const BoardRepresentation& boardRepresentation, int knightCase);
	std::vector<Move> generateBishopMoves(const BoardRepresentation& boardRepresentation, int bishopCase);
	std::vector<Move> generateKingMoves(const BoardRepresentation& boardRepresentation, int kingCase);
	std::vector<Move> generateCastlingMoves(const BoardRepresentation& boardRepresentation, int kingCase);
	std::vector<Move> generateQueenMoves(const BoardRepresentation& boardRepresentation, int queenCase);
	std::vector<Move> generatePawnMoves(const BoardRepresentation& boardRepresentation, int pawnCase);
	std::vector<Move> generatePawnMoves(const BoardRepresentation& boardRepresentation, int pawnCase, PieceType promotion);

	bool isPieceAttacked(const BoardRepresentation& boardRepresentation, int pieceCase);
	bool isPieceInAbsolutePin(BoardRepresentation boardRepresentation, int pieceCase, int kingCase);
	bool isPawnMovePromotion(const BoardRepresentation& boardRepresentation, int pawnCase);
	void filterMovesUncheckingKing(const BoardRepresentation& boardRepresentation, std::vector<Move>& movesToFilter, int kingCase);
	bool isKingCheckmate(const BoardRepresentation& boardRepresentation, int kingCase);
	bool isKingStealMate(const BoardRepresentation& boardRepresentation, int kingCase);

};
