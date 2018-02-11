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

		TEST_METHOD(SimpleKnightMoves)
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

		TEST_METHOD(SimpleBishopMoves)
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

		TEST_METHOD(SimpleQueenMoves)
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

		TEST_METHOD(SimpleKingMoves)
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

		TEST_METHOD(KingInCheked)
		{
			/* Rook check the king*/
			Piece whiteKing = Piece{ PieceType::king, true };
			Piece blackRook = Piece{ PieceType::rook, false };
			std::vector<std::pair<int, Piece>> kingRookCheck{  
			{2, whiteKing } ,{ 58, blackRook } };
			std::vector<std::pair<int, Piece>> kingRookCheck2{ 
			{2, whiteKing } ,{ 7, blackRook } };
			BoardRepresentation boardRepresentation1{ kingRookCheck };
			BoardRepresentation boardRepresentation2{ kingRookCheck2 };
			Assert::IsTrue(isPieceAttacked(boardRepresentation1, 2));
			Assert::IsTrue(isPieceAttacked(boardRepresentation2, 2));

			/* Bishop check the king*/
			Piece blackBishop = Piece{ PieceType::bishop, false };
			std::vector<std::pair<int, Piece>> kingBishopCheck{
				{ 26, whiteKing } ,{ 8, blackBishop } };
			std::vector<std::pair<int, Piece>> kingBishopCheck2{
				{ 26, whiteKing } ,{ 5, blackBishop } };
			BoardRepresentation boardRepresentation3{ kingBishopCheck };
			BoardRepresentation boardRepresentation4{ kingBishopCheck };
			Assert::IsTrue(isPieceAttacked(boardRepresentation3, 26));
			Assert::IsTrue(isPieceAttacked(boardRepresentation4, 26));

			/* Queen check the king*/
			Piece blackQueen = Piece{ PieceType::queen, false };
			std::vector<std::pair<int, Piece>> kingQueenCheck{
				{ 7, whiteKing } ,{ 56, blackQueen } };
			std::vector<std::pair<int, Piece>> kingQueenCheck2{
				{ 7, whiteKing } ,{ 47, blackQueen } };
			BoardRepresentation boardRepresentation5{ kingQueenCheck };
			BoardRepresentation boardRepresentation6{ kingQueenCheck2 };
			Assert::IsTrue(isPieceAttacked(boardRepresentation5, 7));
			Assert::IsTrue(isPieceAttacked(boardRepresentation6, 7));

			/* Knight check the king*/
			Piece blacKnight = Piece{ PieceType::knight, false };
			std::vector<std::pair<int, Piece>> kingKnightCheck{
				{ 4, whiteKing } ,{ 21, blacKnight } };
			std::vector<std::pair<int, Piece>> kingKnightCheck2{
				{ 4, whiteKing } ,{ 10, blacKnight } };
			BoardRepresentation boardRepresentation7{ kingKnightCheck };
			BoardRepresentation boardRepresentation8{ kingKnightCheck2 };
			Assert::IsTrue(isPieceAttacked(boardRepresentation7, 4));
			Assert::IsTrue(isPieceAttacked(boardRepresentation8, 4));

			/* Pawn Check the king*/
			Piece blackPawn = Piece{ PieceType::pawn, false };
			std::vector<std::pair<int, Piece>> kingPawnCheck{
				{ 4, whiteKing } ,{ 11, blackPawn } };
			std::vector<std::pair<int, Piece>> kingPawnCheck2{
				{ 4, whiteKing } ,{ 13, blackPawn } };
			BoardRepresentation boardRepresentation9{ kingPawnCheck };
			BoardRepresentation boardRepresentation10{ kingPawnCheck2 };
			Assert::IsTrue(isPieceAttacked(boardRepresentation9, 4));
			Assert::IsTrue(isPieceAttacked(boardRepresentation10, 4));

			/*Test king should not be check*/
			std::vector<std::pair<int, Piece>> kingNotCheck{
				{ 50, whiteKing } ,{ 35, blackQueen },{ 51, blackPawn },
				{ 53, blackRook },{ 40, blackBishop },{ 58, blacKnight } };
			BoardRepresentation boardRepresentation11{ kingNotCheck };
			BoardRepresentation boardWithBeginningPos;
			Assert::IsTrue(isPieceAttacked(boardRepresentation11, 50) == false);
			//Test with a board with the beginning position
			Assert::IsTrue(isPieceAttacked(boardWithBeginningPos, 4) == false);
		}


		TEST_METHOD(TestAbsolutePin)
		{
			/* Test with Pawn pinned by a rook*/
			Piece whiteKing = Piece{ PieceType::king, true };
			Piece whitePawn = Piece{ PieceType::pawn, false };
			Piece blackRook = Piece{ PieceType::rook, false };
			std::vector<std::pair<int, Piece>> pawnPinned{
				{ 12, whiteKing } ,{ 11, whitePawn }, {8, blackRook } };
			BoardRepresentation boardRepresentation{ pawnPinned };
			Assert::IsTrue(isPieceInAbsolutePin(boardRepresentation,11,12));

			/* Test with knight pinned by a bishop*/
			Piece whiteKnight = Piece{ PieceType::knight, false };
			Piece blackBishop = Piece{ PieceType::bishop, false };
			std::vector<std::pair<int, Piece>> knightPinned{
				{ 4, whiteKing } ,{ 11, whiteKnight },{ 32, blackBishop } };
			BoardRepresentation boardRepresentation2{ knightPinned };
			Assert::IsTrue(isPieceInAbsolutePin(boardRepresentation2, 11, 4));
		}

		TEST_METHOD(CastlingMoves)
		{
			/* White King side Castling*/
			BoardRepresentation kingSideCastling{};
			kingSideCastling.board[5].type = PieceType::none;
			kingSideCastling.board[6].type = PieceType::none;
			auto possiblePins = generateCastlingMoves(kingSideCastling, 4);
			Assert::AreEqual((int)possiblePins.size(), 1);
			Assert::AreEqual(possiblePins[0].from, 4);
			Assert::AreEqual(possiblePins[0].to, 6);

			/* White Queen side Castling*/
			BoardRepresentation queenSideCastling{};
			queenSideCastling.board[1].type = PieceType::none;
			queenSideCastling.board[2].type = PieceType::none;
			queenSideCastling.board[3].type = PieceType::none;
			possiblePins = generateCastlingMoves(queenSideCastling, 4);
			Assert::AreEqual((int)possiblePins.size(), 1);
			Assert::AreEqual(possiblePins[0].from, 4);
			Assert::AreEqual(possiblePins[0].to, 2);

			/* Black King side Castling*/
			BoardRepresentation blackKingSideCastling{};
			blackKingSideCastling.board[61].type = PieceType::none;
			blackKingSideCastling.board[62].type = PieceType::none;
			blackKingSideCastling.isWhiteTurn = false;
			possiblePins = generateCastlingMoves(blackKingSideCastling, 60);
			Assert::AreEqual((int)possiblePins.size(), 1);
			Assert::AreEqual(possiblePins[0].from, 60);
			Assert::AreEqual(possiblePins[0].to, 62);

			/* Black Queen side Castling*/
			BoardRepresentation blackQueenSideCastling{};
			blackQueenSideCastling.board[59].type = PieceType::none;
			blackQueenSideCastling.board[58].type = PieceType::none;
			blackQueenSideCastling.board[57].type = PieceType::none;
			blackQueenSideCastling.isWhiteTurn = false;
			possiblePins = generateCastlingMoves(blackQueenSideCastling, 60);
			Assert::AreEqual((int)possiblePins.size(), 1);
			Assert::AreEqual(possiblePins[0].from, 60);
			Assert::AreEqual(possiblePins[0].to, 58);
			
			//Test if No Castle possible
			BoardRepresentation initialBoard{};
			Assert::AreEqual( (int)generateCastlingMoves(initialBoard, 4).size(), 0);

			/* White King can castle both ways*/
			BoardRepresentation bothCastlingPossible{};
			bothCastlingPossible.board[1].type = PieceType::none;
			bothCastlingPossible.board[2].type = PieceType::none;
			bothCastlingPossible.board[3].type = PieceType::none;
			bothCastlingPossible.board[5].type = PieceType::none;
			bothCastlingPossible.board[6].type = PieceType::none;
			possiblePins = generateCastlingMoves(bothCastlingPossible, 4);
			Assert::AreEqual((int)possiblePins.size(), 2);
			Assert::AreEqual(possiblePins[0].from, 4);
			Assert::AreEqual(possiblePins[0].to, 6);
			Assert::AreEqual(possiblePins[1].from, 4);
			Assert::AreEqual(possiblePins[1].to, 2);
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
			auto whitePawn = Piece{ PieceType::pawn, true };
			auto blackPawn = Piece{ PieceType::pawn, false };
			std::vector<std::pair<int, Piece>> pawnsDisposition{
				{ 16, whitePawn } ,{ 27, whitePawn },{ 46, blackPawn } };
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
			initialBoardRepresentation.move(Move{ 12,28 });
			initialBoardRepresentation.move(Move{ 52,36 }); //Both pawn cant move
			Assert::AreEqual(0, (int) generatePawnMoves(initialBoardRepresentation, 28).size());
			initialBoardRepresentation.move(Move{ 8,24 }); // So its black turn and prepare to block another 2 pawns
			Assert::AreEqual(0, (int)generatePawnMoves(initialBoardRepresentation, 36).size());
			initialBoardRepresentation.move(Move{ 48,40 });
			initialBoardRepresentation.move(Move{ 24,32 }); //After that move, the two pawn on A row are blocked
			Assert::AreEqual(0, (int)generatePawnMoves(initialBoardRepresentation, 40).size());
			initialBoardRepresentation.isWhiteTurn = true;
			Assert::AreEqual(0, (int)generatePawnMoves(initialBoardRepresentation, 32).size());

			/* Test pawn promotion*/
			std::vector<std::pair<int, Piece>> pawnsPromotionDisposition{
				{ 53, whitePawn } ,{ 9, blackPawn } };
			BoardRepresentation boardRepresentation2{ pawnsDisposition };
			Move promotion = generatePawnMoves(boardRepresentation2, 53, PieceType::queen)[0];
			Assert::IsTrue(promotion.to == 61 && promotion.promotion == PieceType::queen);
			boardRepresentation2.isWhiteTurn = false; //Black turn
			promotion = generatePawnMoves(boardRepresentation2, 9, PieceType::knight)[0];
			Assert::IsTrue(promotion.to == 1 && promotion.promotion == PieceType::knight);

			/*Test Pawn Capture*/
			std::vector<std::pair<int, Piece>> pawnsAttack{
				{ 9, whitePawn } ,{ 16, blackPawn },{ 18, blackPawn } };
			BoardRepresentation boardRepresentation3{ pawnsAttack };
			Assert::AreEqual(4, (int)generatePawnMoves(boardRepresentation3, 9).size());
			boardRepresentation3.isWhiteTurn = false;
			Assert::AreEqual(2, (int)generatePawnMoves(boardRepresentation3, 16).size());
			Assert::AreEqual(2, (int)generatePawnMoves(boardRepresentation3, 18).size());
			std::vector<std::pair<int, Piece>> pawnsAttack2{
				{ 43, blackPawn } ,{ 34, whitePawn },{ 36, whitePawn } };
			BoardRepresentation boardRepresentation4{ pawnsAttack2 };
			Assert::AreEqual(2, (int)generatePawnMoves(boardRepresentation4, 34).size());
			Assert::AreEqual(2, (int)generatePawnMoves(boardRepresentation4, 36).size());
			boardRepresentation4.isWhiteTurn = false;
			Assert::AreEqual(3, (int)generatePawnMoves(boardRepresentation4, 43).size());


			/*Test En Passant*/
			BoardRepresentation initialBoardRepresentation2{};
			initialBoardRepresentation2.move(Move{15,31});
			initialBoardRepresentation2.move(Move{ 48,32 });
			initialBoardRepresentation2.move(Move{ 31,39 });
			initialBoardRepresentation2.move(Move{ 54,38 });
			pawnMoves = generatePawnMoves(initialBoardRepresentation2, 39); //First Possible En passant
			Assert::AreEqual(2, (int)pawnMoves.size());
			Assert::IsTrue(pawnMoves[1].from == 39 && pawnMoves[1].to == 46);
			//Prepare black pieces so that two of theyre pawn will be able to do an En passant move
			initialBoardRepresentation2.isWhiteTurn = false;
			initialBoardRepresentation2.move(Move{ 32,24 });
			initialBoardRepresentation2.isWhiteTurn = false;
			initialBoardRepresentation2.move(Move{ 50,34 });
			initialBoardRepresentation2.isWhiteTurn = false;
			initialBoardRepresentation2.move(Move{ 34,26 });
			initialBoardRepresentation2.move(Move{ 9,25 });
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
			initialBoardRep.move(Move{ 12,28 });
			initialBoardRep.move(Move{ 51,43 });
			initialBoardRep.move(Move{ 11,27 });
			initialBoardRep.move(Move{ 43,35 }); 
			auto moves = generateBishopMoves(initialBoardRep, 2); //left white bisho
			Assert::AreEqual(5, (int)moves.size());
			moves = generateBishopMoves(initialBoardRep, 5); //right white bishop
			Assert::AreEqual(5, (int)moves.size());

		}

		TEST_METHOD(KingCheckMate)
		{
			auto blackRook = Piece{ PieceType::rook, false };
			auto whiteKing = Piece{ PieceType::king, true };
			std::vector<std::pair<int, Piece>> kingCheckMated{
				{ 4, whiteKing } ,{ 8, blackRook },{ 7, blackRook } };
			BoardRepresentation boardRepresentation{ kingCheckMated };
			Assert::IsTrue(isKingCheckmate(boardRepresentation, 4));
		}

		TEST_METHOD(KingStealMate)
		{
			auto blackRook = Piece{ PieceType::rook, false };
			auto whiteKing = Piece{ PieceType::king, true };
			std::vector<std::pair<int, Piece>> kingCheckMated{
				{ 4, whiteKing } ,{ 8, blackRook },{ 59, blackRook },{ 61, blackRook } };
			BoardRepresentation boardRepresentation{ kingCheckMated };
			Assert::IsTrue(isKingStealMate(boardRepresentation, 4));
		}

		TEST_METHOD(Perft)
		{
			long long nbrNodes = perft(2, BoardRepresentation{});
			Assert::IsTrue(nbrNodes == 400);
			nbrNodes = perft(3, BoardRepresentation{});
			Assert::IsTrue(nbrNodes == 8902);
			//Uncomment for deepest test (takes times)
			/*long long nbrNodes = perft(4, BoardRepresentation{});
			Assert::IsTrue(nbrNodes == 197281);*/
			/*long long nbrNodes = perft(5, BoardRepresentation{});
			Assert::IsTrue(nbrNodes == 4865609);*/
		}

		private:
			//It would it faster with a BoardRepresentation.unmakeMove function
			//For now, we copy the board after each move for simplicity
			long long perft(int depth, BoardRepresentation& boardRepresentation)
			{
				long long nbrNodes = 0;
				if (depth == 0) return 1;
				
				std::vector<Move> moves = generateMoves(boardRepresentation);
				for (auto& currentMove : moves) {
					BoardRepresentation boardRepAfterMove = boardRepresentation;
					boardRepAfterMove.move(currentMove);
					nbrNodes += perft(depth - 1, boardRepAfterMove);
				}

				return nbrNodes;
			}
	};

	
}