#include "stdafx.h"
#include "CppUnitTest.h"
#include "../OmegaChessEngine/BoardRepresentation.h"
#include "../OmegaChessEngine/BoardRepresentation.cpp"
#include "string"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests
{
	TEST_CLASS(BoardRepresentationTest)
	{
	public:

		TEST_METHOD(TestToString)
		{
			BoardRepresentation board;
			std::string stringBoard = board.toString();

			const std::string validStringBoard =
				"#################################################\r\n"
				"#     #     #     #     #     #     #     #     #\r\n"
				"#  r  #  n  #  b  #  k  #  q  #  b  #  n  #  r  #\r\n"
				"#     #     #     #     #     #     #     #     #\r\n"
				"#################################################\r\n"
				"#     #     #     #     #     #     #     #     #\r\n"
				"#  p  #  p  #  p  #  p  #  p  #  p  #  p  #  p  #\r\n"
				"#     #     #     #     #     #     #     #     #\r\n"
				"#################################################\r\n"
				"#     #     #     #     #     #     #     #     #\r\n"
				"#     #     #     #     #     #     #     #     #\r\n"
				"#     #     #     #     #     #     #     #     #\r\n"
				"#################################################\r\n"
				"#     #     #     #     #     #     #     #     #\r\n"
				"#     #     #     #     #     #     #     #     #\r\n"
				"#     #     #     #     #     #     #     #     #\r\n"
				"#################################################\r\n"
				"#     #     #     #     #     #     #     #     #\r\n"
				"#     #     #     #     #     #     #     #     #\r\n"
				"#     #     #     #     #     #     #     #     #\r\n"
				"#################################################\r\n"
				"#     #     #     #     #     #     #     #     #\r\n"
				"#     #     #     #     #     #     #     #     #\r\n"
				"#     #     #     #     #     #     #     #     #\r\n"
				"#################################################\r\n"
				"#     #     #     #     #     #     #     #     #\r\n"
				"#  p  #  p  #  p  #  p  #  p  #  p  #  p  #  p  #\r\n"
				"#     #     #     #     #     #     #     #     #\r\n"
				"#################################################\r\n"
				"#     #     #     #     #     #     #     #     #\r\n"
				"#  r  #  n  #  b  #  k  #  q  #  b  #  n  #  r  #\r\n"
				"#     #     #     #     #     #     #     #     #\r\n"
				"#################################################\r\n";

			Assert::AreEqual(stringBoard, validStringBoard);
		}

		TEST_METHOD(TestBoardInit)
		{
			BoardRepresentation board;

			Assert::IsTrue(board.isWhiteTurn);
			Assert::IsFalse(board.isEnPensantPossible.first);
			Assert::IsTrue(board.canBlackRook && board.canWhiteRook);
			Assert::AreEqual(board.reversibleMovesInRow, 0);

			//Check if the board contains at total the good number of each type of piece
			int nbrRook=0, nbrBishop=0, nbrKnight=0, nbrKing=0, nbrQueen=0, nbrPawn=0, nbrEmptyCase=0;
			for (auto& e: board.board)
			{
				if (e.type == PieceType::bishop)
					++nbrBishop;
				else if (e.type == PieceType::knight)
					++nbrKnight;
				else if (e.type == PieceType::king)
					++nbrKing;
				else if (e.type == PieceType::queen)
					++nbrQueen;
				else if (e.type == PieceType::pawn)
					++nbrPawn;
				else if (e.type == PieceType::rook)
					++nbrRook;
				else
					++nbrEmptyCase;
			}

			Assert::AreEqual(nbrRook, 4);
			Assert::AreEqual(nbrBishop, 4);
			Assert::AreEqual(nbrKnight, 4);
			Assert::AreEqual(nbrKing, 2);
			Assert::AreEqual(nbrQueen, 2);
			Assert::AreEqual(nbrPawn, 16);
			Assert::AreEqual(nbrEmptyCase, 32);
		}
	};
}