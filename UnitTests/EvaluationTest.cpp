#include "stdafx.h"
#include "CppUnitTest.h"
#include "../OmegaChessEngine/BoardRepresentation.h"
#include "../OmegaChessEngine/Evaluation.h"
#include "../OmegaChessEngine/Evaluation.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests
{
	TEST_CLASS(EvaluationTest)
	{
	public:

		TEST_METHOD(EvalCheckmateWhite)
		{
			std::vector<std::pair<int, Piece>> whiteCheckMated
			{ std::pair<int,Piece> {8, Piece::blackRook }, std::pair<int,Piece> {0, Piece::blackRook }, std::pair<int,Piece> {2, Piece::whiteKing } };
			BoardRepresentation boardRepresentation{ whiteCheckMated };
			Assert::AreEqual(-1 *Evaluation::biggestEvaluation, Evaluation::evaluate(boardRepresentation));
		}

		TEST_METHOD(EvalCheckmateBlack)
		{
			std::vector<std::pair<int, Piece>> blackCheckMated
			{ std::pair<int,Piece> {8, Piece::whiteRook }, std::pair<int,Piece> {0, Piece::whiteRook }, std::pair<int,Piece> {2, Piece::blackKing } };
			BoardRepresentation boardRepresentation{ blackCheckMated };
			boardRepresentation.isWhiteTurn = false;
			Assert::AreEqual(Evaluation::biggestEvaluation, Evaluation::evaluate(boardRepresentation));
		}

		TEST_METHOD(EvalStealMateWhite)
		{
			std::vector<std::pair<int, Piece>> whiteStealMate
			{ std::pair<int,Piece> {56, Piece::blackRook }, std::pair<int,Piece> {58, Piece::blackRook }, std::pair<int,Piece> {15, Piece::blackRook }, std::pair<int,Piece> {1, Piece::whiteKing } };
			BoardRepresentation boardRepresentation{ whiteStealMate };
			Assert::AreEqual(0, Evaluation::evaluate(boardRepresentation));
		}

		TEST_METHOD(EvalStealMateBlack)
		{
			std::vector<std::pair<int, Piece>> blackStealMate
			{ std::pair<int,Piece> {56, Piece::whiteRook }, std::pair<int,Piece> {58, Piece::whiteRook }, std::pair<int,Piece> {15, Piece::whiteRook }, std::pair<int,Piece> {1, Piece::blackKing } };
			BoardRepresentation boardRepresentation{ blackStealMate };
			boardRepresentation.isWhiteTurn = false;
			Assert::AreEqual(0, Evaluation::evaluate(boardRepresentation));
		}

		TEST_METHOD(EvalInitialPosition)
		{
			BoardRepresentation boardRepresentation{};
			Assert::AreEqual(0, Evaluation::evaluate(boardRepresentation));
		}

		TEST_METHOD(EvalWhiteAdvantage)
		{
			BoardRepresentation whiteAdvantage{};
			whiteAdvantage.makeMove(Move{ 11,27 });
			whiteAdvantage.makeMove(Move{ 48,40 });
			whiteAdvantage.makeMove(Move{ 6,21 });
			Assert::IsTrue(Evaluation::evaluate(whiteAdvantage) > 0);
		}

		TEST_METHOD(EvalBlackAdvantage)
		{
			BoardRepresentation blackAdvantage{};
			blackAdvantage.makeMove(Move{ 8,16 });
			blackAdvantage.makeMove(Move{ 51,35 });
			blackAdvantage.makeMove(Move{ 16,24 });
			blackAdvantage.makeMove(Move{ 58,36 });
			Assert::IsTrue(Evaluation::evaluate(blackAdvantage) < 0);
		}





	};
}