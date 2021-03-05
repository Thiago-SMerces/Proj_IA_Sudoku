/* 
* Header file for sudoku solver
* define key classes and macros (LEN)
*/
#ifndef __SUDOKU_HPP
#define __SUDOKU_HPP

#include <fstream>
#include <vector>
#include <queue>

/* 
* Macro to define default sudoku LEN, as for now, it will always assume
* that the sudoku is a 9x9 matrix
*/
#define LEN 9

/* 
* Structure to represent each state from the board
* i.e. each time a different number is added on the
* sudoku matrix (board), a new state is created.
* Note that there are two names to the structure just to 
* make it easier to identify in which step we are when 
* solving it.
*/
typedef struct State
{
    char sudoku_matrix[LEN][LEN];
    int costF;
    int costG;
    int costH;
    bool solved;
    std::vector<int> valid_numbers  = {49, 50, 51, 52, 53, 54, 55, 56, 57, 58};

    // define default operator so priority queue may work
    bool operator<(const State& next) const
    {
        return costF < next.costF;
    }
} State, Node;

// class to define common functions to solve sudoku
class Solver
{
    public:
    // default initializer to start solving sudoku
    virtual State solve(State problem) = 0;
    
    // print the entire solved sudoku in one line
    void printSudoku(char matrix[LEN][LEN]);

    // check if sudoku matrix is the goal (fully filled with non-clashing numbers)
    bool isGoal(State state);

    // check if the number we are trying to assign does not conflict with the row, column or square
    bool isValid(State state, int row, int col, int number);

    // find the first empty spot (dot) while iterating over the sudoku
    int* findDot(State state);

    // count how many possible numbers a spot with a dot could be filled with 
    int numbersInRegion(State state, int row, int col);
};

// class to implement bfs search to solve sudoku
class BFS : public Solver
{
    public:
    // solve problem using breadth-first search (bfs)
    State bfs(State problem);

    // overwrite default Solver initializer to solve problem using bfs
    State solve(State problem)
    {
        return bfs(problem);
    }
};

// class to implement dfs search to solve sudoku
class DFS : public Solver
{
    public:
    // solve problem using depth-first search (dfs)
    State dfs(State problem);

    // overwrite default Solver initializer to solve problem using dfs
    State solve(State problem)
    {
        return dfs(problem);
    }
};

// class to implement a* search to solve sudoku
class AStar : public Solver
{
    public:
    // solve problem using a star search (a*)
    State aStar(State problem);

    // overwrite default Solver initializer to solve problem using a*
    State solve(State problem)
    {
        return aStar(problem);
    }
};

// class to implement backtracking search to solve sudoku
class BACK : public Solver
{
    public:
    // to continue with the idea of continuously solving the matrix
    // use pointers instead of direct copies of values to find the
    // dot location (coordinates). Returns bool so we can know if 
    // there is still a dot or not on the matrix
    bool findDot(State *state, int* i, int* j);

    // literally the same function defined in the parent class,
    // but it accepts a pointer to the state we are solving
    bool isValid(State* state, int row, int col, int number);

    // solve problem using backtracking
    // i and j are optional parameters (defined in the source code)
    // since we want a continuos solution to a state instead of creating
    // new ones
    void back(State* problem, int i, int j);

    // overwrite default Solver initializer to solve problem using backtracking
    // this one needs a little more set up, since we need the problem to 
    // be returned as a State instead of a pointer to keep the code working
    State solve(State problem)
    {
        back(&problem, 0, 0);
        return problem;
    }
};

// class to implement ac3 search to solve sudoku
class AC3 : public Solver
{
    public:
    // solve problem using as a Constraint Satisfaction Problem
    State ac3(State problem);

    // overwrite default Solver initializer to solve problem using ac3
    State solve(State problem)
    {
        // problem.valid_numbers = {49, 50, 51, 52, 53, 54, 55, 56, 57, 58};
        // problemac3(problem);
        return ac3(problem);
    }
};

#endif