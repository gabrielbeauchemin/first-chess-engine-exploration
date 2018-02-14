#include "Evaluation.h"
#include "MoveGeneration.hpp"
#include <limits>

using namespace MoveGeneration;

const int Evaluation::infinity = std::numeric_limits<int>::max();
const int Evaluation::bishopValue = 330;
const int Evaluation::knightValue = 320;
const int Evaluation::pawnValue = 100;
const int Evaluation::rookValue = 500;
const int Evaluation::kingValue = 20000;
const int Evaluation::queenValue = 900;

const std::vector<int> Evaluation::whitePawnPositionValue =
{ 
  0, 0, 0, 0, 0, 0, 0, 0,
  5, 10, 10, -20, -20, 10, 10, 5,
  5, -5, -10, 0, 0, -10, -5, 5,
  0, 0, 0, 20, 20, 0, 0, 0,
  5, 5, 10, 25, 25, 10, 5, 5,
  10, 10, 20, 30, 30, 20, 10, 10,
  50, 50, 50, 50, 50, 50, 50, 50,
  0, 0, 0, 0, 0, 0, 0, 0, 
};

const std::vector<int> Evaluation::blackPawnPositionValue =
{
	0, 0, 0, 0, 0, 0, 0, 0,
	50, 50, 50, 50, 50, 50, 50, 50,
	10, 10, 20, 30, 30, 20, 10, 10,
	5, 5, 10, 25, 25, 10, 5, 5,
	0, 0, 0, 20, 20, 0, 0, 0,
	5, -5, -10, 0, 0, -10, -5, 5,
	5, 10, 10, -20, -20, 10, 10, 5,
	0, 0, 0, 0, 0, 0, 0, 0
};

const std::vector<int> Evaluation::knightPositionValue =
{
	-50, -40, -30, -30, -30, -30, -40, -50,
	-40, -20, 0, 0, 0, 0, -20, -40,
	-30, 0, 10, 15, 15, 10, 0, -30,
	-30, 5, 15, 20, 20, 15, 5, -30,
	-30, 0, 15, 20, 20, 15, 0, -30,
	-30, 5, 10, 15, 15, 10, 5, -30,
	-40, -20, 0, 5, 5, 0, -20, -40,
	-50, -40, -30, -30, -30, -30, -40, -50
};


const std::vector<int> Evaluation::whiteBishopPositionValue =
{
	-20, -10, -10, -10, -10, -10, -10, -20,
	-10, 5, 0, 0, 0, 0, 5, -10,
	-10, 10, 10, 10, 10, 10, 10, -10,
	-10, 0, 10, 10, 10, 10, 0, -10,
	-10, 5, 5, 10, 10, 5, 5, -10,
	-10, 0, 5, 10, 10, 5, 0, -10,
	-10, 0, 0, 0, 0, 0, 0, -10,
	-20, -10, -10, -10, -10, -10, -10, -20
};

const std::vector<int> Evaluation::blackBishopPositionValue =
{
	-20, -10, -10, -10, -10, -10, -10, -20,
	-10, 0, 0, 0, 0, 0, 0, -10,
	-10, 0, 5, 10, 10, 5, 0, -10,
	-10, 5, 5, 10, 10, 5, 5, -10,
	-10, 0, 10, 10, 10, 10, 0, -10,
	-10, 10, 10, 10, 10, 10, 10, -10,
	-10, 5, 0, 0, 0, 0, 5, -10,
	-20, -10, -10, -10, -10, -10, -10, -20,
};

const std::vector<int> Evaluation::whiteRookPositionValue
{
	0, 0, 0, 5, 5, 0, 0, 0,
	-5, 0, 0, 0, 0, 0, 0, -5,
	-5, 0, 0, 0, 0, 0, 0, -5,
	-5, 0, 0, 0, 0, 0, 0, -5,
	-5, 0, 0, 0, 0, 0, 0, -5,
	-5, 0, 0, 0, 0, 0, 0, -5,
	5, 10, 10, 10, 10, 10, 10, 5,
	0, 0, 0, 0, 0, 0, 0, 0,
};

