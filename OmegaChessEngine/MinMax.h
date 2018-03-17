#ifndef MinMax_H
#define MinMax_H 

#include <functional>
#include "BoardRepresentation.h"

class MinMax
{
public: 
	MinMax();
	MinMax(std::function<int(BoardRepresentation&)> getHeuristic,
	std::function<std::vector<Move>(BoardRepresentation&)> generateLegalMoves,
	std::function<bool(BoardRepresentation&)> stopSearching);

	std::pair<int,Move> run(BoardRepresentation& boardRepresentation);

private:
	std::function<int(BoardRepresentation&)> getHeuristic;
	std::function<std::vector<Move>(BoardRepresentation&)> generateLegalMoves;
	std::function<bool(BoardRepresentation&)> stopSearching;

	std::pair<int, Move> maxValue(BoardRepresentation& boardRepresentation, bool makesWhiteWin); //Represent the side of the board of the engine
	int minValue(BoardRepresentation& boardRepresentation, bool makesWhiteWin); //Represent the opposite player
};

#endif

