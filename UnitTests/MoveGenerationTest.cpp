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
			Assert::AreEqual(nbrPossibleMoves, 4);
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

		TEST_METHOD(TestSimpleKingInCheked)
		{
			/* Tests with rook*/
			Piece whiteKing = Piece{ PieceType::king, true };
			Piece blackRook = Piece{ PieceType::rook, false };
			std::vector<std::pair<int, Piece>> kingRookCheck{  
			{2, whiteKing } ,{ 58, blackRook } };
			std::vector<std::pair<int, Piece>> kingRookCheck2{ 
			{2, whiteKing } ,{ 7, blackRook } };
			BoardRepresentation boardRepresentation1{ kingRookCheck };
			BoardRepresentation boardRepresentation2{ kingRookCheck2 };
			Assert::IsTrue(isKingCheck(boardRepresentation1, 2));
			Assert::IsTrue(isKingCheck(boardRepresentation2, 2));

			/* Tests with bishop*/
			Piece blackBishop = Piece{ PieceType::bishop, false };
			std::vector<std::pair<int, Piece>> kingBishopCheck{
				{ 26, whiteKing } ,{ 8, blackBishop } };
			std::vector<std::pair<int, Piece>> kingBishopCheck2{
				{ 26, whiteKing } ,{ 5, blackBishop } };
			BoardRepresentation boardRepresentation3{ kingBishopCheck };
			BoardRepresentation boardRepresentation4{ kingBishopCheck };
			Assert::IsTrue(isKingCheck(boardRepresentation3, 26));
			Assert::IsTrue(isKingCheck(boardRepresentation4, 26));

			/* Tests with Queen*/
			Piece blackQueen = Piece{ PieceType::queen, false };
			std::vector<std::pair<int, Piece>> kingQueenCheck{
				{ 7, whiteKing } ,{ 56, blackQueen } };
			std::vector<std::pair<int, Piece>> kingQueenCheck2{
				{ 7, whiteKing } ,{ 47, blackQueen } };
			BoardRepresentation boardRepresentation5{ kingQueenCheck };
			BoardRepresentation boardRepresentation6{ kingQueenCheck2 };
			Assert::IsTrue(isKingCheck(boardRepresentation5, 7));
			Assert::IsTrue(isKingCheck(boardRepresentation6, 7));

			/* Tests with Knight*/
			Piece blacKnight = Piece{ PieceType::knight, false };
			std::vector<std::pair<int, Piece>> kingKnightCheck{
				{ 4, whiteKing } ,{ 21, blacKnight } };
			std::vector<std::pair<int, Piece>> kingKnightCheck2{
				{ 4, whiteKing } ,{ 10, blacKnight } };
			BoardRepresentation boardRepresentation7{ kingKnightCheck };
			BoardRepresentation boardRepresentation8{ kingKnightCheck2 };
			Assert::IsTrue(isKingCheck(boardRepresentation7, 4));
			Assert::IsTrue(isKingCheck(boardRepresentation8, 4));

			/* Tests with Pawn*/
			Piece blackPawn = Piece{ PieceType::pawn, false };
			std::vector<std::pair<int, Piece>> kingPawnCheck{
				{ 4, whiteKing } ,{ 11, blackPawn } };
			std::vector<std::pair<int, Piece>> kingPawnCheck2{
				{ 4, whiteKing } ,{ 13, blackPawn } };
			BoardRepresentation boardRepresentation9{ kingPawnCheck };
			BoardRepresentation boardRepresentation10{ kingPawnCheck2 };
			Assert::IsTrue(isKingCheck(boardRepresentation9, 4));
			Assert::IsTrue(isKingCheck(boardRepresentation10, 4));

			/*Tests king should not be check*/
			std::vector<std::pair<int, Piece>> kingNotCheck{
				{ 50, whiteKing } ,{ 35, blackQueen },{ 51, blackPawn },
				{ 53, blackRook },{ 40, blackBishop },{ 58, blacKnight } };
			BoardRepresentation boardRepresentation11{ kingNotCheck };
			BoardRepresentation boardWithBeginningPos;
			auto t = isKingCheck(boardWithBeginningPos, 4 == false);
			Assert::IsTrue(isKingCheck(boardRepresentation11, 50) == false);
			//Test with a board with the beginning position
			Assert::IsTrue(isKingCheck(boardWithBeginningPos, 4) == false);
		}

	};
}