const std::vector<int> Evaluation::blackRookPositionValue
{
	0, 0, 0, 0, 0, 0, 0, 0,
	5, 10, 10, 10, 10, 10, 10, 5,
	-5, 0, 0, 0, 0, 0, 0, -5,
	-5, 0, 0, 0, 0, 0, 0, -5,
	-5, 0, 0, 0, 0, 0, 0, -5,
	-5, 0, 0, 0, 0, 0, 0, -5,
	-5, 0, 0, 0, 0, 0, 0, -5,
	0, 0, 0, 5, 5, 0, 0, 0
};

const std::vector<int> Evaluation::whiteQueenPositionValue =
{
	-20,-10,-10, -5, -5,-10,-10,-20,
	-10,  0,  5,  0,  0,  0,  0,-10,
	-10,  5,  5,  5,  5,  5,  0,-10,
	0,  0,  5,  5,  5,  5,  0, -5,
	-5,  0,  5,  5,  5,  5,  0, -5,
	-10,  0,  5,  5,  5,  5,  0,-10,
	-10,  0,  0,  0,  0,  0,  0,-10,
	-20,-10,-10, -5, -5,-10,-10,-20,
};

const std::vector<int> Evaluation::blackQueenPositionValue =
{
	-20,-10,-10, -5, -5,-10,-10,-20,
	-10,  0,  0,  0,  0,  0,  0,-10,
	-10,  0,  5,  5,  5,  5,  0,-10,
	-5,  0,  5,  5,  5,  5,  0, -5,
	0,  0,  5,  5,  5,  5,  0, -5,
	-10,  5,  5,  5,  5,  5,  0,-10,
	-10,  0,  5,  0,  0,  0,  0,-10,
	-20,-10,-10, -5, -5,-10,-10,-20
};

//Modifications from the article to encourage castling
const std::vector<int> Evaluation::whiteKingMiddleGamePositionValue =
{
	20, 30, 30,  0,  0, 10, 30, 20,
	20, 20,  0,  0,  0,  0, 20, 20,
	-10,-20,-20,-20,-20,-20,-20,-10,
	-20,-30,-30,-40,-40,-30,-30,-20,
	-30,-40,-40,-50,-50,-40,-40,-30,
	-30,-40,-40,-50,-50,-40,-40,-30,
	-30,-40,-40,-50,-50,-40,-40,-30,
	-30,-40,-40,-50,-50,-40,-40,-30
};

const std::vector<int> Evaluation::blackKingMiddleGamePositionValue =
{
	-30,-40,-40,-50,-50,-40,-40,-30,
	-30,-40,-40,-50,-50,-40,-40,-30,
	-30,-40,-40,-50,-50,-40,-40,-30,
	-30,-40,-40,-50,-50,-40,-40,-30,
	-20,-30,-30,-40,-40,-30,-30,-20,
	-10,-20,-20,-20,-20,-20,-20,-10,
	20, 20,  0,  0,  0,  0, 20, 20,
	20, 30, 30,  0,  0, 10, 30, 20
};

const std::vector<int> Evaluation::whiteKingEndGamePositionValue =
{
	-50,-30,-30,-30,-30,-30,-30,-50,
	-30,-30,  0,  0,  0,  0,-30,-30,
	-30,-10, 20, 30, 30, 20,-10,-30,
	-30,-10, 30, 40, 40, 30,-10,-30,
	-30,-10, 30, 40, 40, 30,-10,-30,
	-30,-10, 20, 30, 30, 20,-10,-30,
	-30,-20,-10,  0,  0,-10,-20,-30,
	-50,-40,-30,-20,-20,-30,-40,-50,
};

const std::vector<int> Evaluation::blackKingEndGamePositionValue =
{
	-50,-40,-30,-20,-20,-30,-40,-50,
	-30,-20,-10,  0,  0,-10,-20,-30,
	-30,-10, 20, 30, 30, 20,-10,-30,
	-30,-10, 30, 40, 40, 30,-10,-30,
	-30,-10, 30, 40, 40, 30,-10,-30,
	-30,-10, 20, 30, 30, 20,-10,-30,
	-30,-30,  0,  0,  0,  0,-30,-30,
	-50,-30,-30,-30,-30,-30,-30,-50
};


