#include "Evaluation.h"

const int Evaluation::bishopValue = 330;
const int Evaluation::knightValue = 320;
const int Evaluation::pawnValue = 100;
const int Evaluation::rookValue = 500;
const int Evaluation::kingValue = 20000;
const int Evaluation::queenValue = 900;


// pawn
//0, 0, 0, 0, 0, 0, 0, 0,
//50, 50, 50, 50, 50, 50, 50, 50,
//10, 10, 20, 30, 30, 20, 10, 10,
//5, 5, 10, 25, 25, 10, 5, 5,
//0, 0, 0, 20, 20, 0, 0, 0,
//5, -5, -10, 0, 0, -10, -5, 5,
//5, 10, 10, -20, -20, 10, 10, 5,
//0, 0, 0, 0, 0, 0, 0, 0

const int Evaluation::whitePawnPositionValue { 0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0
0,0,0,0,0,0,0,0
0,0,0,0,0,0,0,0
0,0,0,0,0,0,0,0
0,0,0,0,0,0,0,0
0,0,0,0,0,0,0,0
0,0,0,0,0,0,0,0 };



