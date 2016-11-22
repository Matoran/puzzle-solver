#ifndef PUZZLESOLVER_H
#define PUZZLESOLVER_H

#include "puzzlestate.h"
#include "puzzlestate.h"
#include <QQueue>
#include <QSet>

enum Option {withoutSavedStates, withSavedStates, withoutSavedStates4x4, badPlaced, manhattan, automatic};

class PuzzleSolver
{
public:
    PuzzleSolver();
    static bool solve(PuzzleState &start, const PuzzleState &final, Option option = automatic);
private:
    static const int LIMIT = 1e8;
    static bool check(PuzzleState &actual, const PuzzleState &final);
    static bool addState(PuzzleState *state, const PuzzleState &final, QQueue<PuzzleState*> &queue, QSet<PuzzleState> &states);
    static bool addState(PuzzleState *state, const PuzzleState &final, QQueue<PuzzleState*> &queue);
    static bool addState(PuzzleState *state, const PuzzleState &final, QMultiMap<int, PuzzleState*> &queue, int key, QSet<PuzzleState> &states);
    static bool solveWithSavedStates(PuzzleState &start, const PuzzleState &final);
    static bool solveWithoutSavedStates(PuzzleState &start, const PuzzleState &final);
    static bool solveWithoutSavedStates4x4(PuzzleState &start, const PuzzleState &final);
    static bool solveWithSavedStatesAndHeuristicBadPlaced(PuzzleState &start, const PuzzleState &final);
    static bool solveWithSavedStatesAndHeuristicManhattan(PuzzleState &start, const PuzzleState &final);

};



#endif // PUZZLESOLVER_H
