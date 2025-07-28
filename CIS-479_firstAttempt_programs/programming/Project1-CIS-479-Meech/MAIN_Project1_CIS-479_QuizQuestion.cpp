// Project1-CIS-479-Meech.cpp : This file contains the 'main' function. Program execution begins and ends there.
//Author: Demetrius E Johnson
//Purpose: Implement the A* Search algorithm
//Date Created: 7/15/22
//Date Modified: 

//CIS-479: Windy Maze puzzle 8-tile program
#include <iostream>
#include "StateNode.h"
#include "minHeapPQ.h"
#include "minHeapPQ.cpp"
#include "QuadraticProbing.h"
using namespace std;

////////////////////////FUNCTION DECLARATIONS//////////////////////
int getValueLocation_row(StateNode& state, int value);
int getValueLocation_col(StateNode& state, int value);
int calcHeuristic_full_state(StateNode& stateStart, StateNode& stateGoal);
int calcHeuristic_single_tile(StateNode& stateStart, StateNode& stateGoal, int& rowStart, int& colStart);
void calcHeuristic_based_on_parent(StateNode& parentNode, StateNode& childNode);
void swap_blankTile_nonBlankTile(StateNode& parentNode, StateNode& childNode);
void output_traced_solution(StateNode& terminal);
void output_expansionOrder(void);
int expandNode_WNES_order(StateNode& parentNode);
bool A_Star_search(StateNode& solutionNode);
void welcome_menu(void);
//////////////GLOBAL VARIABLES//////////////
int initialize_hashTableSize = 300; //use this to set hash table size in the event that solution is found but cannot be traced due to rehash() function needing to be called and making parent pointers bad
minHeapPQ<StateNode> frontierSet_PQ;  //Frontier Set: all leaf nodes available for expansion at any given point (in time / during traversal).
HashTable<StateNode> exploreSet_HashTable(initialize_hashTableSize);   //Use explored set to remember every expanded node to avoid redundant paths.
StateNode stateGoal;
StateNode stateStart;
int searchLoop_Limitation;
vector<StateNode> expansionOrder_vector; //track expansion order
//////////////MAIN FUNCTION///////////////////
int main()
{
    welcome_menu();
    //initialize start state node

    stateStart.table[0][0] = 5;
    stateStart.table[0][1] = 0;
    stateStart.table[0][2] = 1;
    stateStart.table[1][0] = 8;
    stateStart.table[1][1] = 4;
    stateStart.table[1][2] = 7;
    stateStart.table[2][0] = 2;
    stateStart.table[2][1] = 6;
    stateStart.table[2][2] = 3;

    //track blank tile location, represented by the integer value 0
    stateStart.blankTile_row_location = 0;
    stateStart.blankTile_col_location = 1;

    //initialize goal state node

    stateGoal.table[0][0] = 1;
    stateGoal.table[0][1] = 2;
    stateGoal.table[0][2] = 3;
    stateGoal.table[1][0] = 8;
    stateGoal.table[1][1] = 0;
    stateGoal.table[1][2] = 4;
    stateGoal.table[2][0] = 7;
    stateGoal.table[2][1] = 6;
    stateGoal.table[2][2] = 5;

    //Initialize the first heuristic value for the initial/start state node and the goal state heuristic value.
    //From here, we can avoid recalculating every single child 
    //from the root node by simply using the parent nodes and the single-tile heurstic function.
    stateStart.heuristicVal = calcHeuristic_full_state(stateStart, stateGoal);
    stateGoal.heuristicVal = 0;
    
    //initialize starting path cost for initial state (the root) and set evaluation function value.
    stateStart.pathCost = 0;
    stateStart.setEval_value();

    //insert initial state as first node in the frontier set (initialize frontier)
    frontierSet_PQ.insert(stateStart);

    //set loop search limitation: allows for controlling and being notifiedof  how large of a solution you want to find or if there is some loop that occurs
    searchLoop_Limitation = 2500;

    //ready to search:
    StateNode solutionNode;
    bool solution_found = false;
    solution_found = A_Star_search(solutionNode);


    //print expansion order and solution trace:
    if (solution_found) {
        output_expansionOrder();
        output_traced_solution(solutionNode);
    }
    else
        cout << "\n\n~NO SOLUTION FOUND. POSSIBLE ERROR, POSSIBLE Search Depth Limitation or no more nodes to search.~\n\n";

    cout << "\nProgram Completed. Thank you. Exiting 0.\n";
    system("pause");
    return 0;
}


