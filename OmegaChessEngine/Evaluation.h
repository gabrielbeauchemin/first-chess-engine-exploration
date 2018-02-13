#ifndef Evaluation_H
#define Evaluation_H

//The matrices for the values of the pieces and their position are based of this article:
//https ://chessprogramming.wikispaces.com/Simplified+evaluation+function
class Evaluation
{
private:
	static const int pawnValue;
	static const int bishopValue;
	static const int knightValue;
	static const int rookValue;
	static const int queenValue;
	static const int kingValue;

	static const int whitePawnPositionValue[];
	static const int blackPawnPositionValue[];
	static const int bishopPositionValue[];
	static const int knightPositionValue[];
	static const int rookPositionValue[];
	static const int queenPositionValue[];
	static const int kingPositionValue[];

};

#endif

