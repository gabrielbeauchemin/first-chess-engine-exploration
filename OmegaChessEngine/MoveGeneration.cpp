#include "MoveGeneration.hpp"
#include "NotImplementedException.h"
#include <algorithm>

namespace MoveGeneration
{
	int mailbox[120] = {
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
		-1,  0,  1,  2,  3,  4,  5,  6,  7, -1,
		-1,  8,  9, 10, 11, 12, 13, 14, 15, -1,
		-1, 16, 17, 18, 19, 20, 21, 22, 23, -1,
		-1, 24, 25, 26, 27, 28, 29, 30, 31, -1,
		-1, 32, 33, 34, 35, 36, 37, 38, 39, -1,
		-1, 40, 41, 42, 43, 44, 45, 46, 47, -1,
		-1, 48, 49, 50, 51, 52, 53, 54, 55, -1,
		-1, 56, 57, 58, 59, 60, 61, 62, 63, -1,
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1
	};

	static const int mailbox64[64] = {
		21, 22, 23, 24, 25, 26, 27, 28,
		31, 32, 33, 34, 35, 36, 37, 38,
		41, 42, 43, 44, 45, 46, 47, 48,
		51, 52, 53, 54, 55, 56, 57, 58,
		61, 62, 63, 64, 65, 66, 67, 68,
		71, 72, 73, 74, 75, 76, 77, 78,
		81, 82, 83, 84, 85, 86, 87, 88,
		91, 92, 93, 94, 95, 96, 97, 98
	};

	static const std::vector<int> knightOffset = { -21, -19, -12, -8, 8, 12, 19, 21 };
	static const std::vector<int> bishopOffset = { -11,  -9,  9, 11};
	static const std::vector<int> blackPawnOffset = { -9,  -11};
	static const std::vector<int> whitePawnOffset = {9, 11 };
	static const std::vector<int> rookOffset = { -10,  -1,  1, 10};
	static const std::vector<int> queenOffset = { -11, -10, -9, -1, 1,  9, 10, 11 };
	static const std::vector<int> kingOffset = { -11, -10, -9, -1, 1,  9, 10, 11 };

	std::vector<Move> generateMoves(BoardRepresentation& boardRepresentation)
	{
		//Find king index
		int kingIndex;
		for (int caseIndex = 0; caseIndex < 64; ++caseIndex)
		{
			if (isPieceKing(boardRepresentation.board[caseIndex]) &&
				isPieceWhite(boardRepresentation.board[caseIndex]) == boardRepresentation.isWhiteTurn)
			{
				kingIndex = caseIndex;
				break;
			}
		}

		//If the king is cheked
		bool isKingInCheck = false;
		if (isPieceAttacked(boardRepresentation, kingIndex))
		{
			isKingInCheck = true;
		}

		std::vector<Move> moves;
		for (int caseIndex = 0; caseIndex < 64; ++caseIndex)
		{
			Piece currentPiece = boardRepresentation.board[caseIndex];

			//Move only the pieces of the color which is turn to play
			if (isPieceWhite(currentPiece) != boardRepresentation.isWhiteTurn)
				continue;

			if (isPieceNone(currentPiece))
			{
				continue;
			}
			else
			{
				//Do not move pieces that are in absolute pin
				if (isPieceInAbsolutePin(boardRepresentation, caseIndex, kingIndex))
				{
					continue;
				}
				else if (isPiecePawn(currentPiece))
				{
					if (isPawnMovePromotion(boardRepresentation, caseIndex))
					{
						std::vector<Piece> possiblePromotion = (boardRepresentation.isWhiteTurn)
							                                   ? getWhitePieces() : getBlackPieces();
						for (auto& promotionType : possiblePromotion)
						{
							std::vector<Move> promotionMove = generatePawnMoves(boardRepresentation, caseIndex, promotionType);
							if (isKingInCheck) filterMovesUncheckingKing(boardRepresentation, promotionMove, kingIndex);
							moves.insert(moves.end(), promotionMove.begin(), promotionMove.end());
						}
					}
					else
					{
						auto newMoves = generatePawnMoves(boardRepresentation, caseIndex);
						if (isKingInCheck) filterMovesUncheckingKing(boardRepresentation, newMoves, kingIndex);
						moves.insert(moves.end(), newMoves.begin(), newMoves.end());
					}
				}
				else if (isPieceBishop(currentPiece))
				{
					auto newMoves = generateBishopMoves(boardRepresentation, caseIndex);
					if (isKingInCheck) filterMovesUncheckingKing(boardRepresentation, newMoves, kingIndex);
					moves.insert(moves.end(), newMoves.begin(), newMoves.end());
				}
				else if (isPieceKnight(currentPiece))
				{
					auto newMoves = generateKnightMoves(boardRepresentation, caseIndex);
					if (isKingInCheck) filterMovesUncheckingKing(boardRepresentation, newMoves, kingIndex);
					moves.insert(moves.end(), newMoves.begin(), newMoves.end());
				}
				else if (isPieceRook(currentPiece))
				{
					auto newMoves = generateRookMoves(boardRepresentation, caseIndex);
					if (isKingInCheck) filterMovesUncheckingKing(boardRepresentation, newMoves, kingIndex);
					moves.insert(moves.end(), newMoves.begin(), newMoves.end());
				}
				else if (isPieceKing(currentPiece))
				{
					auto newMoves = generateKingMoves(boardRepresentation, caseIndex);
					moves.insert(moves.end(), newMoves.begin(), newMoves.end());
					if (!isKingInCheck)
					{
						newMoves = generateCastlingMoves(boardRepresentation, caseIndex);
						moves.insert(moves.end(), newMoves.begin(), newMoves.end());
					}
					
				}
				else if (isPieceQueen(currentPiece))
				{
					auto newMoves = generateQueenMoves(boardRepresentation, caseIndex);
					if (isKingInCheck) filterMovesUncheckingKing(boardRepresentation, newMoves, kingIndex);
					moves.insert(moves.end(), newMoves.begin(), newMoves.end());
				}
			}
			
		}

		return moves;
	}

