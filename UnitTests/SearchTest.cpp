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
			const int estimation = 1000;
			Search search{ msMaxByMove };
			auto start = std::chrono::system_clock::now();
			Move m = search.run(BoardRepresentation{});
			auto end = std::chrono::system_clock::now();
			int timePassed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

			Assert::IsTrue(timePassed < msMaxByMove + estimation);
		}

		TEST_METHOD(EnginePlayAgainstItself)
		{
			const int msMaxByMove = 3000;
			const int nbMoves = 70;
			BoardRepresentation boardRepresentation{};
			std::string allPositions = "";
			for (int i = 0; i < nbMoves; ++i)
			{
				Search search{ msMaxByMove };
				Move m = search.run(boardRepresentation);
				boardRepresentation.makeMove(m);
				allPositions += boardRepresentation.toString(); + "\r\n";

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

			BoardRepresentation boardRepresentation{ "4R3/5Rp1/7p/3B1k2/8/8/1PP4P/1N4K1 b - - 0 24" };
			std::string m = search.run(boardRepresentation).getCoordinateAlgebraicNotation();

			BoardRepresentation boardRepresentation2{ "r1bq1rk1/ppp2ppp/8/3pP3/5B2/P2Q1N2/2P2PPP/R3KB1R b KQ - 2 11" };
			m = search.run(boardRepresentation2).getCoordinateAlgebraicNotation();;

			BoardRepresentation boardRepresentation3{ "r2qkb1r/ppp2ppp/2n1b3/3ppP2/8/6N1/PPPPQ1PP/R1B1KBNR b KQkq - 0 7" };
			m = search.run(boardRepresentation3).getCoordinateAlgebraicNotation();

			BoardRepresentation boardRepresentation4{ "r2q1rk1/pppbbppp/2nppn2/4P3/1PB5/P1NQ1N2/1BPP1PPP/4RRK1 b - - 0 12" };
			m = search.run(boardRepresentation4).getCoordinateAlgebraicNotation();

			//In this position, the move found was illegal, king was eaten a protected knight
			BoardRepresentation boardRepresentation5{ "8/R6p/8/8/3k4/1PN5/1K1NB1PP/3R4 b - - 2 25 " };
			m = search.run(boardRepresentation5).getCoordinateAlgebraicNotation();
			Assert::IsFalse("d4c3" == m);
		}

	};
}