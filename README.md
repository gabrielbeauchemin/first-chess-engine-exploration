All codes has been written from scratched (except for the chess protocol). Using Arena, the engine works but one move in about 20, it makes the engine crashed and it needs to be restarted to be able to contine playing. The crashes come from the function that find legal moved, which still has errors. The engine is very slow to explore possible moves. That is because the function to get legal moves is too slow and the board representation is too simple. Also alpha beta probably has some problems : some move doesn't seem to make sense.

**Choices**

BoardRepresentation: Simple array of 64 pieces. Possibility to move and unmove pieces.

GenerateMoves: Mailbox to get unbound moves. Perft deepness 4 finds all moves in 1 sec. Peft deepness 5 finds all moves except 36 in less than a minutes (very too long). 

Moves Exploration: Alpha Beta with deepness iterative to be sure to find a good move in little time.

Heuristic: Simple materialistic heuristic with a beginning of consideration for the state of the game (beginning or end).

Chess protocol: UCI.