	std::vector<Move> generateRookMoves(const BoardRepresentation& boardRepresentation, int rookCase)
	{
		std::vector<Move> moves;

		//Each iteration, move the piece of one case in the direction it can move to generate the moves
		for (const int& currentOffset : rookOffset)
		{
			int currentCaseIndex = rookCase;
			//Generate all possible moves in the direction the current offset
			while (true)
			{
				currentCaseIndex = mailbox[mailbox64[currentCaseIndex] + currentOffset];
				if (currentCaseIndex == -1) break; //Outside of the board

				//The case is occupied
				if (!isPieceNone(boardRepresentation.board[currentCaseIndex]))
				{
					if (isPieceWhite(boardRepresentation.board[currentCaseIndex]) != boardRepresentation.isWhiteTurn)
						moves.push_back(Move{ rookCase,currentCaseIndex }); //Capture

					//its a capture or the piece got blocked by a piece of its camp, 
					//in both case pass to the next direction
					break;
				}
				else //The case is empty
				{
					
					moves.push_back(Move{ rookCase,currentCaseIndex });
				}
				
			}
		}

		return moves;
	}

	std::vector<Move> generateKnightMoves(const BoardRepresentation& boardRepresentation, int knightCase)
	{
		std::vector<Move> moves;

		//Each iteration, move the piece of one case in the direction it can move to generate the moves
		for (const int& currentOffset : knightOffset)
		{
			int currentCaseIndex = knightCase;
			//Generate one move in the direction the current offset
			currentCaseIndex = mailbox[mailbox64[currentCaseIndex] + currentOffset];
			if (currentCaseIndex == -1) continue; //Outside of the board

			//The case is occupied
			if (!isPieceNone(boardRepresentation.board[currentCaseIndex]))
			{
				if (isPieceWhite(boardRepresentation.board[currentCaseIndex]) != boardRepresentation.isWhiteTurn)
					moves.push_back(Move{ knightCase,currentCaseIndex }); //Capture

				//its a capture or the piece got blocked by a piece of its camp, 
				//in both case pass to the next direction																
				continue;
			}
			else //The case is empty
			{
				moves.push_back(Move{ knightCase,currentCaseIndex });

			}
		}

		return moves;
	}

