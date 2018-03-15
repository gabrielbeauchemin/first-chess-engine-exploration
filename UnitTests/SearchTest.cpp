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
			const int estimation = 1050;
			Search search{ msMaxByMove };
			auto start = std::chrono::system_clock::now();
			Move m = search.run(BoardRepresentation{});
			auto end = std::chrono::system_clock::now();
			int timePassed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

			Assert::IsTrue(timePassed < msMaxByMove + estimation);
		}

		TEST_METHOD(EnginePlayAgainstItself)
		{
			const int msMaxByMove = 10000;
			const int nbMoves = 50;
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

	};
}