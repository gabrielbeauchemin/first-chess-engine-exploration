class BoardRepresentation;
#include "BoardRepresentation.h"
#include "Move.h"
#include <vector>

namespace MoveGeneration
{
	std::vector<Move> generateMoves(BoardRepresentation& boardRepresentation);
	std::vector<Move> generateRookMoves(const BoardRepresentation& boardRepresentation, int rookCase, bool isPiecePinned);
	std::vector<Move> generateKnightMoves(const BoardRepresentation& boardRepresentation, int knightCase);
	std::vector<Move> generateBishopMoves(const BoardRepresentation& boardRepresentation, int bishopCase, bool isPiecePinned);
	std::vector<Move> generateKingMoves(BoardRepresentation& boardRepresentation, int kingCase);
	std::vector<Move> generateCastlingMoves(BoardRepresentation& boardRepresentation, int kingCase);
	std::vector<Move> generateQueenMoves(const BoardRepresentation& boardRepresentation, int queenCase, bool isPiecePinned);
	std::vector<Move> generatePawnMoves(const BoardRepresentation& boardRepresentation, int pawnCase);
	std::vector<Move> generatePawnMoves(const BoardRepresentation& boardRepresentation, int pawnCase, Piece promotion);

	bool isPieceAttacked(BoardRepresentation& boardRepresentation, int pieceCase);
	bool isPieceInAbsolutePin(BoardRepresentation& boardRepresentation, int pieceCase, int kingCase);
	bool isPawnMovePromotion(const BoardRepresentation& boardRepresentation, int pawnCase);
	void filterMovesUncheckingKing(BoardRepresentation& boardRepresentation, std::vector<Move>& movesToFilter, int kingCase);
	bool isKingCheckmate(BoardRepresentation& boardRepresentation, int kingCase);
	bool isKingStealMate(BoardRepresentation& boardRepresentation, int kingCase);
	std::vector<int> getLegalOffsetPinnedPiece(const BoardRepresentation& boardRepresentation, int PinnedPieceCase, std::vector<int> possibleOffsets);
	bool isEnPassant(const BoardRepresentation& boardRepresentation, Move move);
	

	

};
