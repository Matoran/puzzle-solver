/*
 * Author: LOPES Marco
 * Purpose: Stock a puzzle state data
 * Date : november 2016
 */
#include "puzzlestate.h"
#include <QTextStream>
#include <QVector>
#include <QQueue>
#include <QStack>
#include <QSet>
#include <iostream>

PuzzleState::PuzzleState(){}

/*
 *   Constructor, Initialise the size of the puzzle and the parent
 *
 *   size: size of the puzzle(3x3 or 4x4)
 *   parent: parent state or nullptr
 *
 */
PuzzleState::PuzzleState(int size, PuzzleState *parent) : size(size), parent(parent){
}

PuzzleState::~PuzzleState()
{
    delete leftChildren;
    delete rightChildren;
    delete topChildren;
    delete bottomChildren;
}

void PuzzleState::generateChildrens()
{
    int pos;
    if (this->position.x() > 0){
        pos = this->position.y()*this->size+this->position.x()-1;
        leftChildren = this->swap(pos);
    }
    if (this->position.x() < this->getSize() - 1) {
        pos = this->position.y()*this->size+this->position.x()+1;
        rightChildren = this->swap(pos);
    }
    if (this->position.y() > 0) {
        pos = (this->position.y()-1)*this->size+this->position.x();
        topChildren =  this->swap(pos);
    }
    if (this->position.y() < this->getSize() - 1) {
        pos = (this->position.y()+1)*this->size+this->position.x();
        bottomChildren = this->swap(pos);
    }
}

/*
 *   Read puzzle configuration from console
 *
 *   returns: void
 */
void PuzzleState::configure()
{
    for (int i = 0; i < this->size; ++i) {
        for (int j = 0; j < this->size; ++j) {
            int number;
            std::cin >> number;
            this->puzzle << number;
            if(number == size*size){
                position.setX(j);
                position.setY(i);
            }
        }
    }
}

/*
 *   Display the disposition of the puzzle
 *
 *   returns: void
 */
void PuzzleState::display() const
{
    QString separation("+---");
    for (int i = 0; i < this->size; ++i) {
        std::cout << " " << separation.repeated(this->size).toStdString() << "+" <<  std::endl;
        for (int j = 0; j < this->size; ++j) {
            if(this->puzzle.at(i*size+j) == size*size)
                 std::cout << " |  ";
            else if(this->puzzle.at(i*size+j) <= 9)
                std::cout << " | " << this->puzzle.at(i*size+j);
            else
                std::cout << " |" << this->puzzle.at(i*size+j);
        }
        std::cout << " | " << std::endl;
    }
    std::cout << " " << separation.repeated(this->size).toStdString() << "+" <<  std::endl;
}


/*
 *   Getter of left children
 *
 *   returns: PuzzleState* left children
 */
PuzzleState *PuzzleState::getLeft()
{
    return this->leftChildren;
}

/*
 *   Getter of right children
 *
 *   returns: PuzzleState* right children
 */
PuzzleState *PuzzleState::getRight()
{
    return this->rightChildren;
}

/*
 *   Getter of top children
 *
 *   returns: PuzzleState* top children
 */
PuzzleState *PuzzleState::getTop()
{
    return this->topChildren;
}

/*
 *   Getter of bottom children
 *
 *   returns: PuzzleState* bottom children
 */
PuzzleState *PuzzleState::getBottom()
{
    return this->bottomChildren;
}

/*
 *   Swap the blank case with pos
 *
 *   pos: the new position of the blank case
 *
 *   returns: PuzzleState* the new state
 */
PuzzleState *PuzzleState::swap(int pos){
    int posSizeSquare = this->position.y()*this->size+this->position.x();
    PuzzleState *state = new PuzzleState(this->size, this);
    state->puzzle = this->puzzle;
    state->puzzle.replace(posSizeSquare, state->puzzle.at(pos));
    state->puzzle.replace(pos, this->size*this->size);
    state->position.setX(pos % this->size);
    state->position.setY(pos / this->size);
    return state;

}

/*
 *   Getter of puzzle
 *
 *   returns: QVector<int> the puzzle data
 */
QVector<int> PuzzleState::getPuzzle() const
{
    return puzzle;
}

/*
 *   Getter of parent
 *
 *   returns:PuzzleState* the parent
 */
PuzzleState *PuzzleState::getParent() const
{
    return parent;
}

/*
 *   Getter of size
 *
 *   returns: int the size of the puzzle
 */
int PuzzleState::getSize() const
{
    return size;
}

/*
 *   Getter of position of blank case(9 or 16)
 *
 *   returns: QPoint the position of the blank case
 */
QPoint PuzzleState::getPosition() const
{
    return position;
}

/*
 *   Get the total distance between actual and final state
 *
 *   returns: int the number of cases bad placed
 */
int PuzzleState::manhattan(const PuzzleState &final) const
{
    int cost = 0;
    for (int line = 0; line < this->size; ++line) {
        for (int column = 0; column < this->size; ++column) {
            int value = this->puzzle.at(line*this->size+column);
            int index = final.getPuzzle().indexOf(value);
            int lineFinal = index / 3;
            int columnFinal = index % 3;
            cost += qAbs(line-lineFinal) + qAbs(column-columnFinal);
        }
    }
    return cost;
}

/*
 *   Get the number of cases bad placed
 *
 *   returns: int the number of cases bad placed
 */
int PuzzleState::badPlaced(const PuzzleState &final) const
{
    int count = 0;
    for (int line = 0; line < this->size; ++line) {
        for (int column = 0; column < this->size; ++column) {
            if(this->puzzle.at(line*this->size+column) != final.getPuzzle().at(line*this->size+column))
                count++;
        }
    }
    return count;
}
