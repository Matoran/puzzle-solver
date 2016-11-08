#include "puzzlestate.h"
#include <QTextStream>
#include <QVector>
#include <QDebug>
#include <QQueue>
#include <QStack>
#include <iostream>

PuzzleState::PuzzleState(int size, PuzzleState *parent) : size(size), parent(parent)
{
}

PuzzleState::~PuzzleState()
{
}

void PuzzleState::configure()
{
    for (int i = 0; i < this->size; ++i) {
        for (int j = 0; j < this->size; ++j) {
            int number;
            cin >> number;
            this->puzzle << number;
            if(number == 9){
                position.setX(j);
                position.setY(i);
            }
        }
    }
}

void PuzzleState::display() const
{
    for (int i = 0; i < this->size; ++i) {
        for (int j = 0; j < this->size; ++j) {
            std::cout << this->puzzle.at(i*size+j) << "\t";
        }
         std::cout <<  std::endl;
    }
     std::cout << "position" << position.x() << " " << position.y() <<  std::endl;
}

bool PuzzleState::check(const PuzzleState &actual, const PuzzleState &final){
    if(actual.puzzle == final.puzzle){
        QStack<const PuzzleState*> stack;
        const PuzzleState *state = &actual;
        do{
            stack.push(state);
            state = state->parent;
        }while(state != nullptr);
         std::cout << "========================" <<  std::endl;
         std::cout << "=========SOLUTION=======" <<  std::endl;
         std::cout << "========================" <<  std::endl;
        while(!stack.isEmpty()){
            stack.pop()->display();
        }
         std::cout << "========================" <<  std::endl;
         std::cout << "===========FIN==========" <<  std::endl;
         std::cout << "========================" <<  std::endl;
        return true;
    }
    return false;
}

bool PuzzleState::find(const PuzzleState &final)
{
    if(final.puzzle == this->puzzle){
        qDebug() << "same" << endl;
        return true;
    }else{
        QVector<PuzzleState*> states;
        states.append(this);
        QQueue<PuzzleState*> queue;
        queue.enqueue(this);
        while(!queue.isEmpty()){
            PuzzleState *actualState = queue.dequeue();
            if(actualState->position.x() > 0){
                states.append(actualState->left());
                if(this->check(*states.constLast(), final)){
                    return true;
                }else{
                    queue.enqueue(states.last());
                }
            }
            if(actualState->position.x() < this->size-1){
                states.append(actualState->right());
                if(this->check(*states.constLast(), final)){
                    return true;
                }else{
                    queue.enqueue(states.last());
                }
            }
            if(actualState->position.y() > 0){
                states.append(actualState->top());
                if(this->check(*states.constLast(), final)){
                    return true;
                }else{
                    queue.enqueue(states.last());
                }
            }
            if(actualState->position.y() < this->size-1){
                states.append(actualState->bottom());
                if(this->check(*states.constLast(), final)){
                    return true;
                }else{
                    queue.enqueue(states.last());
                }
            }
            qDebug() << states.size() << endl;
        }
        return false;
    }
}

PuzzleState* PuzzleState::left()
{
    int pos = this->position.y()*this->size+this->position.x()-1;
    return this->swap(pos);
}

PuzzleState* PuzzleState::right()
{
    int pos = this->position.y()*this->size+this->position.x()+1;
    return this->swap(pos);
}

PuzzleState* PuzzleState::top()
{
    int pos = (this->position.y()-1)*this->size+this->position.x();
    return this->swap(pos);
}

PuzzleState* PuzzleState::bottom()
{
    int pos = (this->position.y()+1)*this->size+this->position.x();
    return this->swap(pos);
}

PuzzleState* PuzzleState::swap(int pos){
    int posSizeSquare = this->position.y()*this->size+this->position.x();
    PuzzleState* state = new PuzzleState(this->size, this);
    state->puzzle = this->puzzle;
    state->puzzle.replace(posSizeSquare, state->puzzle.at(pos));
    state->puzzle.replace(pos, this->size*this->size);
    state->position.setX(pos % this->size);
    state->position.setY(pos / this->size);
    std::cout << "children" <<  std::endl;
    state->display();
    return state;
}
