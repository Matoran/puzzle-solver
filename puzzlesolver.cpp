/*
 * Author: LOPES Marco
 * Purpose: Solve a puzzle
 * Date : november 2016
 */
#include "puzzlesolver.h"
#include <QSet>
#include <QQueue>
#include <QStack>
#include <QDebug>
#include <iostream>
#include "puzzlestate.h"

PuzzleSolver::PuzzleSolver() {}

/*
 *   Compare the actual puzzle with the final puzzle
 *   Display solution if actual == final
 *
 *   actual: state of actual puzzle
 *   final: state of final puzzle
 *
 *   returns: true if equal, false otherwise
 */
bool PuzzleSolver::check(PuzzleState &actual, const PuzzleState &final) {
    if (actual.getPuzzle() == final.getPuzzle()) {

        //create a stack to display solution in correct order
        QStack <PuzzleState*> stack;
        PuzzleState *state = &actual;
        stack.push(state);
        //follow the parent and push all states
        do {
            state = state->getParent();
            stack.push(state);

        } while (state->getParent() != nullptr);

        //display the solution
        std::cout << "========================" << std::endl;
        std::cout << "=========SOLUTION=======" << std::endl;
        std::cout << "========================" << std::endl;
        std::cout << "solved in " << stack.size() << " moves" << std::endl;
        while (!stack.isEmpty()) {
            stack.pop()->display();
        }
        std::cout << "========================" << std::endl;
        std::cout << "===========END==========" << std::endl;
        std::cout << "========================" << std::endl;
        return true;
    }
    return false;
}

/*
 *   Add a new state
 *
 *   state: state to add
 *   final: state to found
 *   queue: used for breadth first search
 *   states: used to not have twice the same state
 *
 *   returns: true if solved false otherwise
 */
bool PuzzleSolver::addState(PuzzleState *state, const PuzzleState &final, QQueue<PuzzleState *> &queue,
                            QSet <PuzzleState> &states) {
    if(state == nullptr)
        return false;
    if (!states.contains(*state)) {
        states.insert(*state);
        //if the state is the final state
        if (check(*state, final)) {
            return true;
            //else add enqueue the state to create childrens later
        } else {
            queue.enqueue(state);
        }
    }
    return false;
}

/*
 *   Add a new state
 *
 *   state: state to add
 *   final: state to found
 *   queue: used for breadth first search
 *
 *   returns: true if solved false otherwise
 */
bool PuzzleSolver::addState(PuzzleState *state, const PuzzleState &final, QQueue<PuzzleState *> &queue) {
    if(state == nullptr)
        return false;
    //if the state is the final state
    if (check(*state, final)) {
        return true;
        //else add enqueue the state to create childrens later
    } else {
        queue.enqueue(state);
    }
    return false;
}


/*
 *   Add a new state
 *
 *   state: state to add
 *   final: state to found
 *   queue: used for breadth first search
 *   key: used to sort the queue(lower value first)
 *   states: used to not have twice the same state
 *
 *   returns: true if solved false otherwise
 */
bool PuzzleSolver::addState(PuzzleState *state, const PuzzleState &final, QMultiMap<int, PuzzleState *> &queue, int key,
                            QSet <PuzzleState> &states) {
    if(state == nullptr)
        return false;
    if (!states.contains(*state)) {
        states.insert(*state);
        //if the state is the final state
        if (check(*state, final)) {
            return true;
            //else add enqueue the state to create childrens later
        } else {
            queue.insert(key, state);
        }
    }
    return false;
}

/*
 *   Try to solve puzzle
 *
 *   start: initial state
 *   final: state to join from initial state
 *
 *   returns: true if solved false otherwise
 */
