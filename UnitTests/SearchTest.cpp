#include "stdafx.h"
#include "CppUnitTest.h"
#include "../OmegaChessEngine/Search.h"
#include "../OmegaChessEngine/Search.cpp"
#include "../OmegaChessEngine/MinMax.cpp"
#include <thread>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests
{
	TEST_CLASS(SearchTest)
	{
	public:

		TEST_METHOD(SearchFromInitialBoard)
		{
			const int msMaxByMove = 5000;
			const int estimation = 1100;
			Search search{ msMaxByMove };
			auto start = std::chrono::system_clock::now();
			Move m = search.run(BoardRepresentation{});
			auto end = std::chrono::system_clock::now();
			auto timePassed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

			Assert::IsTrue(timePassed < msMaxByMove + estimation);
		}

		TEST_METHOD(EnginePlayAgainstItself)
		{
			const int msMaxByMove = 3000;
			const int nbMoves = 3;
			BoardRepresentation boardRepresentation{};
			//std::string allPositions = "";
			for (int i = 0; i < nbMoves; ++i)
			{
				Search search{ msMaxByMove };
				Move m = search.run(boardRepresentation);
				boardRepresentation.makeMove(m);
				//allPositions += boardRepresentation.toString(); + "\r\n";

				int kingIndex;
				for (int i = 0 ;i < 64; ++i)
				{
					if (isPieceKing(boardRepresentation.board[i]) &&
						isPieceWhite(boardRepresentation.board[i]) == boardRepresentation.isWhiteTurn)
					{
						kingIndex = i;
					}
				}
					
				if (MoveGeneration::isKingCheckmate(boardRepresentation, kingIndex))
					break;
			}
		}

		TEST_METHOD(PlayGameAgainstEngine)
		{
			std::vector<Move> moves{Move{12,28}, Move{6,21}, Move{14,22}, Move{5,14}};

			BoardRepresentation boardRepresentation{};
			const int msMaxByMove = 5000;
			for (Move& whiteMove : moves)
			{
				std::string currentBoard = boardRepresentation.toString();
				boardRepresentation.makeMove(whiteMove);
				Search search{ msMaxByMove };
				Move blackMove = search.run(boardRepresentation);
				boardRepresentation.makeMove(blackMove);
			}

		}

		//Test different positions that were failing in arena
		TEST_METHOD(SearchSpecialPositions)
		{
			Search search{ 5000 };

			/*BoardRepresentation boardRepresentation{ "4R3/5Rp1/7p/3B1k2/8/8/1PP4P/1N4K1 b - - 0 24" };
			std::string m = search.run(boardRepresentation).getCoordinateAlgebraicNotation();

			BoardRepresentation boardRepresentation2{ "r1bq1rk1/ppp2ppp/8/3pP3/5B2/P2Q1N2/2P2PPP/R3KB1R b KQ - 2 11" };
			m = search.run(boardRepresentation2).getCoordinateAlgebraicNotation();;

			BoardRepresentation boardRepresentation3{ "r2qkb1r/ppp2ppp/2n1b3/3ppP2/8/6N1/PPPPQ1PP/R1B1KBNR b KQkq - 0 7" };
			m = search.run(boardRepresentation3).getCoordinateAlgebraicNotation();*/

			/*BoardRepresentation boardRepresentation4{ "r2q1rk1/pppbbppp/2nppn2/4P3/1PB5/P1NQ1N2/1BPP1PPP/4RRK1 b - - 0 12" };
			m = search.run(boardRepresentation4).getCoordinateAlgebraicNotation();*/

			//In this position, the move found was illegal, king was eating a protected knight
			BoardRepresentation boardRepresentation5{ "8/R6p/8/8/3k4/1PN5/1K1NB1PP/3R4 b - - 2 25 " };
			auto m = search.run(boardRepresentation5).getCoordinateAlgebraicNotation();

			//From the position, the pawn at g3 is absolutly pined from the black queen.
			//It can eat the queen, but the engine could not see it
			//BoardRepresentation boardRepresentation6{ "3rkb1r/p1pb1ppp/8/3Qp3/5P1q/6P1/PPPP3P/RNB1K1NR w KQk - 1 11" };
			//m = search.run(boardRepresentation6).getCoordinateAlgebraicNotation();
			//Assert::IsTrue("g3h4" == m);

			////Coup en passant in reverse was failing
			//BoardRepresentation boardRepresentation7{ "r2qkb1r/1ppn1pp1/3pp2p/p2PP3/8/1BN2Q2/PB3PPP/1K2R3 b k - 0 19" };
			//m = search.run(boardRepresentation7).getCoordinateAlgebraicNotation();

			////Reverse pawn promotion was failing
			//BoardRepresentation boardRepresentation9{ "r4r2/ppp3pp/3P1pk1/3B4/4P1b1/5N2/PPN2KPP/R6R w - -1 20" };
			//boardRepresentation9.makeMove(Move{43,50});
			//boardRepresentation9.clearLastMovesMetaData();
			//m = search.run(boardRepresentation9).getCoordinateAlgebraicNotation();

			////After pawn eat rook to get a promotion, it was failing
			//BoardRepresentation boardRepresentation10{ "r2r4/ppP3pp/5pk1/3B4/4P3/5K2/PPN3PP/R6R w - - 1 22" };
			//boardRepresentation10.makeMove(Move{ 50,59 });
			//boardRepresentation10.clearLastMovesMetaData();
			//m = search.run(boardRepresentation10).getCoordinateAlgebraicNotation();

			////the two kings could go one next to the other
			//BoardRepresentation boardRepresentation11{ "8/1pp5/3p4/5k1K/p1PP2p1/6PP/PP6/8 b - - 0 14 " };
		 //   m = search.run(boardRepresentation11).getCoordinateAlgebraicNotation();
			 
			//TO DO:Then king rook, moves not found
			//r2r2k1/ppp2ppp/5n2/3ppP2/8/3P1P2/PPPNN1PP/R3K2R w KQ - 1 17 
		}

	};
}