	std::vector<Move> generateBishopMoves(const BoardRepresentation& boardRepresentation, int bishopCase)
	{
		std::vector<Move> moves;

		//Each iteration, move the piece of one case in the direction it can move to generate the moves
		for (const int& currentOffset : bishopOffset)
		{
			int currentCaseIndex = bishopCase;
			//Generate all possible moves in the direction the current offset
			while (true)
			{
				currentCaseIndex = mailbox[mailbox64[currentCaseIndex] + currentOffset];
				if (currentCaseIndex == -1) break; //Outside of the board

				//The case is occupied
				if (!isPieceNone(boardRepresentation.board[currentCaseIndex]))
				{
					if (isPieceWhite(boardRepresentation.board[currentCaseIndex]) != boardRepresentation.isWhiteTurn)
						moves.push_back(Move{ bishopCase,currentCaseIndex }); //Capture
					//its a capture or the piece got blocked by a piece of its camp, 
					//in both case pass to the next direction
					break;
				}
				else //The case is empty
				{
					moves.push_back(Move{ bishopCase,currentCaseIndex });
				}
			}
		}

		return moves;
	}

	//Do not include the castling move to be more modular
	std::vector<Move> generateKingMoves(const BoardRepresentation& boardRepresentation, int kingCase)
	{
		std::vector<Move> moves;

		//Each iteration, move the piece of one case in the direction it can move to generate the moves
		for (const int& currentOffset : kingOffset)
		{
			int currentCaseIndex = kingCase;
			//Generate one move in the direction the current offset
			currentCaseIndex = mailbox[mailbox64[currentCaseIndex] + currentOffset];
			if (currentCaseIndex == -1) continue; //Outside of the board

			//The case is occupied
			if (!isPieceNone(boardRepresentation.board[currentCaseIndex]))
			{
				//Capture opposite piece
				if (isPieceWhite(boardRepresentation.board[currentCaseIndex]) != boardRepresentation.isWhiteTurn)
				{
					//The opposite piece should not be protected to be captured by the king
					if(!isPieceAttacked(boardRepresentation, currentCaseIndex))
					   moves.push_back(Move{ kingCase,currentCaseIndex }); 
				}

				//its a capture or the piece got blocked by a piece of its camp, 
				//in both case pass to the next direction																
				continue;
			}
			else //The case is empty
			{
				//The king should not be in check at this new case
				if (!isPieceAttacked(boardRepresentation, currentCaseIndex))
				   moves.push_back(Move{ kingCase,currentCaseIndex });
			}
			
		}

		return moves;
	}

	//Do not check if king is in check, it is checked in generateMoves
	std::vector<Move> generateCastlingMoves(const BoardRepresentation& boardRepresentation, int kingCase)
	{
		std::vector<Move> possibleCastlings;
		std::vector<Move> castlingMoves = (boardRepresentation.isWhiteTurn)
			                       ? std::vector<Move>{ Move{ 4,6 }, Move{ 4,2 }}
		                           : std::vector<Move>{ Move{ 60,62 }, Move{ 60,58 }};

		//If king has already castle or move, it doesnt have the right to castle					    
		if ((boardRepresentation.isWhiteTurn && !boardRepresentation.canWhiteCastle) ||
			(!boardRepresentation.isWhiteTurn && !boardRepresentation.canBlackCastle))
		{
			return std::vector<Move>{};
		}

		//For both possible castling
		for (auto castle : castlingMoves)
		{
			//If there is place between the king and the rook
			bool isSpaceToCastle = true;
			int kingDirection = (castle.from - castle.to < 0) ? 1 : -1;
			for (int indexCase = kingCase + kingDirection;
				indexCase != castle.to; indexCase += kingDirection)
			{
				if (!isPieceNone(boardRepresentation.board[indexCase]) ||
					isPieceAttacked(boardRepresentation, indexCase))
				{
					isSpaceToCastle = false;
					break; 
				}
			}
			if (isSpaceToCastle) possibleCastlings.push_back(castle);
		}
		
		return possibleCastlings;
	}

	std::vector<Move> generateQueenMoves(const BoardRepresentation& boardRepresentation, int queenCase)
	{
		std::vector<Move> moves;

		//Each iteration, move the piece of one case in the direction it can move to generate the moves
		for (const int& currentOffset : queenOffset)
		{
			int currentCaseIndex = queenCase;
			//Generate all possible moves in the direction the current offset
			while (true)
			{
				currentCaseIndex = mailbox[mailbox64[currentCaseIndex] + currentOffset];
				if (currentCaseIndex == -1) break; //Outside of the board

			   //The case is occupied
				if (!isPieceNone(boardRepresentation.board[currentCaseIndex]))
				{
					if (isPieceWhite(boardRepresentation.board[currentCaseIndex]) != boardRepresentation.isWhiteTurn)
						moves.push_back(Move{ queenCase,currentCaseIndex }); //Capture
																			 //its a capture or the piece got blocked by a piece of its camp, 
																		    //in both case pass to the next direction
					break;
				}
				else //The case is empty
				{
					moves.push_back(Move{ queenCase,currentCaseIndex });
				}
				
			}
		}

		return moves;
	}

