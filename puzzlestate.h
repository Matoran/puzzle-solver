#ifndef PUZZLESTATE_H
#define PUZZLESTATE_H

#include <QTextStream>
#include <QVector>
#include <QPoint>


class PuzzleState
{
public:
    PuzzleState(int size, PuzzleState *parent = nullptr);
    ~PuzzleState();
    void configure();
    void display() const;
    bool find(const PuzzleState &final);
    PuzzleState* left();
    PuzzleState* right();
    PuzzleState* top();
    PuzzleState* bottom();
private:
    int size;
    QPoint position;
    PuzzleState *parent;
    QVector<int> puzzle;
    QTextStream cin{stdin};
    //QTextStream cout{stdout};
    PuzzleState *swap(int pos);
    bool check(const PuzzleState &actual, const PuzzleState &final);
};

#endif // PUZZLESTATE_H
