#include "AlphaBeta.h"
#include "Evaluation.h"
#include <iostream>

AlphaBeta::AlphaBeta()
{
}

AlphaBeta::AlphaBeta(std::function<int(BoardRepresentation&)> getHeuristic, std::function<std::vector<Move>(BoardRepresentation&)> generateLegalMoves, std::function<bool(BoardRepresentation&)> stopSearching)
	:getHeuristic{getHeuristic},
	generateLegalMoves{generateLegalMoves},
	stopSearching{stopSearching}
{
}

std::pair<int, Move> AlphaBeta::maxValue(BoardRepresentation& boardRepresentation, bool makesWhiteWin, int alpha, int beta)
{
	if (this->stopSearching(boardRepresentation))
	{
		return std::pair<int, Move>(this->getHeuristic(boardRepresentation), Move{ -1, -1 });
	}

	//Black wants the biggest negative score and white want the biggest positive score
	//Max player want the best move depending of its camp
	int bestScore = makesWhiteWin  ? -1 * Evaluation::biggestEvaluation : Evaluation::biggestEvaluation;
	Move bestMove{ -1,-1 };
	std::vector<Move> possibleMoves = this->generateLegalMoves(boardRepresentation);
	for (Move& m : possibleMoves)
	{
		boardRepresentation.makeMove(m);
		int score = minValue(boardRepresentation, makesWhiteWin, alpha, beta);
		boardRepresentation.unmakeMove(m);
		if (makesWhiteWin)
		{
			if (bestScore <= score)
			{
				bestScore = score;
				bestMove = m;
			}
			if (bestScore > alpha)
			{
				alpha = bestScore;
			}
			if (beta <= alpha)
			{
				break;
			}
		}
		else
		{
			if (bestScore >= score)
			{
				bestScore = score;
				bestMove = m;
			}
			if (bestScore < alpha)
			{
				alpha = bestScore;
			}
			if (beta >= alpha)
			{
				break;
			}
		}
	}

	return std::pair<int, Move>(bestScore, bestMove);
}

int AlphaBeta::minValue(BoardRepresentation& boardRepresentation, bool makesWhiteWin, int alpha, int beta)
{
	if (this->stopSearching(boardRepresentation))
	{
		return this->getHeuristic(boardRepresentation);
	}

	//Black wants the biggest negative score and white want the biggest positive score
	//Min player want the best move depending of its camp
	int bestScore = makesWhiteWin ? Evaluation::biggestEvaluation : -1 *Evaluation::biggestEvaluation;
	std::vector<Move> possibleMoves = this->generateLegalMoves(boardRepresentation);
	for (Move& m : possibleMoves)
	{
		boardRepresentation.makeMove(m);
		int score = maxValue(boardRepresentation, makesWhiteWin, alpha, beta).first;
		boardRepresentation.unmakeMove(m);
		if (makesWhiteWin)
		{
			if (bestScore >= score)
			{
				bestScore = score;
			}
			if (bestScore < beta)
			{
				beta = bestScore;
			}
			if (beta <= alpha)
			{
				break;
			}
		}
		else
		{
			if (bestScore <= score)
		    {
				bestScore = score;
			}
			if (bestScore > beta)
			{
				beta = bestScore;
			}
			if (beta >= alpha)
			{
				break;
			}
		}

	}

	return bestScore;
}

std::pair<int,Move> AlphaBeta::run(BoardRepresentation& boardRepresentation)
{
	bool makesWhiteWin = boardRepresentation.isWhiteTurn;
	if (makesWhiteWin)
	{
		return maxValue(boardRepresentation, makesWhiteWin, -1 * Evaluation::biggestEvaluation, Evaluation::biggestEvaluation);
	}
	else
	{
		return maxValue(boardRepresentation, makesWhiteWin, Evaluation::biggestEvaluation, -1* Evaluation::biggestEvaluation);
	}
	
}