	//Does not include the move pawn for a promotion to be more modular, see function generatePawnPromotion
	std::vector<Move> generatePawnMoves(const BoardRepresentation& boardRepresentation, int pawnCase)
	{
		std::vector<Move> moves;
		const int row = 8;

		//Moving forward of one case if the case is empty
		int step = (boardRepresentation.isWhiteTurn) ? row : -row;
		bool isNextCaseEmpty = isPieceNone(boardRepresentation.board[pawnCase + step]);
		if(isNextCaseEmpty)
		   moves.push_back(Move{ pawnCase,pawnCase + step });

		//Moving forward of two cases is only possible if the pawn is on its initial case
		bool isOnInitialRow = (boardRepresentation.isWhiteTurn) ? pawnCase / row == 1
			                                      : pawnCase / row == 6;
		if (isOnInitialRow)
		{
			if(isNextCaseEmpty && isPieceNone(boardRepresentation.board[pawnCase + step + step]))
			moves.push_back(Move{ pawnCase,pawnCase + step + step });
		}

		//Capture a piece
		std::vector<int> pawnOffset = (boardRepresentation.isWhiteTurn) ? whitePawnOffset : blackPawnOffset;
		for (const int& currentOffset : pawnOffset)
		{
			int currentCaseIndex = pawnCase;
			//Generate one move in the direction the current offset
			currentCaseIndex = mailbox[mailbox64[currentCaseIndex] + currentOffset];
			if (currentCaseIndex == -1) continue; //Outside of the board

			//The case is occupied
			if (!isPieceNone(boardRepresentation.board[currentCaseIndex]))
			{
				//Capture opposite piece
				if (isPieceWhite(boardRepresentation.board[currentCaseIndex]) != boardRepresentation.isWhiteTurn)
				{
					moves.push_back(Move{ pawnCase,currentCaseIndex });
				}
				//Else the pawn is blocked by a piece of its camp, nothing to do
			}
			//Else the case is empty, pawn cannot eat it
		}

		//Coup en passant if it is possible
		if (boardRepresentation.isEnPensantPossible.first)
		{
			//If the pawn that can be captured by en passant is at the side of the pawn
			if (pawnCase + 1 == boardRepresentation.isEnPensantPossible.second) //Right side
			{
				int posOppositePawn = pawnCase + 1;
				int posAfterEnPassant = posOppositePawn + step;
				moves.push_back(Move{ pawnCase, posAfterEnPassant });
				
			}
			if (pawnCase - 1 == boardRepresentation.isEnPensantPossible.second) //Left side
			{
				int posOppositePawn = pawnCase - 1;
				int posAfterEnPassant = posOppositePawn + step;
				moves.push_back(Move{ pawnCase, posAfterEnPassant });
			}
		}


		return moves;
	}

	std::vector<Move> generatePawnMoves(const BoardRepresentation& board, int pawnCase, Piece promotion)
	{
		const int row = 8;
		int step = (board.isWhiteTurn) ? row : -row;
		return std::vector<Move>{ Move{ pawnCase,pawnCase + step, promotion }};
	}