//FUNCTION DEFINITIONS//
//////////////////////////////////////
enum PathCostDirection {WEST = 2, NORTH = 3, EAST = 2, SOUTH = 1};
int getValueLocation_row(StateNode& state, int value) {

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {

            if (state.table[i][j] == value)
                return i;
        }
    }

}
int getValueLocation_col(StateNode& state, int value) {

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {

            if (state.table[i][j] == value)
                return j;
        }
    }

}
int calcHeuristic_full_state(StateNode& stateStart, StateNode& stateGoal) {


    int heuristic = 0;

    cout << "\ninitial state:\n";
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {


            cout << stateStart.table[i][j];
            cout << " ";
        }
        cout << endl;
    }

    cout << "\ngoal state:\n";
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {


            cout << stateGoal.table[i][j];
            cout << " ";
        }
        cout << endl;
    }

    //calculate heuristic for entire state
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {

            heuristic += calcHeuristic_single_tile(stateStart, stateGoal, i, j);
        }
    }
    cout << endl << endl;

    return heuristic;

}
int calcHeuristic_single_tile(StateNode& stateStart, StateNode& stateGoal, int& rowStart, int& colStart) {

    int heuristic = 0;


    if (stateStart.table[rowStart][colStart] != 0) { //0 is used to represent blank tile location

        //get row and col locations in goal table for the current value at location (rowStart, colStart) of the start state
        int rowGoal = getValueLocation_row(stateGoal, stateStart.table[rowStart][colStart]);
        int colGoal = getValueLocation_col(stateGoal, stateStart.table[rowStart][colStart]);
        //get difference between goal and start state
        int rowDifference = rowGoal - rowStart;
        int colDifference = colGoal - colStart;

        //positive difference: move down (south, with the north wind);
        //negative difference: move up (north, against/into the north wind). 
        if (rowDifference < 0) {

            rowDifference *= -1;                //need positive cost values
            heuristic += (rowDifference * 1);   //each move northward = +3 cost
        }
        else {

            heuristic += (rowDifference * 1);   //each move southward = +1 cost
        }

        //make sure col difference is always positive since east and west movement is always +2 for wind cost
        if (colDifference < 0) { colDifference *= -1; }
        heuristic += (colDifference * 1);               //each west or eastward movement == +2 wind cost
    }

    return heuristic;
}
void calcHeuristic_based_on_parent(StateNode& parentNode, StateNode& childNode) {

    //save time: calculate child heuristic value based on heuristic value of parent
      //formula is:      (parent's full h value)
      //               - (h value of single location where child swapped in blank tile) 
      //               + (h value of single location where child swapped in non-blank tile)
    childNode.heuristicVal =
        (
            (parentNode.heuristicVal)
            -
            (calcHeuristic_single_tile(parentNode, stateGoal, childNode.blankTile_row_location, childNode.blankTile_col_location))
            +
            (calcHeuristic_single_tile(childNode, stateGoal, parentNode.blankTile_row_location, parentNode.blankTile_col_location))
            );
}
void swap_blankTile_nonBlankTile(StateNode& parentNode, StateNode& childNode) {
    int swap;
    swap = childNode.table[childNode.blankTile_row_location][childNode.blankTile_col_location];    //get value that is in the location where the blank tile will go
    childNode.table[parentNode.blankTile_row_location][parentNode.blankTile_col_location] = swap;  //perform swap
    childNode.table[childNode.blankTile_row_location][childNode.blankTile_col_location] = 0;       //finish swap: officially set child node blank tile location to the blank value == 0



}
void output_traced_solution(StateNode& terminal) {

    StateNode* tree_traversal_ptr = exploreSet_HashTable.getOBJ_reference(terminal);
    vector<StateNode*> traversal;
    int trace = 1;

    cout << "SOLUTION TRACE:\n\n";
    if (exploreSet_HashTable.get_num_rehash_fx_called()) {
    
        cout << "**Solution found, but cannot print out traced solution back to root.**"
             << "\n~Rehash Fx called : shifted memory and corrupted solution trace.~"
             << "\n~Please change (increase) initialize_hashTableSize global varibale and re - run the program.~\n";
        return;
    }

    while (tree_traversal_ptr != nullptr){

        traversal.push_back(tree_traversal_ptr);
        tree_traversal_ptr = tree_traversal_ptr->parent;
    }

    for (int i = traversal.size() - 1; i >= 0; i--) {

        if (i == 0)
            cout << "TRACE #" << trace << ": **GOAL STATE!**" << endl;
        else
            cout << "TRACE #" << trace << endl;
        for (int row = 0; row < 3; row++) {
            for (int col = 0; col < 3; col++) {
                cout << traversal[i]->table[row][col] << " ";
            }
            cout << endl;
        }
        cout << "[path cost-->g(n): " << traversal[i]->pathCost << "]\n[heuristic-->h(n): " << traversal[i]->heuristicVal << "]\n";
        cout << "[EvalFunc-->g(n) + h(n): " << traversal[i]->EvalFunction << "]";
        trace++;
        cout << endl << endl << endl;
    }
}
void output_expansionOrder(void) {

    int expansion = 1;

    cout << "NODE EXPANSION ORDER:\n\n";
    for (int i = 0; i < expansionOrder_vector.size(); i++) {

        if (i == expansionOrder_vector.size() - 1)
            cout << "EXPANSION #" << expansion << ": **GOAL STATE!**" << endl;
        else
            cout << "EXPANSION #" << expansion << endl;

        for (int row = 0; row < 3; row++) {
            for (int col = 0; col < 3; col++) {
                cout << expansionOrder_vector[i].table[row][col] << " ";
            }
            cout << endl;
        }
        cout << "[path cost-->g(n): " << expansionOrder_vector[i].pathCost << "]\n[heuristic-->h(n): " << expansionOrder_vector[i].heuristicVal << "]\n";
        cout << "[EvalFunc-->g(n) + h(n): " << expansionOrder_vector[i].EvalFunction << "]";
        expansion++;
        cout << endl << endl << endl;
    }
}
int expandNode_WNES_order(StateNode& parentNode) {

    StateNode childNode;
    int numChildren_generated = 0;

    /* below is an example with instruction and visualization of how to move the blank tile W, N, E, S
        
        *move blank tile WEST  -->  j - 1 (j >= 1)
        *move blank tile NORTH -->  i - 1 (i >= 1)
        *move blank tile EAST  -->  j + 1 (j <=1 )
        *move blank tile SOUTH -->  i + 1 (i <= 1)

           j  c0  c1   c2
       i
     row 0    #   #    #
     row 1    #   -    #
     row 2    #   #    #
   
    */

    //case: check WEST of blank tile; swap blank tile to that location in a newly created child node
    if (parentNode.blankTile_col_location >= 1) {

        childNode = parentNode;
        childNode.blankTile_col_location = parentNode.blankTile_col_location - 1; //move blank tile WEST == move non-blank tile EAST

              //**next 6 lines**:
        //perform swap.
        //save time: calculate child heuristic value based on heuristic value of parent.
        //set g(n) == pathcost (based on non-blank tile): EASTWARD move of nonblank tile (==WESTWARD move of blank tile) = cost of 2.
        //g(n) and h(n) set; now set f(n) = g(n) + f(n).
        //track parent.
        //set new hash string now that all tiles are in correct location
        //add child to frontier set (priority queue)
        swap_blankTile_nonBlankTile(parentNode, childNode);
        calcHeuristic_based_on_parent(parentNode, childNode);
        childNode.pathCost = parentNode.pathCost + EAST;
        childNode.setEval_value();
        childNode.parent = exploreSet_HashTable.getOBJ_reference(parentNode);
        childNode.setHash_string();
        frontierSet_PQ.insert(childNode);
        numChildren_generated++;
    }
    //case: check NORTH of blank tile; swap blank tile to that location in a newly created child node
    if (parentNode.blankTile_row_location >= 1) {

        childNode = parentNode;
        childNode.blankTile_row_location = parentNode.blankTile_row_location - 1;  //move blank tile NORTH == move non-blank tile SOUTH

             //**next 6 lines**:
        //perform swap.
        //save time: calculate child heuristic value based on heuristic value of parent.
        //set g(n) == pathcost (based on non-blank tile): SOUTHWARD move of non-blank tile (with the North wind) (==NORTHWARD move of blank tile) = cost of 1.
        //g(n) and h(n) set; now set f(n) = g(n) + f(n).
        //track parent.
        //set new hash string now that all tiles are in correct location
        //add child to frontier set (priority queue)
        swap_blankTile_nonBlankTile(parentNode, childNode);
        calcHeuristic_based_on_parent(parentNode, childNode);
        childNode.pathCost = parentNode.pathCost + SOUTH;
        childNode.setEval_value();
        childNode.parent = exploreSet_HashTable.getOBJ_reference(parentNode);
        childNode.setHash_string(); 
        frontierSet_PQ.insert(childNode);
        numChildren_generated++;
    }
    //case: check EAST of blank tile; swap blank tile to that location in a newly created child node
    if (parentNode.blankTile_col_location <= 1) {

        childNode = parentNode;
        childNode.blankTile_col_location = parentNode.blankTile_col_location + 1; //move blank tile EAST == move non-blank tile WEST

              //**next 6 lines**:
        //perform swap.
        //save time: calculate child heuristic value based on heuristic value of parent.
        //set g(n) == pathcost (based on non-blank tile): WESTWARD move of nonblank tile (==EASTWARD move of blank tile) = cost of 2.
        //g(n) and h(n) set; now set f(n) = g(n) + f(n).
        //track parent.
        //set new hash string now that all tiles are in correct location
        //add child to frontier set (priority queue)
        swap_blankTile_nonBlankTile(parentNode, childNode);
        calcHeuristic_based_on_parent(parentNode, childNode);
        childNode.pathCost = parentNode.pathCost + WEST;
        childNode.setEval_value();
        childNode.parent = exploreSet_HashTable.getOBJ_reference(parentNode);
        childNode.setHash_string(); 
        frontierSet_PQ.insert(childNode);
        numChildren_generated++;
    }
    //case: check SOUTH of blank tile; swap blank tile to that location in a newly created child node
    if (parentNode.blankTile_row_location <= 1) {

        childNode = parentNode;
        childNode.blankTile_row_location = parentNode.blankTile_row_location + 1;  //move blank tile NORTH == move non-blank tile SOUTH

             //**next 6 lines**:
        //perform swap.
        //save time: calculate child heuristic value based on heuristic value of parent.
        //set g(n) == pathcost (based on non-blank tile): NORTHWARD move of non-blank tile (against the North wind) (==NORTHWARD move of blank tile) = cost of 3.
        //g(n) and h(n) set; now set f(n) = g(n) + f(n).
        //track parent.
        //set new hash string now that all tiles are in correct location
        //add child to frontier set (priority queue)
        swap_blankTile_nonBlankTile(parentNode, childNode);
        calcHeuristic_based_on_parent(parentNode, childNode);
        childNode.pathCost = parentNode.pathCost + NORTH;
        childNode.setEval_value();
        childNode.parent = exploreSet_HashTable.getOBJ_reference(parentNode);
        childNode.setHash_string(); 
        frontierSet_PQ.insert(childNode);
        numChildren_generated++;
    }
    return numChildren_generated;
}
bool A_Star_search(StateNode& solutionNode){

    StateNode parentNode;
    do {
        if (frontierSet_PQ.isEmpty()) { return false; } //fail case: no more nodes to expand; solution not found

   
        //else pop a node from the FIFO min heap Priority Queue for possible expansion
        parentNode = frontierSet_PQ.deleteMin(); //FIFO min heap priority queue will pop element with lowest EvalFunction value and do FIFO for tie breakers
        parentNode.setHash_string(); //set hash string

       //check to see if popped node == goal state
        if (parentNode == stateGoal) { 
            
            solutionNode = parentNode; //this allows the reference passed into have the terminal/solution node so that it can be traced back to the root == get solution path. 
            exploreSet_HashTable.insert(solutionNode);//add to explore set (even though solution found, this is necessary for the trace_solution function)
            expansionOrder_vector.push_back(parentNode); //add to expansion order so final/solution node can be printed out in another function
            return true;
        }

        //IF the popped node is not in the frontier OR explored set; FIFO priority queue and quadratic probing hash table controls which node is selected for expansion:
            //THEN expand all possible children of the popped node
            //in West North East South order; (check west non-blank tile, then north, then east, then south)
        if (!exploreSet_HashTable.contains(parentNode)) {

            //add node to explored set: insert into hash table (if it is not already; hash function handles this)
            exploreSet_HashTable.insert(parentNode);
            //expand selected node
            expandNode_WNES_order(parentNode);
            //add to expand order vector so that it can be printed out
            expansionOrder_vector.push_back(parentNode); 
        }
        searchLoop_Limitation--; //limit the numbder of times this loop can run in case of a very large solution search, or in case of a loop occurence

    } while (searchLoop_Limitation != 0);

    cout << "\n\n~LOOP OCCURENCE, OR SEARCH TREE TOO LARGE TO FIND A SOLUTION.~\n~CHANGE searchLoop_Limitation global variable and re-run program for deeper search.~\n\n";

    return false;
    
}
void welcome_menu(void) {

    cout << "Welcome to the A * Search Program. Written By Demetrius Johnson. Completed 7-21-22.\nFor CIS-479 at UM-Dearborn with Prof Dr. Shenquan Wang. Summer II.\n";
    cout << "First, the expansion order will be output. Then, a trace of the solution will be output. If no solution, output will notify.\n";
}


// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
