#include "Search.h"
#include "Evaluation.h"
#include "MoveGeneration.hpp"
#include "iostream"

#include <string>
Search::Search(int msMaxByMove)
	: timer{msMaxByMove}
{
}

Move Search::run(BoardRepresentation& b)
{
	int currentDepth = 1;
	Move bestMoveFound{ -1,-1 };
	int bestHeuristic =  b.isWhiteTurn ? -1 * Evaluation::biggestEvaluation : Evaluation::biggestEvaluation;

	timer.reset();
	while (true)
	{
		b.clearLastMovesMetaData();
		auto stopSearching = [currentDepth,this](BoardRepresentation& b) 
		{
			return this->timer.isTimeOut() || b.getCurrentDepth() > currentDepth;
		};
		this->alphaBeta = AlphaBeta{ Evaluation::evaluate, MoveGeneration::generateMoves, stopSearching };
		std::pair<int, Move> res = this->alphaBeta.run(b);
		if (b.isWhiteTurn)
		{
			if (bestHeuristic <= res.first)
			{
				bestHeuristic = res.first;
				bestMoveFound = res.second;
			}
		}
		else
		{
			if (bestHeuristic >= res.first)
			{
				bestHeuristic = res.first;
				bestMoveFound = res.second;
			}
		}

		if (timer.isTimeOut()) break;
		++currentDepth;
	}
	
	assert(bestMoveFound.from != -1 && bestMoveFound.to != -1);
	return bestMoveFound;
}

void Search::stop()
{
	//For nows, search is used in a synchrone way so nothing is necessary
	//When search will be on a thread: this method will be necessary:
	//timer.forceTimeOut();
}

void Search::setTimeMax(uint64_t msMaxByMove)
{
	timer.setTimeMax(msMaxByMove);
}
