#include "MoveGeneration.hpp"
#include "NotImplementedException.h"


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
	static const std::vector<int> rookOffset = { -10,  -1,  1, 10};
	static const std::vector<int> queenOffset = { -11, -10, -9, -1, 1,  9, 10, 11 };
	static const std::vector<int> kingOffset = { -11, -10, -9, -1, 1,  9, 10, 11 };

	bool isStateLegal(const BoardRepresentation state, Notation lastMove)
	{
		
			//Check if the cases between the Castle and the king are empty
			/*for (int caseInTheWay = move.from + 1; caseInTheWay < move.to; ++caseInTheWay)
				if (board[caseInTheWay].type != PieceType::none)
					return MoveResult{ false };*/

			//Valid if the king can Castle
			//bool isKingSideCastle = move.from < move.to; //If false, we know its a queen sine castle
			//int translationKingRook = (isKingSideCastle) ? 2 : -3;
			//bool isRookPresent = board[move.from + translationKingRook].type == PieceType::rook;
			//bool canKingCastle = (this->isWhiteTurn) ? canWhiteCastle : canBlackCastle;
			//if (!isRookPresent || !canKingCastle)
			//{
			//	return MoveResult{ false }; //King is not allowed to castle
			//}

		return true;
	}

	std::vector<Notation> generateMoves(const BoardRepresentation& boardRepresentation)
	{
		//Find king index
		int kingIndex;
		for (int caseIndex = 0; caseIndex < 64; ++caseIndex)
		{
			if (boardRepresentation.board[caseIndex].type == PieceType::king &&
				boardRepresentation.board[caseIndex].isWhite == boardRepresentation.isWhiteTurn)
			{
				kingIndex = caseIndex;
				break;
			}
		}

		//If the king is cheked
		if (isPieceAttacked(boardRepresentation, kingIndex))
		{
			//That do not include the Castling
			return generateKingMoves(boardRepresentation, kingIndex);
		}

		std::vector<Notation> moves;
		for (int caseIndex = 0; caseIndex < 64; ++caseIndex)
		{
			Piece currentPiece = boardRepresentation.board[caseIndex];
			PieceType typeOfCase = currentPiece.type;

			//Move only the pieces of the color which is turn to play
			if (currentPiece.isWhite != boardRepresentation.isWhiteTurn)
				break;

			if (typeOfCase != PieceType::none)
			{
				break;
			}
			else
			{
				//Do not move pieces that are in absolute pin
				if (isPieceInAbsolutePin(boardRepresentation, caseIndex, kingIndex))
				{
					break;
				}
				else if (typeOfCase == PieceType::pawn)
				{
					if (isPawnMovePromotion(boardRepresentation, caseIndex))
					{
						std::vector<PieceType> possiblePromotion{ PieceType::bishop,PieceType::knight,PieceType::queen, PieceType::rook };
						for (auto& promotionType : possiblePromotion)
						{
							Notation promotionMove = generatePawnMoves(boardRepresentation, caseIndex, promotionType);
							moves.push_back(promotionMove);
						}
					}
					else
					{
						auto newMoves = generatePawnMoves(boardRepresentation, caseIndex);
						moves.insert(moves.end(), newMoves.begin(), newMoves.end());
					}
				}
				else if (typeOfCase == PieceType::bishop)
				{
					auto newMoves = generateBishopMoves(boardRepresentation, caseIndex);
					moves.insert(moves.end(), newMoves.begin(), newMoves.end());
				}
				else if (typeOfCase == PieceType::knight)
				{
					auto newMoves = generateKingMoves(boardRepresentation, caseIndex);
					moves.insert(moves.end(), newMoves.begin(), newMoves.end());
				}
				else if (typeOfCase == PieceType::rook)
				{
					auto newMoves = generateRookMoves(boardRepresentation, caseIndex);
					moves.insert(moves.end(), newMoves.begin(), newMoves.end());
				}
				else if (typeOfCase == PieceType::king)
				{
					auto newMoves = generateKingMoves(boardRepresentation, caseIndex);
					moves.insert(moves.end(), newMoves.begin(), newMoves.end());
					newMoves = generateCastlingMoves(boardRepresentation, caseIndex);
					moves.insert(moves.end(), newMoves.begin(), newMoves.end());
				}
				else if (typeOfCase == PieceType::queen)
				{
					auto newMoves = generateQueenMoves(boardRepresentation, caseIndex);
					moves.insert(moves.end(), newMoves.begin(), newMoves.end());
				}
			}
			
		}

		return moves;
	}

	std::vector<Notation> generateRookMoves(const BoardRepresentation& boardRepresentation, int rookCase)
	{
		std::vector<Notation> moves;

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
				if (boardRepresentation.board[currentCaseIndex].type != PieceType::none)
				{
					if (boardRepresentation.board[currentCaseIndex].isWhite != boardRepresentation.isWhiteTurn)
						moves.push_back(Notation{ rookCase,currentCaseIndex }); //Capture

					//its a capture or the piece got blocked by a piece of its camp, 
					//in both case pass to the next direction
					break;
				}
				else //The case is empty
				{
					
					moves.push_back(Notation{ rookCase,currentCaseIndex });
				}
				
			}
		}

		return moves;
	}

	std::vector<Notation> generateKnightMoves(const BoardRepresentation& boardRepresentation, int knightCase)
	{
		std::vector<Notation> moves;

		//Each iteration, move the piece of one case in the direction it can move to generate the moves
		for (const int& currentOffset : knightOffset)
		{
			int currentCaseIndex = knightCase;
			//Generate one move in the direction the current offset
			currentCaseIndex = mailbox[mailbox64[currentCaseIndex] + currentOffset];
			if (currentCaseIndex == -1) continue; //Outside of the board

			//The case is occupied
			if (boardRepresentation.board[currentCaseIndex].type != PieceType::none)
			{
				if (boardRepresentation.board[currentCaseIndex].isWhite != boardRepresentation.isWhiteTurn)
					moves.push_back(Notation{ knightCase,currentCaseIndex }); //Capture

				//its a capture or the piece got blocked by a piece of its camp, 
				//in both case pass to the next direction																
				continue;
			}
			else //The case is empty
			{
				moves.push_back(Notation{ knightCase,currentCaseIndex });

			}
		}

		return moves;
	}

	std::vector<Notation> generateBishopMoves(const BoardRepresentation& boardRepresentation, int bishopCase)
	{
		std::vector<Notation> moves;

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
				if (boardRepresentation.board[currentCaseIndex].type != PieceType::none)
				{
					if (boardRepresentation.board[currentCaseIndex].isWhite != boardRepresentation.isWhiteTurn)
						moves.push_back(Notation{ bishopCase,currentCaseIndex }); //Capture
					//its a capture or the piece got blocked by a piece of its camp, 
					//in both case pass to the next direction
					break;
				}
				else //The case is empty
				{
					moves.push_back(Notation{ bishopCase,currentCaseIndex });
				}
			}
		}

		return moves;
	}

	//Do not include the castling move to be more modular
	std::vector<Notation> generateKingMoves(const BoardRepresentation& boardRepresentation, int kingCase)
	{
		std::vector<Notation> moves;

		//Each iteration, move the piece of one case in the direction it can move to generate the moves
		for (const int& currentOffset : kingOffset)
		{
			int currentCaseIndex = kingCase;
			//Generate one move in the direction the current offset
			currentCaseIndex = mailbox[mailbox64[currentCaseIndex] + currentOffset];
			if (currentCaseIndex == -1) continue; //Outside of the board

			//The case is occupied
			if (boardRepresentation.board[currentCaseIndex].type != PieceType::none)
			{
				//Capture opposite piece
				if (boardRepresentation.board[currentCaseIndex].isWhite != boardRepresentation.isWhiteTurn)
				{
					//The opposite piece should not be protected to be captured by the king
					if(!isPieceAttacked(boardRepresentation, currentCaseIndex))
					   moves.push_back(Notation{ kingCase,currentCaseIndex }); 
				}

				//its a capture or the piece got blocked by a piece of its camp, 
				//in both case pass to the next direction																
				continue;
			}
			else //The case is empty
			{
				moves.push_back(Notation{ kingCase,currentCaseIndex });
			}
			
		}

		return moves;
	}

	//Do not check if king is in check, it is checked in generateMoves
	std::vector<Notation> generateCastlingMoves(const BoardRepresentation& boardRepresentation, int kingCase)
	{
		std::vector<Notation> possibleCastlings;
		std::vector<Notation> castlingMoves = (boardRepresentation.isWhiteTurn)
			                       ? std::vector<Notation>{ Notation{ 4,6 }, Notation{ 4,2 }}
		                           : std::vector<Notation>{ Notation{ 60,62 }, Notation{ 60,58 }};

		//If king has already castle or move, it doesnt have the right to castle					    
		if ((boardRepresentation.isWhiteTurn && !boardRepresentation.canWhiteCastle) ||
			(!boardRepresentation.isWhiteTurn && !boardRepresentation.canBlackCastle))
		{
			return std::vector<Notation>{};
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
				if (boardRepresentation.board[indexCase].type != PieceType::none ||
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

	std::vector<Notation> generateQueenMoves(const BoardRepresentation& boardRepresentation, int queenCase)
	{
		std::vector<Notation> moves;

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
				if (boardRepresentation.board[currentCaseIndex].type != PieceType::none)
				{
					if (boardRepresentation.board[currentCaseIndex].isWhite != boardRepresentation.isWhiteTurn)
						moves.push_back(Notation{ queenCase,currentCaseIndex }); //Capture
																				  //its a capture or the piece got blocked by a piece of its camp, 
																				  //in both case pass to the next direction
					break;
				}
				else //The case is empty
				{
					moves.push_back(Notation{ queenCase,currentCaseIndex });
				}
				
			}
		}

		return moves;
	}

	//Does not include the move pawn for a promotion to be more modular, see function generatePawnPromotion
	std::vector<Notation> generatePawnMoves(const BoardRepresentation& boardRepresentation, int pawnCase)
	{
		std::vector<Notation> moves;
		const int row = 8;

		//Moving forward of one case if the case is empty
		int step = (boardRepresentation.isWhiteTurn) ? row : -row;
		bool isNextCaseEmpty = boardRepresentation.board[pawnCase + step].type == PieceType::none;
		if(isNextCaseEmpty)
		   moves.push_back(Notation{ pawnCase,pawnCase + step });

		//Moving forward of two cases is only possible if the pawn is on its initial case
		bool isOnInitialRow = (boardRepresentation.isWhiteTurn) ? pawnCase / row == 1
			                                      : pawnCase / row == 6;
		if (isOnInitialRow)
		{
			if(isNextCaseEmpty && boardRepresentation.board[pawnCase + step + step].type == PieceType::none)
			moves.push_back(Notation{ pawnCase,pawnCase + step + step });
		}

		//Coup en passant if it is possible
		if (boardRepresentation.isEnPensantPossible.first)
		{
			//If the pawn that can be captured by en passant is at the side of the pawn
			if (pawnCase + 1 == boardRepresentation.isEnPensantPossible.second) //Right side
			{
				int posOppositePawn = pawnCase + 1;
				int posAfterEnPassant = posOppositePawn + step;
				moves.push_back(Notation{ pawnCase, posAfterEnPassant });
				
			}
			if (pawnCase - 1 == boardRepresentation.isEnPensantPossible.second) //Left side
			{
				int posOppositePawn = pawnCase - 1;
				int posAfterEnPassant = posOppositePawn + step;
				moves.push_back(Notation{ pawnCase, posAfterEnPassant });
			}
		}

		return moves;
	}

	Notation generatePawnMoves(const BoardRepresentation& board, int pawnCase, PieceType promotion)
	{
		const int row = 8;
		int step = (board.isWhiteTurn) ? row : -row;
		return Notation{ pawnCase,pawnCase + step, promotion };
	}

	bool isPieceAttacked(const BoardRepresentation& boardRepresentation, int kingCase)
	{
		//Each iteration, move the king in diagonal
		//If he meets bishop or queen or pawn from the opposite camp, hes in check
		for (const int& currentOffset : bishopOffset /*Diagonal Moves*/)
		{
			int currentCaseIndex = kingCase;
			//Generate all possible moves in the direction the current diagon
			while (true)
			{
				currentCaseIndex = mailbox[mailbox64[currentCaseIndex] + currentOffset];
				if (currentCaseIndex == -1) break; //Outside of the board

				//King meets a piece
				auto currentCase = boardRepresentation.board[currentCaseIndex];
				if (currentCase.type != PieceType::none)
				{
					//King meets a bishop or queen or pawn from opposite camp, hes in check
					if (currentCase.isWhite != boardRepresentation.isWhiteTurn
						&& (currentCase.type == PieceType::queen ||
							currentCase.type == PieceType::bishop ||
							currentCase.type == PieceType::pawn))
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

		//Each iteration, move the king in lines
		//If he meets rook or queen from the opposite camp, hes in check
		for (const int& currentOffset : rookOffset /*Moves in line*/)
		{
			int currentCaseIndex = kingCase;
			//Generate all possible moves in the direction the current offset
			while (true)
			{
				currentCaseIndex = mailbox[mailbox64[currentCaseIndex] + currentOffset];
				if (currentCaseIndex == -1) break; //Outside of the board

				//King meets a piece
				auto currentCase = boardRepresentation.board[currentCaseIndex];
				if (currentCase.type != PieceType::none)
				{
					//King meets a rook or queen from opposite camp, hes in check
					if (currentCase.isWhite != boardRepresentation.isWhiteTurn
						&& (currentCase.type == PieceType::queen || currentCase.type == PieceType::rook))
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

		//Move the king like a knight: 
		//If he meets a knight from the opposite camp, hes in check
		for (const int& currentOffset : knightOffset)
		{
			int currentCaseIndex = kingCase;
			currentCaseIndex = mailbox[mailbox64[currentCaseIndex] + currentOffset];

			if (currentCaseIndex == -1) continue; //Outside of the board

			auto currentCase = boardRepresentation.board[currentCaseIndex];
			//King meets a rook or queen from opposite camp, hes in check
			if (currentCase.isWhite != boardRepresentation.isWhiteTurn
				&& currentCase.type == PieceType::knight)
			{
				return true;
			}
		}

		//King didnt find any pieces of the opposite camp that are attacking him
		return false;

	}

	bool isPieceInAbsolutePin(BoardRepresentation boardRepresentation, int pieceCase, int kingCase)
	{
		//If the piece is in absolute pin, that means that if
		//It is gone, the king is cheked
		boardRepresentation.board[pieceCase].type = PieceType::none;
		return isPieceAttacked(boardRepresentation, kingCase);
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
}

