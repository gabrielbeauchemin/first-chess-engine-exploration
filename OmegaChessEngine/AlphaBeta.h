#ifndef AlphaBeta_H
#define AlphaBeta_H 

#include <functional>
#include "BoardRepresentation.h"

class AlphaBeta
{
public: 
	AlphaBeta();
	AlphaBeta(std::function<int(BoardRepresentation&)> getHeuristic,
	std::function<std::vector<Move>(BoardRepresentation&)> generateLegalMoves,
	std::function<bool(BoardRepresentation&)> stopSearching);

	std::pair<int,Move> run(BoardRepresentation& boardRepresentation);

private:
	std::function<int(BoardRepresentation&)> getHeuristic;
	std::function<std::vector<Move>(BoardRepresentation&)> generateLegalMoves;
	std::function<bool(BoardRepresentation&)> stopSearching;

	std::pair<int, Move> maxValue(BoardRepresentation& boardRepresentation, bool makesWhiteWin, int alpha, int beta); //Represent the side of the board of the engine
	int minValue(BoardRepresentation& boardRepresentation, bool makesWhiteWin, int alpha, int beta); //Represent the opposite player
};

#endif