	bool isPieceAttacked(const BoardRepresentation& boardRepresentation, int pieceCase)
	{
		//Each iteration, move the piece in diagonal
		//If he meets bishop or queen or pawn from the opposite camp, hes in check
		for (const int& currentOffset : bishopOffset /*Diagonal Moves*/)
		{
			int currentCaseIndex = pieceCase;

			//Generate all possible moves in the direction the current diagonal
			bool firstOffset = true;
			while (true)
			{
				currentCaseIndex = mailbox[mailbox64[currentCaseIndex] + currentOffset];
				if (currentCaseIndex == -1) break; //Outside of the board

				//piece meets a piece
				auto currentCase = boardRepresentation.board[currentCaseIndex];
				if (!isPieceNone(currentCase))
				{
					//piece meets pawn after only one offset
					if (isPieceWhite(currentCase) != boardRepresentation.isWhiteTurn
						&& isPiecePawn(currentCase) && firstOffset)
					{
						return true;
					}
					//piece meets a bishop or queen from opposite camp, hes in check
					else if (isPieceWhite(currentCase) != boardRepresentation.isWhiteTurn &&
						     (isPieceQueen(currentCase) || isPieceBishop(currentCase)) )
					{
						return true;
					}
					//He meets a piece of its own camp or a piece from the opposite camp
					//That cant put him in check. Look into another direction
					else
					{
						break;
					}
				}

				//The case is empty, continue searching in this direction
				firstOffset = false;
			}
		}

		//Each iteration, move the piece in lines
		//If he meets rook or queen from the opposite camp, hes in check
		for (const int& currentOffset : rookOffset /*Moves in line*/)
		{
			int currentCaseIndex = pieceCase;
			//Generate all possible moves in the direction the current offset
			while (true)
			{
				currentCaseIndex = mailbox[mailbox64[currentCaseIndex] + currentOffset];
				if (currentCaseIndex == -1) break; //Outside of the board

				//piece meets a piece
				auto currentCase = boardRepresentation.board[currentCaseIndex];
				if (!isPieceNone(currentCase))
				{
					auto t = isPieceWhite(currentCase);
					//piece meets a rook or queen from opposite camp, hes in check
					if (isPieceWhite(currentCase) != boardRepresentation.isWhiteTurn
						&& (isPieceQueen(currentCase) || isPieceRook(currentCase)))
					{
						return true;
					}
					//He meets a piece of its own camp or a piece from the opposite camp
					//That cant put him in check. Look into another direction
					else
					{
						break;
					}
				}

				//The case is empty, nothing to do, continue searching in this direction
			}
		}

		//Move the piece like a knight: 
		//If he meets a knight from the opposite camp, hes in check
		for (const int& currentOffset : knightOffset)
		{
			int currentCaseIndex = pieceCase;
			currentCaseIndex = mailbox[mailbox64[currentCaseIndex] + currentOffset];

			if (currentCaseIndex == -1) continue; //Outside of the board

			auto currentCase = boardRepresentation.board[currentCaseIndex];
			//piece meets a rook or queen from opposite camp, hes in check
			if (isPieceWhite(currentCase) != boardRepresentation.isWhiteTurn
				&& isPieceKnight(currentCase))
			{
				return true;
			}
		}

		//piece didnt find any pieces of the opposite camp that are attacking him
		return false;

	}

	bool isPieceInAbsolutePin(BoardRepresentation& boardRepresentation, int pieceCase, int kingCase)
	{
		//If the piece is in absolute pin, that means that if
		//It is gone, the king is cheked
		Piece tmp = boardRepresentation.board[pieceCase];
		boardRepresentation.board[pieceCase] = Piece::none;
		bool isPieceBasolutePin = isPieceAttacked(boardRepresentation, kingCase);
		boardRepresentation.board[pieceCase] = tmp;
		return isPieceBasolutePin;
	}

	bool isPawnMovePromotion(const BoardRepresentation& boardRepresentation, int pawnCaseBeforeMove)
	{
		if (boardRepresentation.isWhiteTurn)
		{
			return (pawnCaseBeforeMove / 8) == 6;
		}
		else
		{
			return (pawnCaseBeforeMove / 8) == 1;
		}
	}

	void filterMovesUncheckingKing(BoardRepresentation& boardRepresentation, std::vector<Move>& movesToFilter, int kingCase)
	{
		std::remove_if(movesToFilter.begin(), movesToFilter.end(), 
			[&](const Move & move)
		{
			boardRepresentation.move(move);
			bool isKingInCheck = isPieceAttacked(boardRepresentation, kingCase);
			boardRepresentation.unmakeMove(move);
			return isKingInCheck;
		});
		
	}

	bool isKingCheckmate(BoardRepresentation & boardRepresentation, int kingCase)
	{
		return isPieceAttacked(boardRepresentation, kingCase) &&
			   generateMoves(boardRepresentation).size() == 0;
	}

	bool isKingStealMate(BoardRepresentation & boardRepresentation, int kingCase)
	{
		return generateMoves(boardRepresentation).size() == 0 &&
			  !isPieceAttacked(boardRepresentation, kingCase);
	}
}

