/*
 * Author: LOPES Marco
 * Purpose: Solve a puzzle
 * Date : november 2016
 */
#include <QCoreApplication>
#include <QQueue>
#include <QTextStream>
#include <QElapsedTimer>
#include "puzzlestate.h"
#include "puzzlesolver.h"

/*
 *   Create the puzzle and the call the puzzle solver
 *
 *   returns: EXIT_SUCCESS
 */
int main() {
    QTextStream cin(stdin);
    QTextStream cout(stdout);

    cout << "enter size of puzzle" << endl;
    int size(0);
    cin >> size;
    cout << size << "x" << size << endl;

    PuzzleState initialPuzzle(size);
    cout << "enter initial state" << endl;
    initialPuzzle.configure();
    initialPuzzle.display();

    PuzzleState finalPuzzle(size);
    cout << "enter final state" << endl;
    finalPuzzle.configure();
    finalPuzzle.display();
    cout << "cases bad placed: " << initialPuzzle.badPlaced(finalPuzzle) << endl;
    cout << "Manhattan distance: " << initialPuzzle.manhattan(finalPuzzle) << endl;
    cout << "===========================================================" << endl;
    cout << "===========================================================" << endl;
    cout << "0: Try without saving tested states" << endl;
    cout << "1: Try with saving tested states" << endl;
    cout << "2: Try without saving tested states but little trick" << endl;
    cout << "3: Try with number of bad placed" << endl;
    cout << "4: Try with Manhattan distance" << endl;
    cout << "===========================================================" << endl;
    cout << "===========================================================" << endl;
    cin >> size;
    QElapsedTimer timer;
    qint64 nanoSec;
    timer.start();
    if (PuzzleSolver::solve(initialPuzzle, finalPuzzle, (Option) size)) {
        nanoSec = timer.nsecsElapsed();
        cout << "solved in " << nanoSec / 1000 << " microsecond(s)" << endl;
    } else {
        cout << "unsolvable" << endl;
    }


    return EXIT_SUCCESS;
}
