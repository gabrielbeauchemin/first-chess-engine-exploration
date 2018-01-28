#include "MoveGeneration.hpp"
#include "NotImplementedException.h"

namespace MoveGeneration
{
	static const int mailbox[120] = {
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
		-1,  0,  1,  2,  3,  4,  5,  6,  7, -1,
		-1,  8,  9, 10, 11, 12, 13, 14, 15, -1,
		-1, 16, 17, 18, 19, 20, 21, 22, 23, -1,
		-1, 24, 25, 26, 27, 28, 29, 30, 31, -1,
		-1, 32, 33, 34, 35, 36, 37, 38, 39, -1,
		-1, 40, 41, 42, 43, 44, 45, 46, 47, -1,
		-1, 48, 49, 50, 51, 52, 53, 54, 55, -1,
		-1, 56, 57, 58, 59, 60, 61, 62, 63, -1,
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1
	};

	static const int mailbox64[64] = {
		21, 22, 23, 24, 25, 26, 27, 28,
		31, 32, 33, 34, 35, 36, 37, 38,
		41, 42, 43, 44, 45, 46, 47, 48,
		51, 52, 53, 54, 55, 56, 57, 58,
		61, 62, 63, 64, 65, 66, 67, 68,
		71, 72, 73, 74, 75, 76, 77, 78,
		81, 82, 83, 84, 85, 86, 87, 88,
		91, 92, 93, 94, 95, 96, 97, 98
	};

	static const int knightMoves[] = { -21, -19, -12, -8, 8, 12, 19, 21 };
	static const int bishopMoves[] = { -11,  -9,  9, 11, 0,  0,  0,  0 };
	static const int rookMoves[] = { -10,  -1,  1, 10, 0,  0,  0,  0 };
	static const int queenMoves[] = { -11, -10, -9, -1, 1,  9, 10, 11 };
	static const int kingMoves[] = { -11, -10, -9, -1, 1,  9, 10, 11 };

	bool isStateLegal(BoardRepresentation state, Notation lastMove)
	{
		//Invalid to go to a case that is already occupied
		/*if (board[move.to].type != PieceType::none)
			return MoveResult{ false };*/


		//Check if the cases between the Castle and the king are empty
		/*for (int caseInTheWay = move.from + 1; caseInTheWay < move.to; ++caseInTheWay)
			if (board[caseInTheWay].type != PieceType::none)
				return MoveResult{ false };*/

		//Valid if the king can Castle
		//bool isKingSideCastle = move.from < move.to; //If false, we know its a queen sine castle
		//int translationKingRook = (isKingSideCastle) ? 2 : -3;
		//bool isRookPresent = board[move.from + translationKingRook].type == PieceType::rook;
		//bool canKingCastle = (this->isWhiteTurn) ? canWhiteCastle : canBlackCastle;
		//if (!isRookPresent || !canKingCastle)
		//{
		//	return MoveResult{ false }; //King is not allowed to castle
		//}

		return true;
	}

	std::vector<Move> generateMoves(BoardRepresentation board)
	{
		throw NotImplementedException{};
	}

	std::vector<Move> generateRookMoves(BoardRepresentation board, int rookCase)
	{
		throw NotImplementedException{};
	}

	std::vector<Move> generateKnightMoves(BoardRepresentation board, int knightCase)
	{
		throw NotImplementedException{};
	}

	std::vector<Move> generateBishopMoves(BoardRepresentation board, int bishopCase)
	{
		throw NotImplementedException{};
	}

	std::vector<Move> generateKingMoves(BoardRepresentation board, int kingCase)
	{
		throw NotImplementedException{};
	}

	std::vector<Move> generateQueenMoves(BoardRepresentation board, int queenCase)
	{
		throw NotImplementedException{};
	}

	std::vector<Move> generatePawnMoves(BoardRepresentation board, int pawnCase)
	{
		throw NotImplementedException{};
	}
	bool isKingCheck(BoardRepresentation board, int pawnCase)
	{
		return false;
	}
}

