
//The protocol is based and inspired by the one of this open source engine:
//https://github.com/fluxroot/pulse/blob/master/src/main/cpp/pulse.h

#ifndef UCIProtocol_H
#define UCIProtocol_H

#include "Evaluation.h"
#include "MoveGeneration.hpp"
#include "Search.h"

class UCIProtocol
{
public:
	void run();

	//Next are possible methods to output information to the UI
	/*void sendBestMove(int bestMove, int ponderMove);
	void sendStatus(int currentDepth, int currentMaxDepth, uint64_t totalNodes, int currentMove, int currentMoveNumber);
	void sendStatus(bool force, int currentDepth, int currentMaxDepth, uint64_t totalNodes, int currentMove,int currentMoveNumber);
	void sendMove(RootEntry entry, int currentDepth, int currentMaxDepth, uint64_t totalNodes);
	static std::string fromMove(int move);*/

private:
	const int msMaxByMove = 5000;
	Search search{ msMaxByMove };
	BoardRepresentation currentPosition;
	//std::chrono::system_clock::time_point startTime, statusStartTime;

	void receiveInitialize();
	void receiveReady();
	void receiveNewGame();
	void receivePosition(std::istringstream& input);
	void receiveGo(std::istringstream& input);
	void receivePonderHit();
	void receiveStop();
	void receiveQuit();
};


#endif

