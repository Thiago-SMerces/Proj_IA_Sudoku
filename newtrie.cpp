/* 
* Source file for sudoku solver
* main execution and functions 
* are found here
*/
#include <bits/stdc++.h>
#include <chrono>
#include "sudoku.hpp"

using namespace std::chrono;
using namespace std;
/* 
    Used ASCII table values for reference

    |    ASCII  CODE    |    CHAR    |
    |         46        |     .      |
    |         48        |     0      |
    |         49        |     1      |
    |         50        |     2      |
    |         51        |     3      |
    |         52        |     4      |
    |         53        |     5      |
    |         54        |     6      |
    |         55        |     7      |
    |         56        |     8      |
    |         57        |     9      |
*/

void Solver::printSudoku(char matrix[LEN][LEN]) {
    for(int i = 0; i < LEN; i++)
        for(int j = 0; j < LEN; j++)
            cout << matrix[i][j];
        
    cout << endl;
}

bool Solver::isGoal(State state) {
    for(int i = 0; i < LEN; i++)
        for(int j = 0; j < LEN; j++)
            if(state.sudoku_matrix[i][j] == 46)
                return false;
        
    return true;
}

bool Solver::isValid(State state, int row, int col, int number) {
    for(int i = 0; i < LEN; i++)
        if(state.sudoku_matrix[row][i] == number)
            return false;

    for(int j = 0; j < LEN; j++)
        if(state.sudoku_matrix[j][col] == number)
            return false;

    int startI = row - row % 3, startJ = col - col % 3;
    for(int i = 0; i < 3; i++)
        for(int j = 0; j < 3; j++)
            if(state.sudoku_matrix[i + startI][j + startJ] == number)
                return false;

    return true;
}

int* Solver::findDot(State state) {
    int* ret = new int[2];
    ret[0] = -1;
    ret[1] = -1;
    for(int i = 0; i < LEN; i++) {
        for(int j = 0; j < LEN; j++) { 
            if(state.sudoku_matrix[i][j] == 46) {
                ret[0] = i;
                ret[1] = j;
                return ret;
            }
        }
    }
    return ret;
}

int Solver::numbersInRegion(State state, int row, int col) {
    int numbersInRegion = 0;
    vector<int> coordinatesI;
    vector<int> coordinatesJ;
    for(int i = 0; i < LEN; i++) {
        if(isdigit(state.sudoku_matrix[row][i])) {
            numbersInRegion++;
            coordinatesI.push_back(row);
            coordinatesJ.push_back(i);
        }
    }

    for(int j = 0; j < LEN; j++) {
        if(isdigit(state.sudoku_matrix[j][col])) {
            for(int validate = 0; validate < coordinatesI.size(); validate++) {
                if(!(coordinatesJ[validate] == col && coordinatesI[validate] == j)) {
                    numbersInRegion++;
                    coordinatesI.push_back(j);
                    coordinatesJ.push_back(col);
                }
            }
        }
    }

    int startI = row - row % 3, startJ = col - col % 3;
    for(int i = 0; i < 3; i++)
        for(int j = 0; j < 3; j++)
            if(isdigit(state.sudoku_matrix[i + startI][j + startJ]))
                for(int validate = 0; validate < coordinatesI.size(); validate++)
                    if(!(coordinatesJ[validate] == i + startI && coordinatesI[validate] == j + startJ))
                        numbersInRegion++;
    return numbersInRegion;
}

State BFS::bfs(State problem) {
    Node node = problem;
    // check if state is already solved
    if(isGoal(node)) {
        node.solved = true;
        return node;
    }
    queue<Node> queue;
    queue.push(node);

    while(!queue.empty()) {
        node = queue.front();
        queue.pop();
        int* coord = findDot(node);

        // this step ultimately replaces isGoal since it checks the entire matrix for empty spots
        if(coord[0] == -1) {
            delete[] coord;
            node.solved = true;
            return node;
        }

        // copy matrix beforehand to avoid copying it for every valid number
        Node child;
        for(int i = 0; i < LEN; i++)
            for(int j = 0; j < LEN; j++)
                child.sudoku_matrix[i][j] = node.sudoku_matrix[i][j];

        for(int number = 49; number < 58; number++) {
            if(isValid(node, coord[0], coord[1], number)) {
                child.sudoku_matrix[coord[0]][coord[1]] = number;
                queue.push(child);
                child.sudoku_matrix[coord[0]][coord[1]] = 46;
            }
        }
        // clear pointer to coordinates
        delete[] coord;
    }
    node.solved = false;
    return node;
}