const std::unordered_map<Piece, int> Evaluation::piecesValue =
{
	{ Piece::blackBishop, bishopValue },{ Piece::whiteBishop, bishopValue },
	{ Piece::blackKnight, knightValue },{ Piece::whiteKnight, knightValue },
	{ Piece::blackRook, rookValue },{ Piece::whiteRook, rookValue },
	{ Piece::blackPawn, pawnValue },{ Piece::whitePawn, pawnValue },
	{ Piece::blackQueen, queenValue },{ Piece::whiteQueen, queenValue }
};

//King is not present, because it is the only one which has a matrice for 
//the beginning of the game, and one the end of the game
const std::unordered_map<Piece, std::vector<int>> Evaluation::positionsValue =
{ 
	{ Piece::blackBishop, blackBishopPositionValue }, {Piece::whiteBishop, whiteBishopPositionValue},
	{Piece::blackKnight, knightPositionValue}, {Piece::whiteKnight, knightPositionValue},
	{ Piece::blackRook, blackRookPositionValue },{ Piece::whiteRook, whiteRookPositionValue },
	{ Piece::blackPawn, blackPawnPositionValue },{ Piece::whitePawn, whitePawnPositionValue },
	{ Piece::blackQueen, blackQueenPositionValue },{ Piece::whiteQueen, whiteQueenPositionValue }
};

const int Evaluation::evaluate(BoardRepresentation& boardRepresentation)
{
	int colorFactor = (boardRepresentation.isWhiteTurn) ? 1 : -1;

	int score = 0, kingIndex;
	std::vector<Piece> pieces;
	for (int caseIndex = 0; caseIndex < 64; ++caseIndex)
	{
		Piece currentCase = boardRepresentation.board[caseIndex];
		if (isPieceNone(currentCase)) continue;
		pieces.push_back(currentCase);

		//Special Process for king because it has a matrice for the
		//Middle game an one for the end game
		if (isPieceKing)
		{
			//Calculate later once we have the list of pieces
			//So its faster to calculate if its a an endgame or not
			kingIndex = caseIndex;
		}
		else
		{
			score += piecesValue.find(currentCase)->second;
			score += positionsValue.find(currentCase)->second[caseIndex];
		}
	}

	if (isKingCheckmate(boardRepresentation, kingIndex))
	{
		return colorFactor * infinity;
	}

	if (isKingStealMate(boardRepresentation, kingIndex) ||
		boardRepresentation.reversibleMovesInRow >= 50)
	{
		return 0;
	}

	//Add king score
	score += kingValue;
	if (isEndGame(boardRepresentation, pieces))
	{
		if (boardRepresentation.isWhiteTurn)
			score += whiteKingEndGamePositionValue[kingIndex];
		else
			score += blackKingEndGamePositionValue[kingIndex];
	}
	else
	{
		if (boardRepresentation.isWhiteTurn)
			score += whiteKingMiddleGamePositionValue[kingIndex];
		else
			score += blackKingMiddleGamePositionValue[kingIndex];
	}

	return score * colorFactor;
}

//Can do better: as for now do like the article recommend:
//Both sides have no queens or
//Every side which has a queen has additionally no other pieces or one minorpiece maximum.
bool Evaluation::isEndGame(BoardRepresentation& boardRepresentation, std::vector<Piece>& pieces)
{
	std::vector<Piece> whiteMinorPieces;
	std::vector<Piece> blackMinorPieces;
	bool whitehasQueen = false, blackHasQueen = false;

	auto isMinorPiece = [](Piece p) { return (isPieceBishop(p) || isPieceKnight(p) || isPieceRook(p)); };
	for (auto& piece : pieces)
	{
		if (isPieceWhite)
		{
			if (isMinorPiece(piece)) whiteMinorPieces.push_back(piece);
			else if (isPieceQueen(piece)) whitehasQueen = true;

		}
		else
		{
			if (isMinorPiece(piece)) blackMinorPieces.push_back(piece);
			else if (isPieceQueen(piece)) blackHasQueen = true;
		}
	}

	return (   
		        (!whitehasQueen && ! blackHasQueen)
		    || ((!whitehasQueen && blackHasQueen) && (blackMinorPieces.size() <= 1))
		    || ((whitehasQueen && !blackHasQueen) && (whiteMinorPieces.size() <= 1))
			|| ((blackMinorPieces.size() <= 1) && (whiteMinorPieces.size() <= 1) )
			);
}


