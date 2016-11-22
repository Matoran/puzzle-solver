#ifndef PUZZLESTATE_H
#define PUZZLESTATE_H

#include <QTextStream>
#include <QVector>
#include <QPoint>


class PuzzleState
{
public:
    PuzzleState();
    PuzzleState(int size, PuzzleState *parent = nullptr);
    ~PuzzleState();
    void configure();
    void random();
    void display() const;
    PuzzleState *getLeft();
    PuzzleState *getRight();
    PuzzleState *getTop();
    PuzzleState *getBottom();
    QVector<int> getPuzzle() const;
    PuzzleState *getParent() const;
    int getSize() const;
    QPoint getPosition() const;
    int badPlaced(const PuzzleState &final) const;
    int manhattan(const PuzzleState &final) const;
    void generateChildrens();

private:
    int size;
    QPoint position;
    PuzzleState *parent = nullptr;
    PuzzleState *leftChildren = nullptr;
    PuzzleState *rightChildren = nullptr;
    PuzzleState *topChildren = nullptr;
    PuzzleState *bottomChildren = nullptr;
    QVector<int> puzzle;
    PuzzleState *swap(int pos);

};

inline bool operator ==(PuzzleState s1, PuzzleState s2){
    return s1.getPuzzle() == s2.getPuzzle();
}

inline uint qHash(const PuzzleState &puzzle)
{
    return qHash(puzzle.getPuzzle());
}

#endif // PUZZLESTATE_H
