class BoardRepresentation;
#include "BoardRepresentation.h"
#include "Notation.h"
#include <vector>

namespace MoveGeneration
{
	std::vector<Notation> generateMoves(BoardRepresentation& board);
	std::vector<Notation> generateRookMoves(BoardRepresentation& board, int rookCase);
	std::vector<Notation> generateKnightMoves(BoardRepresentation& board, int knightCase);
	std::vector<Notation> generateBishopMoves(BoardRepresentation& board, int bishopCase);
	std::vector<Notation> generateKingMoves(BoardRepresentation& board, int kingCase);
	std::vector<Notation> generateCastlingMoves(BoardRepresentation& board, int kingCase);
	std::vector<Notation> generateQueenMoves(BoardRepresentation& board, int queenCase);
	std::vector<Notation> generatePawnMoves(BoardRepresentation& board, int pawnCase);
	Notation generatePawnMoves(BoardRepresentation& board, int pawnCase, PieceType promotion);

	bool isPieceAttacked(BoardRepresentation& board, int pieceCase);
	bool isPieceInAbsolutePin(BoardRepresentation& board, int pieceCase, int kingCase);
	bool isPawnMovePromotion(BoardRepresentation& boardRepresentation, int pawnCase);
};
