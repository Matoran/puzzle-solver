#include <QCoreApplication>
#include <QQueue>
#include <QTextStream>
#include <QDebug>
#include "puzzlestate.h"

int main(int argc, char *argv[])
{

    QCoreApplication a(argc, argv);
    QTextStream cin(stdin);
    QTextStream cout(stdout);
    cout << "enter size of puzzle" << endl;
    int size(0);
    cin >> size;
    cout << size << "x" << size << endl;
    PuzzleState initialPuzzle(size), finalPuzzle(size);
    cout << "enter initial state" << endl;
    initialPuzzle.configure();
    initialPuzzle.display();
    cout << "enter final state" << endl;
    finalPuzzle.configure();
    finalPuzzle.display();
    initialPuzzle.find(finalPuzzle);
    return a.exec();
}