State DFS::dfs(State problem) {
    Node node = problem;
    // check if state is already solved
    if(isGoal(node)) {
        node.solved = true;
        return node;        
    }
    stack<Node> stack;
    stack.push(node);

    while(!stack.empty()) {
        node = stack.top();
        stack.pop();
        int* coord = findDot(node);

        // this step ultimately replaces isGoal since it checks the entire matrix for empty spots
        if(coord[0] == -1) {
            delete[] coord;
            node.solved = true;
            return node;
        }

        // copy matrix beforehand to avoid copying it for every valid number
        Node child;
        for(int i = 0; i < LEN; i++)
            for(int j = 0; j < LEN; j++)
                child.sudoku_matrix[i][j] = node.sudoku_matrix[i][j];

        for(int number = 49; number < 58; number++) {
            if(isValid(node, coord[0], coord[1], number)) {
                child.sudoku_matrix[coord[0]][coord[1]] = number;
                stack.push(child);
                child.sudoku_matrix[coord[0]][coord[1]] = 46;
            }
        }
        // clear pointer to coordinates
        delete[] coord;
    }
    node.solved = false;
    return node;
}

State AStar::aStar(State problem) {
    Node node = problem;
    node.costG = 0;
    node.costF = 0;
    int possibilities = 0;
    // check if state is already solved
    if(isGoal(node)) {
        node.solved = true;
        return node;
    }
    priority_queue<Node> pqueue;
    pqueue.push(node);

    while(!pqueue.empty()) {
        node = pqueue.top();
        pqueue.pop();
        int* coord = findDot(node);

        // this step ultimately replaces isGoal since it checks the entire matrix for empty spots
        if(coord[0] == -1) {
            delete[] coord;
            node.solved = true;
            return node;
        }

        // copy matrix beforehand to avoid copying it for every valid number
        Node child;
        for(int i = 0; i < LEN; i++)
            for(int j = 0; j < LEN; j++)
                child.sudoku_matrix[i][j] = node.sudoku_matrix[i][j];

        for(int number = 49; number < 58; number++) {
            if(isValid(node, coord[0], coord[1], number)) {
                // count possible numbers that may occupy this empty space (dot)
                // keep counting until we're out of numbers to test
                possibilities++;
                int child_number = number + 1;
                while(child_number < 58) {
                    if(isValid(node, coord[0], coord[1], child_number))
                        possibilities++;
                    child_number++;
                }

                // give all members of struct their respective values
                child.sudoku_matrix[coord[0]][coord[1]] = number;
                child.costG = possibilities;
                child.costH = numbersInRegion(child, coord[0], coord[1]);
                child.costF = child.costG + child.costH;
                pqueue.push(child);
                child.sudoku_matrix[coord[0]][coord[1]] = 46;
            }
        }
        // clear pointer to coordinates
        delete[] coord;
    }
    node.solved = false;
    return node;
}

bool BACK::findDot(State *state, int* i, int* j) {
    int cont = 0;
    int minpossibilities = 9;
    bool found = false;
    for (int row = 0; row < LEN; row++) {
        for (int column = 0; column < LEN; column++) {
            cont = 0;
            if(state->sudoku_matrix[row][column] == 46) {
                for (int number = 49; number < 58; number++){
                    if(isValid(state, row, column, number))
                        cont++;
                }
                if (minpossibilities > cont){
                    minpossibilities = cont;
                    *i = row;
                    *j = column;
                }
                found = true;
            }
            // l++;
        }
        // l = 0;
        // k++;
    }
    // found = false;
    return found;
}

bool BACK::isValid(State* state, int row, int col, int number) {
    int i, j;
    for(i = 0; i < LEN; i++)
        if(state->sudoku_matrix[row][i] == number || state->sudoku_matrix[i][col] == number)
            return false;

    int startI = row - row % 3, startJ = col - col % 3;
    for(i = 0; i < 3; i++)
        for(j = 0; j < 3; j++)
            if(state->sudoku_matrix[i + startI][j + startJ] == number)
                return false;

    return true;
}

