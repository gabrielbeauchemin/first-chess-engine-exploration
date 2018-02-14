#ifndef Evaluation_H
#define Evaluation_H

#include "BoardRepresentation.h"
#include <vector>
#include <unordered_map>



//The matrices for the values of the pieces and their position are based of this article:
//https ://chessprogramming.wikispaces.com/Simplified+evaluation+function
class Evaluation
{
private:
	static const int infinity;

	static const int pawnValue;
	static const int bishopValue;
	static const int knightValue;
	static const int rookValue;
	static const int queenValue;
	static const int kingValue;

	static const std::vector<int> whitePawnPositionValue;
	static const std::vector<int> blackPawnPositionValue;
	static const std::vector<int> whiteBishopPositionValue;
	static const std::vector<int> blackBishopPositionValue;
	static const std::vector<int> knightPositionValue; //Same thing for white or black
	static const std::vector<int> whiteRookPositionValue;
	static const std::vector<int> blackRookPositionValue;
	static const std::vector<int> whiteQueenPositionValue;
	static const std::vector<int> blackQueenPositionValue;
	static const std::vector<int> whiteKingMiddleGamePositionValue;
	static const std::vector<int> blackKingMiddleGamePositionValue;
	static const std::vector<int> whiteKingEndGamePositionValue;
	static const std::vector<int> blackKingEndGamePositionValue;

	static const std::unordered_map <Piece, int> piecesValue;
	static const std::unordered_map <Piece, std::vector<int>> positionsValue;

	static bool isEndGame(BoardRepresentation& boardRepresentation, std::vector<Piece>& pieces);

public :
	static const int evaluate(BoardRepresentation& boardRepresentation);

};

#endif