bool PuzzleSolver::solve(PuzzleState &start, const PuzzleState &final, Option option) {
    switch (option) {
    case withoutSavedStates:
        return solveWithoutSavedStates(start, final);
        break;
    case withoutSavedStates4x4:
        return solveWithoutSavedStates4x4(start, final);
        break;
    case withSavedStates:
        return solveWithSavedStates(start, final);
        break;
    case badPlaced:
        return solveWithSavedStatesAndHeuristicBadPlaced(start, final);
        break;
    case manhattan:
        return solveWithSavedStatesAndHeuristicManhattan(start, final);
        break;
    case automatic:
        if (start.getSize() == 3) {
            return solveWithSavedStates(start, final);
        } else {
            return solveWithoutSavedStates4x4(start, final);
        }
        break;
    default:
        break;
    }
    return false;
}

/*
 *   Try to solve puzzle with saving old states
 *
 *   start: initial state
 *   final: state to join from initial state
 *
 *   returns: true if solved false otherwise
 */
bool PuzzleSolver::solveWithSavedStates(PuzzleState &start, const PuzzleState &final) {
    QQueue < PuzzleState * > queue;
    QSet <PuzzleState> states;

    if (start.getPuzzle() == final.getPuzzle()) {
        return true;
    } else {
        states.insert(start);
        queue.enqueue(&start);
        int i = 0;
        //breadth first search
        while (!queue.isEmpty()) {
            PuzzleState *actualState = queue.dequeue();
            //generate all childrens of actual state and add thems to the queue
            actualState->generateChildrens();
            if (addState(actualState->getLeft(), final, queue, states))
                return true;
            if (addState(actualState->getRight(), final, queue, states))
                return true;
            if (addState(actualState->getTop(), final, queue, states))
                return true;
            if (addState(actualState->getBottom(), final, queue, states))
                return true;
            i++;
            if (queue.size() > LIMIT) {
                std::cout << LIMIT << " reached" << std::endl;
                queue.dequeue()->display();
                return false;
            }
        }
        return false;
    }
}


/*
 *   Try to solve puzzle without saving old states
 *
 *   start: initial state
 *   final: state to join from initial state
 *
 *   returns: true if solved false otherwise
 */
bool PuzzleSolver::solveWithoutSavedStates(PuzzleState &start, const PuzzleState &final) {
    QQueue < PuzzleState * > queue;
    if (start.getPuzzle() == final.getPuzzle()) {
        return true;
    } else {
        queue.enqueue(&start);
        int i = 0;
        //breadth first search
        while (!queue.isEmpty()) {
            PuzzleState *actualState = queue.dequeue();
            //generate all childrens of actual state and add thems to the queue
            actualState->generateChildrens();
            if (addState(actualState->getLeft(), final, queue))
                return true;
            if (addState(actualState->getRight(), final, queue))
                return true;
            if (addState(actualState->getTop(), final, queue))
                return true;
            if (addState(actualState->getBottom(), final, queue))
                return true;
            i++;
            if (queue.size() > LIMIT) {
                std::cout << LIMIT << " reached" << std::endl;
                queue.dequeue()->display();
                return false;
            }
        }
        return false;
    }
}

/*
 *   Try to solve puzzle without saving old states with little trick
 *
 *   start: initial state
 *   final: state to join from initial state
 *
 *   returns: true if solved false otherwise
 */
bool PuzzleSolver::solveWithoutSavedStates4x4(PuzzleState &start, const PuzzleState &final) {
    QQueue < PuzzleState * > queue;
    if (start.getPuzzle() == final.getPuzzle()) {
        return true;
    } else {
        queue.enqueue(&start);
        int i = 0;
        //breadth first search
        while (!queue.isEmpty()) {
            PuzzleState *actualState = queue.dequeue();
            //generate all childrens of actual state and add thems to the queue
            actualState->generateChildrens();
            PuzzleState *left = actualState->getLeft();
            if (left != nullptr && (left->getParent()->getParent() == nullptr || left->getPuzzle() != left->getParent()->getParent()->getPuzzle()))
                if (addState(left, final, queue))
                    return true;
            PuzzleState *right = actualState->getRight();
            if (right != nullptr && (right->getParent()->getParent() == nullptr || right->getPuzzle() != right->getParent()->getParent()->getPuzzle()))
                if (addState(right, final, queue))
                    return true;
            PuzzleState *top = actualState->getTop();
            if (top != nullptr && (top->getParent()->getParent() == nullptr || top->getPuzzle() != top->getParent()->getParent()->getPuzzle()))
                if (addState(top, final, queue))
                    return true;
            PuzzleState *bottom = actualState->getBottom();
            if (bottom != nullptr && (bottom->getParent()->getParent() == nullptr || bottom->getPuzzle() != bottom->getParent()->getParent()->getPuzzle()))
                if (addState(bottom, final, queue))
                    return true;
            i++;
            if (queue.size() > LIMIT) {
                std::cout << LIMIT << " reached" << std::endl;
                queue.dequeue()->display();
                return false;
            }
        }
        return false;
    }
}

