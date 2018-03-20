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
	int currentDepth = 2;
	Move bestMoveFound{ -1,-1 };
	int bestHeuristic =  b.isWhiteTurn ? -1 * Evaluation::biggestEvaluation : Evaluation::biggestEvaluation;

	timer.reset();
	while (true)
	{
		auto stopSearching = [currentDepth,this](BoardRepresentation& b) 
		{
			return /*timer.isTimeOut() ||*/ b.getCurrentDepth() > 0; 
		};
		this->minMax = MinMax{ Evaluation::evaluate, MoveGeneration::generateMoves, stopSearching };
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

		//TO DO:Decomment, to put back deep iterative
		/*if (timer.isTimeOut())*/ break;
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

void Search::setTimeMax(int msMaxByMove)
{
	timer.setTimeMax(msMaxByMove);
}
