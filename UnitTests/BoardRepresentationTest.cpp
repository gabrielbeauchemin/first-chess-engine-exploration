#include "stdafx.h"
#include "CppUnitTest.h"
#include "../OmegaChessEngine/BoardRepresentation.h"
#include "../OmegaChessEngine/BoardRepresentation.cpp"
#include "../OmegaChessEngine/MoveGeneration.hpp"
#include "../OmegaChessEngine/MoveGeneration.cpp"
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
			/*std::string stringBoard = board.toString();

			const std::string validStringBoard =
				"#################################################\r\n"
				"#     #     #     #     #     #     #     #     #\r\n"
				"#  r  #  n  #  b  #  q  #  k  #  b  #  n  #  r  #\r\n"
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
				"#  r  #  n  #  b  #  q  #  k  #  b  #  n  #  r  #\r\n"
				"#     #     #     #     #     #     #     #     #\r\n"
				"#################################################\r\n";

			Assert::AreEqual(stringBoard, validStringBoard);*/
		}

		TEST_METHOD(TestBoardInit)
		{
			BoardRepresentation board;

			Assert::IsTrue(board.isWhiteTurn);
			Assert::IsFalse(board.isEnPensantPossible.first);
			Assert::IsTrue(board.canBlackCastle && board.canWhiteCastle);
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

		TEST_METHOD(TestPlayCompleteGame)
		{
			//Random game that includes all kind of moves to test the board representation
		    //It includes a kings side castle, an en passant, a promotion, a checkmate
			//And moves from the bishop, the knights, the rook, the queen and the pawn
			Notation gameMoves[] = { Notation{12,20}, Notation{49,33}, Notation{6,21}, Notation{33,25}, Notation{10,26}, Notation{25,18}, Notation{11,27}, Notation{18,9}, Notation{5,26}, Notation{9,2,PieceType::queen}, Notation{3,2}, Notation{57,40}, Notation{4,6}, Notation{50,34}, Notation{2,10}, Notation{58,49}, Notation{10,37}, Notation{56, 58}, Notation{1,18}, Notation{58,42}, Notation{37,53} };
							
			//Play the game and check after each move if eveyrthing is ok
			BoardRepresentation boardRepresentation;
			for (auto& move : gameMoves)
			{
				MoveResult res = boardRepresentation.move(move);

				Assert::IsTrue(res.isMoveLegal);

				//Special cases to check:

				//CheckMate (last move of the game (37,53))
				if (move.from == 37 && move.to == 53)
				{
					Assert::IsTrue(res.isEndGame);
					Assert::IsTrue(res.isWhiteWinner);
				}
				else 
				{
					Assert::IsFalse(res.isEndGame);

					//En Passant( Move Before En passant (10,26), should permit the En passant move
					if (move.from == 10 && move.to == 26)
					{
						Assert::IsTrue(boardRepresentation.isEnPensantPossible.first);
					}
					//Promotion move (9,2)
					else if (move.from == 9 && move.to == 2)
					{
						boardRepresentation.board[2].type = PieceType::queen;
					}
					//King Castle (4,6)
					else if (move.from == 4 && move.to == 6)
					{
						Assert::IsTrue(boardRepresentation.board[6].type == PieceType::king);
						Assert::IsTrue(boardRepresentation.board[5].type == PieceType::rook);
						Assert::IsFalse(boardRepresentation.canWhiteCastle);
						Assert::IsTrue(boardRepresentation.canBlackCastle);
					}
				}
			}

				
		}
	};

	

}