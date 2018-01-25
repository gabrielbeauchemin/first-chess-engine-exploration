#ifndef Move_H
#define Move_H

#include <vector>
#include "Piece.h"
#include "Notation.h"

struct Move
{  
	Piece piece;
	std::vector<Notation> freeCases;

	Move(){}

	Move(Piece piece, std::vector<Notation> possibleMoves)
		: piece{piece},
		freeCases{possibleMoves}
	{
	}

	Move(Piece piece): piece{ piece }
	{
	}

	void add(Notation n)
	{
		freeCases.push_back(n);
	}

};

#endif

