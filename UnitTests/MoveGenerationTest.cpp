#include "stdafx.h"
#include "CppUnitTest.h"
#include "../OmegaChessEngine/MoveGeneration.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace MoveGeneration;
namespace UnitTests
{
	TEST_CLASS(MoveGenerationTest)
	{
	public:

		TEST_METHOD(TestSimpleKnightMoves)
		{
			//Place knight in the center at position e4
			Piece knight = Piece{ PieceType::knight, true };
			std::vector<std::pair<int, Piece>> knightCenter{ std::pair<int,Piece> {28, knight } };
			BoardRepresentation boardRepresentation{ knightCenter };
			int nbrPossibleMoves = generateKnightMoves(boardRepresentation, 28).size();
			Assert::AreEqual(nbrPossibleMoves, 8);

			//Place knight in the center at position b2
			std::vector<std::pair<int, Piece>> knightCorner{ std::pair<int,Piece> {9, knight } };
			BoardRepresentation boardRepresentation2{ knightCorner };
			nbrPossibleMoves = generateKnightMoves(boardRepresentation2, 9).size();
			Assert::AreEqual(nbrPossibleMoves, 3);
		}

		TEST_METHOD(TestSimpleBishopMoves)
		{
			//Place knight in the center at position e4
			Piece bishop = Piece{ PieceType::bishop, true };
			std::vector<std::pair<int, Piece>> bishopCenter{ std::pair<int,Piece> {28, bishop } };
			BoardRepresentation boardRepresentation{ bishopCenter };
			int nbrPossibleMoves = generateBishopMoves(boardRepresentation, 28).size();
			Assert::AreEqual(nbrPossibleMoves, 13);

			//Place knight in the center at position b2
			std::vector<std::pair<int, Piece>> bishopCorner{ std::pair<int,Piece> {9, bishop } };
			BoardRepresentation boardRepresentation2{ bishopCorner };
			nbrPossibleMoves = generateBishopMoves(boardRepresentation2, 9).size();
			Assert::AreEqual(nbrPossibleMoves, 9);
		}

		TEST_METHOD(TestSimpleQueenMoves)
		{
			//Place knight in the center at position e4
			Piece queen = Piece{ PieceType::queen, true };
			std::vector<std::pair<int, Piece>> queenCenter{ std::pair<int,Piece> {28, queen } };
			BoardRepresentation boardRepresentation{ queenCenter };
			int nbrPossibleMoves = generateQueenMoves(boardRepresentation, 28).size();
			Assert::AreEqual(nbrPossibleMoves, 27);

			//Place knight in the center at position b2
			std::vector<std::pair<int, Piece>> queenCorner{ std::pair<int,Piece> {9, queen } };
			BoardRepresentation boardRepresentation2{ queenCorner };
			nbrPossibleMoves = generateQueenMoves(boardRepresentation2, 9).size();
			Assert::AreEqual(nbrPossibleMoves, 23);
		}

		TEST_METHOD(TestSimpleKingMoves)
		{
			//Place knight in the center at position e4
			Piece king = Piece{ PieceType::king, true };
			std::vector<std::pair<int, Piece>> kingCenter{ std::pair<int,Piece> {28, king } };
			BoardRepresentation boardRepresentation{ kingCenter };
			int nbrPossibleMoves = generateKingMoves(boardRepresentation, 28).size();
			Assert::AreEqual(nbrPossibleMoves, 8);

			//Place knight in the center at position a1
			std::vector<std::pair<int, Piece>> kingCorner{ std::pair<int,Piece> {0, king } };
			BoardRepresentation boardRepresentation2{ kingCorner };
			nbrPossibleMoves = generateKingMoves(boardRepresentation2, 0).size();
			Assert::AreEqual(nbrPossibleMoves, 3);
		}

	};
}