void BACK::back(State* cps, int i = 0, int j = 0) {
    // check if there are empty spots on matrix, if not we found the solution
    if(!findDot(cps, &i, &j)) {
        cps->solved = true;
        return;
    }

    // for each possibility, try to fill the matrix with valid numbers
    for(int number = 49; number < 58; number++) {
        if(isValid(cps, i, j, number)) {
            cps->sudoku_matrix[i][j] = number;
            // call recursively starting at coordinates from previous call
            // instead of 0
            back(cps, i, j);
            // if matrix is solved, return to finish recursion
            if (cps->solved) 
                return;
            cps->sudoku_matrix[i][j] = 46;
        }
    }
    cps->solved = false;
    return;
}

State AC3::ac3(State problem) 
{
    Node node = problem;
    // check if state is already solved
    if (isGoal(node))
    {
        node.solved = true;
        return node;        
    }
    stack<Node> stack;
    stack.push(node);

    while(!stack.empty()) {
        node = stack.top();
        stack.pop();
        int* coord = findDot(node);

        // this step ultimately replaces isGoal since it checks the entire matrix for empty spots
        if(coord[0] == -1) {
            delete[] coord;
            node.solved = true;
            return node;
        }

        // copy matrix beforehand to avoid copying it for every valid number
        Node child;
        for(int i = 0; i < LEN; i++)
            for(int j = 0; j < LEN; j++)
                child.sudoku_matrix[i][j] = node.sudoku_matrix[i][j];

        for(int number = node.valid_numbers[0]; number < node.valid_numbers.back(); number++) {
            // node.valid_numbers.push_back(number);
            if (isValid(node, coord[0], coord[1], number)) 
            {
                child.sudoku_matrix[coord[0]][coord[1]] = number;
                child.valid_numbers = node.valid_numbers;
                auto itr = find(child.valid_numbers.begin(), child.valid_numbers.end(), number);
                // child.valid_numbers.erase(itr);
                // child.valid_numbers.erase(child.valid_numbers.begin() + (2));
                stack.push(child);
                child.sudoku_matrix[coord[0]][coord[1]] = 46;
                // child.valid_numbers.insert(itr, number);
            }
        }
        // clear pointer to coordinates
        delete[] coord;
    }
    node.solved = false;
    return node;
}

int main(int argc, char** argv) {
    // sanity check for program inputs
    if(argc < 3) {
        cerr << "Error! Not enough arguments to run program.\n";
        return -1;
    } 

    // persistent variables through reading declarations
    ifstream entry;
    string current_line;
    State initial;
    Solver* sol;
    int line_number = 0;

    // check which method is the chosen one to solve the sudoku(s)
    if(strcmp(argv[1], "bfs") == 0)
        sol = new BFS();
    else if(strcmp(argv[1], "dfs") == 0)
        sol = new DFS();
    else if(strcmp(argv[1], "a*") == 0 || strcmp(argv[1], "astar") == 0)
        sol = new AStar();
    else if(strcmp(argv[1], "backtracking") == 0 || strcmp(argv[1], "back") == 0)
        sol = new BACK();
    else if(strcmp(argv[1], "ac3") == 0)
        sol = new AC3();
    // if no known method is specified, return error message and close program
    else {
        cerr << "Error! Unknown option to solve sudoku.\n";
        return -1;
    }
    
    // open file in binary mode so that not too much time is spent in
    // file reading
    entry.open(argv[2], ios::in | ios::binary);
    if (entry.is_open()){
        //read file line by line and input each character in the sudoku matrix
        while(getline(entry, current_line)){
            auto start = high_resolution_clock::now();
            line_number++;
            int z = 0;
            for(int i = 0; i < LEN; i++){
                for(int j = 0; j < LEN; ++j){
                    initial.sudoku_matrix[i][j] = current_line[z];
                    z++;
                }
            }

            // try to find a solution to the given sudoku
            State solved = sol->solve(initial);

            // if solution is found, print matrix on screen
            if(solved.solved)
                sol->printSudoku(solved.sudoku_matrix);
            // if not, print message
            else
                printf("Sorry, could not find solution for given sudoku at line %d\n", line_number);

            auto stop = high_resolution_clock::now(); 
            auto duration = duration_cast<microseconds>(stop - start); 
            cout << "Linha: " << line_number << " leva: " << (double) duration.count()/1000000 << " seconds." << endl;
        }

        // once whole file is read, close it
        entry.close();
    }

    // if could not open file, print error message and close program
    else{
        fprintf(stderr, "Unable to open requested file. %s\n", strerror(errno));
        delete sol;
        return -1;
    }
    delete sol;
    return 0;
}