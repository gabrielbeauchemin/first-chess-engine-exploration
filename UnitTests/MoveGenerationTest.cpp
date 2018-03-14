#include "stdafx.h"
#include "CppUnitTest.h"
#include "../OmegaChessEngine/MoveGeneration.hpp"
#include "../OmegaChessEngine/Piece.h"
#include "../OmegaChessEngine/Piece.cpp"
#include "../OmegaChessEngine/BoardRepresentation.h"
#include <assert.h>
#include <thread>
#include <atomic>
#include <mutex>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace MoveGeneration;
namespace UnitTests
{
	TEST_CLASS(MoveGenerationTest)
	{
	public:

		TEST_METHOD(SimpleKnightMoves)
		{
			//Place knight in the center at position e4
			std::vector<std::pair<int, Piece>> knightCenter{ std::pair<int,Piece> {28, Piece::whiteKnight } };
			BoardRepresentation boardRepresentation{ knightCenter };
			int nbrPossibleMoves = generateKnightMoves(boardRepresentation, 28).size();
			Assert::AreEqual(nbrPossibleMoves, 8);

			//Place knight in the center at position b2
			std::vector<std::pair<int, Piece>> knightCorner{ std::pair<int,Piece> {9, Piece::whiteKnight } };
			BoardRepresentation boardRepresentation2{ knightCorner };
			nbrPossibleMoves = generateKnightMoves(boardRepresentation2, 9).size();
			Assert::AreEqual(nbrPossibleMoves, 4);
		}

		TEST_METHOD(SimpleBishopMoves)
		{
			//Place knight in the center at position e4
			std::vector<std::pair<int, Piece>> bishopCenter{ std::pair<int,Piece> {28, Piece::whiteBishop } };
			BoardRepresentation boardRepresentation{ bishopCenter };
			int nbrPossibleMoves = generateBishopMoves(boardRepresentation, 28, false).size();
			Assert::AreEqual(nbrPossibleMoves, 13);

			//Place knight in the center at position b2
			std::vector<std::pair<int, Piece>> bishopCorner{ std::pair<int,Piece> {9, Piece::whiteBishop } };
			BoardRepresentation boardRepresentation2{ bishopCorner };
			nbrPossibleMoves = generateBishopMoves(boardRepresentation2, 9, false).size();
			Assert::AreEqual(nbrPossibleMoves, 9);
		}

		TEST_METHOD(SimpleQueenMoves)
		{
			//Place knight in the center at position e4
			std::vector<std::pair<int, Piece>> queenCenter{ std::pair<int,Piece> {28, Piece::whiteQueen } };
			BoardRepresentation boardRepresentation{ queenCenter };
			int nbrPossibleMoves = generateQueenMoves(boardRepresentation, 28, false).size();
			Assert::AreEqual(nbrPossibleMoves, 27);

			//Place knight in the center at position b2
			std::vector<std::pair<int, Piece>> queenCorner{ std::pair<int,Piece> {9, Piece::whiteQueen } };
			BoardRepresentation boardRepresentation2{ queenCorner };
			nbrPossibleMoves = generateQueenMoves(boardRepresentation2, 9, false).size();
			Assert::AreEqual(nbrPossibleMoves, 23);
		}

		TEST_METHOD(SimpleKingMoves)
		{
			//Place knight in the center at position e4
			std::vector<std::pair<int, Piece>> kingCenter{ std::pair<int,Piece> {28, Piece::whiteKing } };
			BoardRepresentation boardRepresentation{ kingCenter };
			int nbrPossibleMoves = generateKingMoves(boardRepresentation, 28).size();
			Assert::AreEqual(nbrPossibleMoves, 8);

			//Place knight in the center at position a1
			std::vector<std::pair<int, Piece>> kingCorner{ std::pair<int,Piece> {0, Piece::whiteKing } };
			BoardRepresentation boardRepresentation2{ kingCorner };
			nbrPossibleMoves = generateKingMoves(boardRepresentation2, 0).size();
			Assert::AreEqual(nbrPossibleMoves, 3);
		}

		TEST_METHOD(KingInCheked)
		{
			/* Rook check the king*/
			std::vector<std::pair<int, Piece>> kingRookCheck{  
			{2, Piece::whiteKing } ,{ 58, Piece::blackRook } };
			std::vector<std::pair<int, Piece>> kingRookCheck2{ 
			{2, Piece::whiteKing } ,{ 7, Piece::blackRook } };
			BoardRepresentation boardRepresentation1{ kingRookCheck };
			BoardRepresentation boardRepresentation2{ kingRookCheck2 };
			Assert::IsTrue(isPieceAttacked(boardRepresentation1, 2));
			Assert::IsTrue(isPieceAttacked(boardRepresentation2, 2));

			/* Bishop check the king*/
			std::vector<std::pair<int, Piece>> kingBishopCheck{
				{ 26, Piece::whiteKing } ,{ 8, Piece::blackBishop } };
			std::vector<std::pair<int, Piece>> kingBishopCheck2{
				{ 26, Piece::whiteKing } ,{ 5, Piece::blackBishop } };
			BoardRepresentation boardRepresentation3{ kingBishopCheck };
			BoardRepresentation boardRepresentation4{ kingBishopCheck };
			Assert::IsTrue(isPieceAttacked(boardRepresentation3, 26));
			Assert::IsTrue(isPieceAttacked(boardRepresentation4, 26));

			/* Queen check the king*/
			std::vector<std::pair<int, Piece>> kingQueenCheck{
				{ 7, Piece::whiteKing } ,{ 56, Piece::blackQueen } };
			std::vector<std::pair<int, Piece>> kingQueenCheck2{
				{ 7, Piece::whiteKing } ,{ 47, Piece::blackQueen } };
			BoardRepresentation boardRepresentation5{ kingQueenCheck };
			BoardRepresentation boardRepresentation6{ kingQueenCheck2 };
			Assert::IsTrue(isPieceAttacked(boardRepresentation5, 7));
			Assert::IsTrue(isPieceAttacked(boardRepresentation6, 7));

			/* Knight check the king*/
			std::vector<std::pair<int, Piece>> kingKnightCheck{
				{ 4, Piece::whiteKing } ,{ 21, Piece::blackKnight } };
			std::vector<std::pair<int, Piece>> kingKnightCheck2{
				{ 4, Piece::whiteKing } ,{ 10, Piece::blackKnight } };
			BoardRepresentation boardRepresentation7{ kingKnightCheck };
			BoardRepresentation boardRepresentation8{ kingKnightCheck2 };
			Assert::IsTrue(isPieceAttacked(boardRepresentation7, 4));
			Assert::IsTrue(isPieceAttacked(boardRepresentation8, 4));

			/* Pawn Check the king*/
			std::vector<std::pair<int, Piece>> kingPawnCheck{
				{ 4, Piece::whiteKing } ,{ 11, Piece::blackPawn } };
			std::vector<std::pair<int, Piece>> kingPawnCheck2{
				{ 4, Piece::whiteKing } ,{ 13, Piece::blackPawn } };
			BoardRepresentation boardRepresentation9{ kingPawnCheck };
			BoardRepresentation boardRepresentation10{ kingPawnCheck2 };
			Assert::IsTrue(isPieceAttacked(boardRepresentation9, 4));
			Assert::IsTrue(isPieceAttacked(boardRepresentation10, 4));

			/*Test king should not be check*/
			std::vector<std::pair<int, Piece>> kingNotCheck{
				{ 50, Piece::whiteKing } ,{ 35, Piece::blackQueen },{ 51, Piece::blackPawn },
				{ 53, Piece::blackRook },{ 40, Piece::blackBishop },{ 58, Piece::blackKnight } };
			BoardRepresentation boardRepresentation11{ kingNotCheck };
			BoardRepresentation boardWithBeginningPos;
			Assert::IsTrue(isPieceAttacked(boardRepresentation11, 50) == false);
			//Test with a board with the beginning position
			Assert::IsTrue(isPieceAttacked(boardWithBeginningPos, 4) == false);
		}


		TEST_METHOD(TestAbsolutePin)
		{
			/* Test with Pawn pinned by a rook*/
			std::vector<std::pair<int, Piece>> pawnPinned{
				{ 12, Piece::whiteKing } ,{ 11, Piece::whitePawn }, {8, Piece::blackRook } };
			BoardRepresentation boardRepresentation{ pawnPinned };
			Assert::IsTrue(isPieceInAbsolutePin(boardRepresentation,11,12));

			/* Test with knight pinned by a bishop*/
			std::vector<std::pair<int, Piece>> knightPinned{
				{ 4, Piece::whiteKing } ,{ 11, Piece::whiteKnight },{ 32, Piece::blackBishop } };
			BoardRepresentation boardRepresentation2{ knightPinned };
			Assert::IsTrue(isPieceInAbsolutePin(boardRepresentation2, 11, 4));
		}

		TEST_METHOD(CastlingMoves)
		{
			/* White King side Castling*/
			BoardRepresentation kingSideCastling{};
			kingSideCastling.board[5] = Piece::none;
			kingSideCastling.board[6] = Piece::none;
			auto possibleCastlings = generateCastlingMoves(kingSideCastling, 4);
			Assert::AreEqual((int)possibleCastlings.size(), 1);
			Assert::AreEqual(possibleCastlings[0].from, 4);
			Assert::AreEqual(possibleCastlings[0].to, 6);

			/* White Queen side Castling*/
			BoardRepresentation queenSideCastling{};
			queenSideCastling.board[1] = Piece::none;
			queenSideCastling.board[2] = Piece::none;
			queenSideCastling.board[3] = Piece::none;
			possibleCastlings = generateCastlingMoves(queenSideCastling, 4);
			Assert::AreEqual((int)possibleCastlings.size(), 1);
			Assert::AreEqual(possibleCastlings[0].from, 4);
			Assert::AreEqual(possibleCastlings[0].to, 2);

			/* Black King side Castling*/
			BoardRepresentation blackKingSideCastling{};
			blackKingSideCastling.board[61] = Piece::none;
			blackKingSideCastling.board[62] = Piece::none;
			blackKingSideCastling.isWhiteTurn = false;
			possibleCastlings = generateCastlingMoves(blackKingSideCastling, 60);
			Assert::AreEqual((int)possibleCastlings.size(), 1);
			Assert::AreEqual(possibleCastlings[0].from, 60);
			Assert::AreEqual(possibleCastlings[0].to, 62);

			/* Black Queen side Castling*/
			BoardRepresentation blackQueenSideCastling{};
			blackQueenSideCastling.board[59] = Piece::none;
			blackQueenSideCastling.board[58] = Piece::none;
			blackQueenSideCastling.board[57] = Piece::none;
			blackQueenSideCastling.isWhiteTurn = false;
			possibleCastlings = generateCastlingMoves(blackQueenSideCastling, 60);
			Assert::AreEqual((int)possibleCastlings.size(), 1);
			Assert::AreEqual(possibleCastlings[0].from, 60);
			Assert::AreEqual(possibleCastlings[0].to, 58);
			
			//Test if No Castle possible
			BoardRepresentation initialBoard{};
			Assert::AreEqual( (int)generateCastlingMoves(initialBoard, 4).size(), 0);
			queenSideCastling.board[1] = Piece::whiteKnight; //only a knight in the way
			Assert::AreEqual((int)generateCastlingMoves(initialBoard, 4).size(), 0);
			queenSideCastling.board[5] = Piece::whiteBishop; //only a bishop in the way
			Assert::AreEqual((int)generateCastlingMoves(initialBoard, 4).size(), 0);

			/* White King can castle both ways*/
			BoardRepresentation bothCastlingPossible{};
			bothCastlingPossible.board[1] = Piece::none;
			bothCastlingPossible.board[2] = Piece::none;
			bothCastlingPossible.board[3] = Piece::none;
			bothCastlingPossible.board[5] = Piece::none;
			bothCastlingPossible.board[6] = Piece::none;
			possibleCastlings = generateCastlingMoves(bothCastlingPossible, 4);
			Assert::AreEqual((int)possibleCastlings.size(), 2);
			Assert::AreEqual(possibleCastlings[0].from, 4);
			Assert::AreEqual(possibleCastlings[0].to, 6);
			Assert::AreEqual(possibleCastlings[1].from, 4);
			Assert::AreEqual(possibleCastlings[1].to, 2);
		}

		TEST_METHOD(PawnMoves)
		{
			/* From initial board, pawns can move of one case or two*/
			BoardRepresentation boardRepresentation{};
			auto pawnMoves = generatePawnMoves(boardRepresentation, 8);
			Assert::AreEqual(2, (int)pawnMoves.size());
			Assert::AreEqual(pawnMoves[0].to, 16);
			Assert::AreEqual(pawnMoves[1].to, 24);
			pawnMoves = generatePawnMoves(boardRepresentation, 12);
			Assert::AreEqual(2, (int)pawnMoves.size());
			Assert::AreEqual(pawnMoves[0].to, 20);
			Assert::AreEqual(pawnMoves[1].to, 28);
			boardRepresentation.isWhiteTurn = false;
			pawnMoves = generatePawnMoves(boardRepresentation, 48);
			Assert::AreEqual(2, (int)pawnMoves.size());
			Assert::AreEqual(pawnMoves[0].to, 40);
			Assert::AreEqual(pawnMoves[1].to, 32);
			pawnMoves = generatePawnMoves(boardRepresentation, 51);
			Assert::AreEqual(2, (int)pawnMoves.size());
			Assert::AreEqual(pawnMoves[0].to, 43);
			Assert::AreEqual(pawnMoves[1].to, 35);
			pawnMoves = generatePawnMoves(boardRepresentation, 55);
			Assert::AreEqual(2, (int)pawnMoves.size());
			Assert::AreEqual(pawnMoves[0].to, 47);
			Assert::AreEqual(pawnMoves[1].to, 39);

			/* Test pawns that has already move can only move of one case*/
			std::vector<std::pair<int, Piece>> pawnsDisposition{
				{ 16, Piece::whitePawn } ,{ 27, Piece::whitePawn },{ 46, Piece::blackPawn } };
			BoardRepresentation boardRepresentation1{ pawnsDisposition };
			pawnMoves = generatePawnMoves(boardRepresentation1, 16);
			Assert::AreEqual(1, (int)pawnMoves.size());
			Assert::AreEqual(24, pawnMoves[0].to);
			pawnMoves = generatePawnMoves(boardRepresentation1, 27);
			Assert::AreEqual(1, (int)pawnMoves.size());
			Assert::AreEqual(35, pawnMoves[0].to);
			boardRepresentation1.isWhiteTurn = false; //Black turn
			pawnMoves = generatePawnMoves(boardRepresentation1, 46);
			Assert::AreEqual(1, (int)pawnMoves.size());
			Assert::AreEqual(38, pawnMoves[0].to);

			/* Test pawn that are blocked*/
			BoardRepresentation initialBoardRepresentation{};
			initialBoardRepresentation.makeMove(Move{ 12,28 });
			initialBoardRepresentation.makeMove(Move{ 52,36 }); //Both pawn cant move
			Assert::AreEqual(0, (int) generatePawnMoves(initialBoardRepresentation, 28).size());
			initialBoardRepresentation.makeMove(Move{ 8,24 }); // So its black turn and prepare to block another 2 pawns
			Assert::AreEqual(0, (int)generatePawnMoves(initialBoardRepresentation, 36).size());
			initialBoardRepresentation.makeMove(Move{ 48,40 });
			initialBoardRepresentation.makeMove(Move{ 24,32 }); //After that move, the two pawn on A row are blocked
			Assert::AreEqual(0, (int)generatePawnMoves(initialBoardRepresentation, 40).size());
			initialBoardRepresentation.isWhiteTurn = true;
			Assert::AreEqual(0, (int)generatePawnMoves(initialBoardRepresentation, 32).size());

			/* Test pawn promotion*/
			std::vector<std::pair<int, Piece>> pawnsPromotionDisposition{
				{ 53, Piece::whitePawn } ,{ 9, Piece::blackPawn } };
			BoardRepresentation boardRepresentation2{ pawnsDisposition };
			Move promotion = generatePawnPromotionMoves(boardRepresentation2, 53, Piece::whiteQueen)[0];
			Assert::IsTrue(promotion.to == 61 && promotion.promotion == Piece::whiteQueen);
			boardRepresentation2.isWhiteTurn = false; //Black turn
			promotion = generatePawnPromotionMoves(boardRepresentation2, 9, Piece::blackKnight)[0];
			Assert::IsTrue(promotion.to == 1 && promotion.promotion == Piece::blackKnight);

			/*Test Pawn Capture*/
			std::vector<std::pair<int, Piece>> pawnsAttack{
				{ 9, Piece::whitePawn } ,{ 16, Piece::blackPawn },{ 18, Piece::blackPawn } };
			BoardRepresentation boardRepresentation3{ pawnsAttack };
			Assert::AreEqual(4, (int)generatePawnMoves(boardRepresentation3, 9).size());
			boardRepresentation3.isWhiteTurn = false;
			Assert::AreEqual(2, (int)generatePawnMoves(boardRepresentation3, 16).size());
			Assert::AreEqual(2, (int)generatePawnMoves(boardRepresentation3, 18).size());
			std::vector<std::pair<int, Piece>> pawnsAttack2{
				{ 43, Piece::blackPawn } ,{ 34, Piece::whitePawn },{ 36, Piece::whitePawn } };
			BoardRepresentation boardRepresentation4{ pawnsAttack2 };
			Assert::AreEqual(2, (int)generatePawnMoves(boardRepresentation4, 34).size());
			Assert::AreEqual(2, (int)generatePawnMoves(boardRepresentation4, 36).size());
			boardRepresentation4.isWhiteTurn = false;
			Assert::AreEqual(3, (int)generatePawnMoves(boardRepresentation4, 43).size());


			/*Test En Passant*/
			BoardRepresentation initialBoardRepresentation2{};
			initialBoardRepresentation2.makeMove(Move{15,31});
			initialBoardRepresentation2.makeMove(Move{ 48,32 });
			initialBoardRepresentation2.makeMove(Move{ 31,39 });
			initialBoardRepresentation2.makeMove(Move{ 54,38 });
			pawnMoves = generatePawnMoves(initialBoardRepresentation2, 39); //First Possible En passant
			Assert::AreEqual(2, (int)pawnMoves.size());
			Assert::IsTrue(pawnMoves[1].from == 39 && pawnMoves[1].to == 46);
			//Prepare black pieces so that two of theyre pawn will be able to do an En passant move
			initialBoardRepresentation2.isWhiteTurn = false;
			initialBoardRepresentation2.makeMove(Move{ 32,24 });
			initialBoardRepresentation2.isWhiteTurn = false;
			initialBoardRepresentation2.makeMove(Move{ 50,34 });
			initialBoardRepresentation2.isWhiteTurn = false;
			initialBoardRepresentation2.makeMove(Move{ 34,26 });
			initialBoardRepresentation2.makeMove(Move{ 9,25 });
			//Black Pawns at position 24 and 26 should be able to capture en passant the white pawn at position 25
			auto firstPawnMoves = generatePawnMoves(initialBoardRepresentation2, 24);
			auto secondPawnMoves = generatePawnMoves(initialBoardRepresentation2, 26);
			Assert::IsTrue(firstPawnMoves.size() == 2);
			Assert::IsTrue(secondPawnMoves.size() == 2);
			Assert::IsTrue(firstPawnMoves[1].from == 24 && firstPawnMoves[1].to == 17);
			Assert::IsTrue(secondPawnMoves[1].from == 26 && secondPawnMoves[1].to == 17);
		}

		TEST_METHOD(ComplexBishopMove)
		{
			//After one move, the white should be able to take out their bishop
			BoardRepresentation initialBoardRep{};
			//Open both white pawns to let the white bishops pass
			initialBoardRep.makeMove(Move{ 12,28 });
			initialBoardRep.makeMove(Move{ 51,43 });
			initialBoardRep.makeMove(Move{ 11,27 });
			initialBoardRep.makeMove(Move{ 43,35 }); 
			auto moves = generateBishopMoves(initialBoardRep, 2, false); //left white bisho
			Assert::AreEqual(5, (int)moves.size());
			moves = generateBishopMoves(initialBoardRep, 5, false); //right white bishop
			Assert::AreEqual(5, (int)moves.size());

		}

		TEST_METHOD(KingCheckMate)
		{
			std::vector<std::pair<int, Piece>> kingCheckMated{
				{ 4, Piece::whiteKing } ,{ 8, Piece::blackRook },{ 7, Piece::blackRook } };
			BoardRepresentation boardRepresentation{ kingCheckMated };
			Assert::IsTrue(isKingCheckmate(boardRepresentation, 4));
		}

		TEST_METHOD(KingStealMate)
		{
			std::vector<std::pair<int, Piece>> kingCheckMated{
				{ 4, Piece::whiteKing } ,{ 8, Piece::blackRook },{ 59, Piece::blackRook },{ 61, Piece::blackRook } };
			BoardRepresentation boardRepresentation{ kingCheckMated };
			Assert::IsTrue(isKingStealMate(boardRepresentation, 4));
		}

		TEST_METHOD(Perft)
		{
			long long nbrNodes = perftParralel(2, BoardRepresentation{});
			Assert::IsTrue(nbrNodes == 400);
			nbrNodes = perftParralel(3, BoardRepresentation{});
			Assert::IsTrue(nbrNodes == 8902);
			//Uncomment for deepest test (takes times)
			/*nbrNodes = perftParralel(4, BoardRepresentation{});
			Assert::IsTrue(nbrNodes == 197281);*/
			/*nbrNodes = perftParralel(5, BoardRepresentation{});
			Assert::IsTrue(nbrNodes == 4865609);*/
		}

		private:
			long long perft(int depth, BoardRepresentation& boardRepresentation)
			{
				long long nbrNodes = 0;
				if (depth == 0) return 1;

				std::vector<Move> moves = generateMoves(boardRepresentation);
				for (auto& currentMove : moves) {
					boardRepresentation.makeMove(currentMove);
					nbrNodes += perft(depth - 1, boardRepresentation);
					boardRepresentation.unmakeMove(currentMove);
				}


				return nbrNodes;
			}

			long long perftParralel(int depth, BoardRepresentation& boardRepresentation)
			{
				std::atomic<long long> nbrNodes = 0;
				std::mutex m;
				unsigned int nbrCore = std::thread::hardware_concurrency();
				std::vector<Move> moves = generateMoves(boardRepresentation);
				int nbrSubTreesPerThread = moves.size() / nbrCore;

				std::vector<std::thread> threads;
				for (int indexThread = 0; indexThread < nbrCore; ++indexThread)
				{
					threads.push_back(std::thread{ [&, indexThread]()
						{
							for (int indexSubtree = indexThread * nbrSubTreesPerThread;
								indexSubtree < (indexThread +1) * nbrSubTreesPerThread;
								++indexSubtree)
							{
								auto moveToMake = moves[indexSubtree];
								m.lock();
								boardRepresentation.makeMove(moveToMake);
								BoardRepresentation copyBoard{ boardRepresentation };
								boardRepresentation.unmakeMove(moveToMake);
								m.unlock();

								nbrNodes += perft(depth-1, copyBoard);
							}
						} 
					});
				}

				for (auto& t : threads)
					t.join();

				return nbrNodes;
			}

			

	};

	
}