/*
 *   Try to solve puzzle with saving old states and queue is sorted by the number of cases bad placed
 *
 *   start: initial state
 *   final: state to join from initial state
 *
 *   returns: true if solved false otherwise
 */
bool PuzzleSolver::solveWithSavedStatesAndHeuristicBadPlaced(PuzzleState &start, const PuzzleState &final) {
    QMultiMap < int, PuzzleState * > queue;
    QSet <PuzzleState> states;

    if (start.getPuzzle() == final.getPuzzle()) {
        return true;
    } else {
        states.insert(start);
        queue.insert(start.badPlaced(final), &start);
        //breadth first search
        while (!queue.isEmpty()) {
            PuzzleState *actualState = queue.lowerBound(0).value();
            //generate all childrens of actual state and add thems to the queue
            actualState->generateChildrens();
            queue.remove(queue.lowerBound(0).key(), actualState);
            PuzzleState *left = actualState->getLeft();
            if (left != nullptr && addState(left, final, queue, left->badPlaced(final), states))
                return true;
            PuzzleState *right = actualState->getRight();
            if (right != nullptr && addState(right, final, queue, right->badPlaced(final), states))
                return true;
            PuzzleState *top = actualState->getTop();
            if (top != nullptr && addState(top, final, queue, top->badPlaced(final), states))
                return true;
            PuzzleState *bottom = actualState->getBottom();
            if (bottom != nullptr && addState(bottom, final, queue, bottom->badPlaced(final), states))
                return true;
            if (queue.size() > LIMIT) {
                std::cout << LIMIT << " reached" << std::endl;
                queue.lowerBound(0).value()->display();
                return false;
            }
        }
        return false;
    }
}

/*
 *   Try to solve puzzle with saving old states and queue is sorted by the Manhattan distance
 *
 *   start: initial state
 *   final: state to join from initial state
 *
 *   returns: true if solved false otherwise
 */
bool PuzzleSolver::solveWithSavedStatesAndHeuristicManhattan(PuzzleState &start, const PuzzleState &final) {
    QMultiMap < int, PuzzleState * > queue;
    QSet <PuzzleState> states;

    if (start.getPuzzle() == final.getPuzzle()) {
        return true;
    } else {
        states.insert(start);
        queue.insert(start.badPlaced(final), &start);
        //breadth first search
        int i = 0;
        while (!queue.isEmpty()) {
            i++;
            PuzzleState *actualState = queue.lowerBound(0).value();
            //generate all childrens of actual state and add thems to the queue
            actualState->generateChildrens();
            queue.remove(queue.lowerBound(0).key(), actualState);
            PuzzleState *left = actualState->getLeft();
            if (left != nullptr && addState(left, final, queue, left->manhattan(final), states))
                return true;
            PuzzleState *right = actualState->getRight();
            if (right != nullptr && addState(right, final, queue, right->manhattan(final), states))
                return true;
            PuzzleState *top = actualState->getTop();
            if (top != nullptr && addState(top, final, queue, top->manhattan(final), states))
                return true;
            PuzzleState *bottom = actualState->getBottom();
            if (bottom != nullptr && addState(bottom, final, queue, bottom->manhattan(final), states))
                return true;
            if (queue.size() > LIMIT) {
                std::cout << LIMIT << " reached" << std::endl;
                queue.lowerBound(0).value()->display();
                return false;
            }
            //std::cout << i << "count" << std::endl;
        }
        return false;
    }
}
