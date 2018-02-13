#include "stdafx.h"
#include "CppUnitTest.h"
#include "../OmegaChessEngine/BoardRepresentation.h"
#include "../OmegaChessEngine/BoardRepresentation.cpp"
#include "../OmegaChessEngine/MoveGeneration.hpp"
#include "../OmegaChessEngine/MoveGeneration.cpp"
#include "../OmegaChessEngine/Piece.h"
#include "string"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace MoveGeneration;

namespace UnitTests
{
	TEST_CLASS(BoardRepresentationTest)
	{
	public:

		TEST_METHOD(ToString)
		{
			BoardRepresentation board;
			std::string stringBoard = board.toString();

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

			Assert::AreEqual(stringBoard, validStringBoard);
		}

		TEST_METHOD(BoardInit)
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
				if (isPieceBishop(e))
					++nbrBishop;
				else if (isPieceKnight(e))
					++nbrKnight;
				else if (isPieceKing(e))
					++nbrKing;
				else if (isPieceQueen(e))
					++nbrQueen;
				else if (isPiecePawn(e))
					++nbrPawn;
				else if (isPieceRook(e))
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

		TEST_METHOD(PlayCompleteGame)
		{
			//Random game that includes all kind of moves to test the board representation
		    //It includes a kings side castle, an en passant, a promotion, a checkmate
			//And moves from the bishop, the knights, the rook, the queen and the pawn
			Move gameMoves[] = { Move{12,20}, Move{49,33}, Move{6,21}, Move{33,25}, Move{10,26}, Move{25,18}, Move{11,27}, Move{18,9}, Move{5,26}, Move{9,2,Piece::blackQueen}, Move{3,2}, Move{57,40}, Move{4,6}, Move{50,34}, Move{2,10}, Move{58,49}, Move{10,37}, Move{56, 58}, Move{1,18}, Move{58,42}, Move{37,53} };
							
			//Play the game and check after each move if eveyrthing is ok
			BoardRepresentation boardRepresentation;
			for (auto& move : gameMoves)
			{
				boardRepresentation.move(move);

				//Special cases to check:

				//CheckMate (last move of the game (37,53))
				if (move.from == 37 && move.to == 53)
				{
					Assert::IsTrue(isPieceAttacked(boardRepresentation, move.to));
					auto kingMoves = generateKingMoves(boardRepresentation, 60);
					Assert::AreEqual(0, (int)kingMoves.size());
				}
				else 
				{
					//En Passant( Move Before En passant (10,26), should permit the En passant move
					if (move.from == 10 && move.to == 26)
					{
						Assert::IsTrue(boardRepresentation.isEnPensantPossible.first);
					}
					//Promotion move (9,2)
					else if (move.from == 9 && move.to == 2)
					{
						Assert::IsTrue(isPieceQueen(boardRepresentation.board[2]));
						Assert::IsTrue(!isPieceWhite(boardRepresentation.board[2]));
					}
					//King Castle (4,6)
					else if (move.from == 4 && move.to == 6)
					{
						Assert::IsFalse(boardRepresentation.canWhiteCastle);
						Assert::IsTrue(boardRepresentation.canBlackCastle);
					}
				}
			}
			std::string validEndGame =
				"#################################################\r\n"
				"#     #     #     #     #     #     #     #     #\r\n"
				"#     #     #     #  q  #  k  #  b  #  n  #  r  #\r\n"
				"#     #     #     #     #     #     #     #     #\r\n"
				"#################################################\r\n"
				"#     #     #     #     #     #     #     #     #\r\n"
				"#  p  #  b  #     #  p  #  p  #  q  #  p  #  p  #\r\n"
				"#     #     #     #     #     #     #     #     #\r\n"
				"#################################################\r\n"
				"#     #     #     #     #     #     #     #     #\r\n"
				"#  n  #     #  r  #     #     #     #     #     #\r\n"
				"#     #     #     #     #     #     #     #     #\r\n"
				"#################################################\r\n"
				"#     #     #     #     #     #     #     #     #\r\n"
				"#     #     #  p  #     #     #     #     #     #\r\n"
				"#     #     #     #     #     #     #     #     #\r\n"
				"#################################################\r\n"
				"#     #     #     #     #     #     #     #     #\r\n"
				"#     #     #  b  #  p  #     #     #     #     #\r\n"
				"#     #     #     #     #     #     #     #     #\r\n"
				"#################################################\r\n"
				"#     #     #     #     #     #     #     #     #\r\n"
				"#     #     #  n  #     #  p  #  n  #     #     #\r\n"
				"#     #     #     #     #     #     #     #     #\r\n"
				"#################################################\r\n"
				"#     #     #     #     #     #     #     #     #\r\n"
				"#  p  #     #     #     #     #  p  #  p  #  p  #\r\n"
				"#     #     #     #     #     #     #     #     #\r\n"
				"#################################################\r\n"
				"#     #     #     #     #     #     #     #     #\r\n"
				"#  r  #     #     #     #     #  r  #  k  #     #\r\n"
				"#     #     #     #     #     #     #     #     #\r\n"
				"#################################################\r\n";
			auto t = boardRepresentation.toString();
			Assert::AreEqual(boardRepresentation.toString(), validEndGame);
		}

		TEST_METHOD(MoveKingCastling)
		{
			/* White King side Castling*/
			BoardRepresentation kingSideCastling{};
			kingSideCastling.board[5] = Piece::none;
			kingSideCastling.board[6] = Piece::none;
			kingSideCastling.move(Move{ 4,6 });
			Assert::IsTrue(isPieceNone(kingSideCastling.board[4]));
			Assert::IsTrue(isPieceKing(kingSideCastling.board[6]));
			Assert::IsTrue(isPieceRook(kingSideCastling.board[5]));

			/* White Queen side Castling*/
			BoardRepresentation queenSideCastling{};
			queenSideCastling.board[1] = Piece::none;
			queenSideCastling.board[2] = Piece::none;
			queenSideCastling.board[3] = Piece::none;
			queenSideCastling.move(Move{ 4,2 });
			Assert::IsTrue(isPieceNone(queenSideCastling.board[4]));
			Assert::IsTrue(isPieceKing(queenSideCastling.board[2]));
			Assert::IsTrue(isPieceRook(queenSideCastling.board[3]));

			/* Black King side Castling*/
			BoardRepresentation blackKingSideCastling{};
			blackKingSideCastling.board[61] = Piece::none;
			blackKingSideCastling.board[62] = Piece::none;
			blackKingSideCastling.isWhiteTurn = false;
			blackKingSideCastling.move(Move{ 60,62 });
			Assert::IsTrue(isPieceNone(blackKingSideCastling.board[60]));
			Assert::IsTrue(isPieceKing(blackKingSideCastling.board[62]));
			Assert::IsTrue(isPieceRook(blackKingSideCastling.board[61]));

			/* Black Queen side Castling*/
			BoardRepresentation blackQueenSideCastling{};
			blackQueenSideCastling.board[59] = Piece::none;
			blackQueenSideCastling.board[58] = Piece::none;
			blackQueenSideCastling.board[57] = Piece::none;
			blackQueenSideCastling.isWhiteTurn = false;
			blackQueenSideCastling.move(Move{ 60,58 });
			Assert::IsTrue(isPieceNone(blackQueenSideCastling.board[60]));
			Assert::IsTrue(isPieceKing(blackQueenSideCastling.board[58]));
			Assert::IsTrue(isPieceRook(blackQueenSideCastling.board[59]));
		}
	};

	

}