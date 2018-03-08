#include "Search.h"
#include "Evaluation.h"
#include "MoveGeneration.hpp"




Search::Search(int msMaxByMove)
	: timer{msMaxByMove}
{
}

Move Search::run(BoardRepresentation b)
{
	int currentDepth = 2;
	Move bestMoveFound{ -1,-1 };
	int bestHeuristic =  b.isWhiteTurn ? -1 * Evaluation::biggestEvaluation : Evaluation::biggestEvaluation;

	timer.reset();
	while (true)
	{
		this->minMax = MinMax{ Evaluation::evaluate, MoveGeneration::generateMoves,
			[&](BoardRepresentation b) {return timer.isTimeOut() || b.getCurrentDepth() > currentDepth; } };
		std::pair<int, Move> res = this->minMax.run(b);
		if (b.isWhiteTurn)
		{
			if (bestHeuristic < res.first)
			{
				bestHeuristic = res.first;
				bestMoveFound = res.second;
			}
		}
		else
		{
			if (bestHeuristic > res.first)
			{
				bestHeuristic = res.first;
				bestMoveFound = res.second;
			}
		}

		if (timer.isTimeOut()) break;
		++currentDepth;
	}
	
	return bestMoveFound;
}

void Search::stop()
{
	timer.forceTimeOut();
}

void Search::setTimeMax(int msMaxByMove)
{
	timer.setTimeMax(msMaxByMove);
}
