// CIS-479_RETAKE-P1-MEECH.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

//Author: Demetrius E Johnson
//Purpose: Implement the A* Search and Iterative Deepening Search (IDS) algorithms
//Date Created: 7/15/22
//Date Modified: 

//CIS-479: Windy Maze Problem (North wind)

/*  [] = blank tile, #= wall obstacle, 0 = initial state, G = goal state


    []      []     []     []     []    

    []      #       #     []     []

    []      #       G     []     []

    0       #       #     []     []

    []      #       []    []     []

    []      []      []    []     []

    Move North (against wind) cost = 3.
    Move South (with wind) cost = 1.
    Move East or West (side wind) cost = 2.

    //special notes: each valid location is a state.
    

*/

#include <iostream>
#include "FIFO.h"
#include "StateNode.h"
#include "minHeapPQ.h"
#include "minHeapPQ.cpp"
#include "QuadraticProbing.h"
#include <cstdlib>
#include <iomanip> //so we can set number of digits output for integer in the output table
using namespace std;

////////////////////////FUNCTION DECLARATIONS//////////////////////
int getValueLocation_row(StateNode& state, int value);
int getValueLocation_col(StateNode& state, int value);
int calcHeuristic(const StateNode& goal_state, const StateNode& curr_state);
void output_maze_table(vector<vector<int>>& table, const int& numRows, const int& numCols);
int expandNode_WNES_order(vector<vector<int>>& table, StateNode& parentNode);
void output_expansionOrder(vector<vector<int>> table_copy);
void output_traced_solution(vector<vector<int>> table_copy, StateNode& terminal);
bool A_Star_search(vector<vector<int>>& table, StateNode& solutionNode);
void welcome_menu(void);
//////////////GLOBAL VARIABLES//////////////
int initialize_hashTableSize = 300; //use this to set hash table size in the event that solution is found but cannot be traced due to rehash() function needing to be called and making parent pointers bad
minHeapPQ<StateNode> frontierSet_PQ;  //Frontier Set: all leaf nodes available for expansion at any given point (in time / during traversal).
HashTable<StateNode> exploreSet_HashTable(initialize_hashTableSize);   //Use explored set to remember every expanded node to avoid redundant paths.
HashTable<FIFO> FIFO_tie_breaker_HashTable;   //use this hash table to maintain FIFO order among nodes with same EvalFunction magnitude
StateNode stateGoal;
StateNode stateStart;
int searchLoop_Limitation;
int maze_Rows = 6;
int maze_Cols = 5;
int init_row_location = 3;
int init_col_location = 0;
int goal_row_location = 2;
int goal_col_location = 2;
vector<StateNode> expansionOrder_vector; //track expansion order
//////////////MAIN FUNCTION///////////////////
int main()
{
    welcome_menu();
    

    /////////////A* Search//////////////////////////
    //initialize windy maze (North Wind)
    /*  [] = blank tile, #= wall obstacle, i = initial state, G = goal state


    []  []  []  []  []

    []  #   #   []  []

    []  #   G   []  []

    i   #   #   []  []

    []  #   []  []  []

    []  []  []  []  []
*/

    //initiate 2D array table using vectors
    vector<vector<int>>windy_maze_array;
    windy_maze_array.resize(maze_Rows);
    for (int i = 0; i < windy_maze_array.size(); i++)
        windy_maze_array[i].resize(maze_Cols);

    //initialize all locations to blank tile locations, since majority are blank = 0:
    for (int i = 0; i < maze_Rows; i++)
        for (int j = 0; j < maze_Cols; j++)
            windy_maze_array[i][j] = 0;

    //initialize wall obstacle locations = INT_MIN
    windy_maze_array[1][1] = INT_MIN;
    windy_maze_array[2][1] = INT_MIN;
    windy_maze_array[3][1] = INT_MIN;
    windy_maze_array[4][1] = INT_MIN;
    windy_maze_array[1][2] = INT_MIN;
    windy_maze_array[3][2] = INT_MIN;

    //initialize goal location = INT_MAX
    windy_maze_array[2][2] = INT_MAX;

    //initialize start location = -1
    windy_maze_array[init_row_location][init_col_location] = -1;
    //output maze table to ensure it was built properly
    cout << "\nNorth Wind. Movement costs: W and E side wind = 2; N against wind = 3, S with wind = 1.\n\n";
    output_maze_table(windy_maze_array, maze_Rows, maze_Cols);


    //Initialize the start and goal states; first their location; then their heuristic values, pathcost, and eval fucntion

    stateStart.row_location = init_row_location;
    stateStart.col_location = init_col_location;

    stateGoal.row_location = goal_row_location;
    stateGoal.col_location = goal_row_location;

    stateStart.heuristicVal = calcHeuristic(stateGoal, stateStart);
    stateGoal.heuristicVal = calcHeuristic(stateGoal, stateGoal);


    //initialize starting path cost for initial state (the root) and set evaluation function value.
    stateStart.pathCost = 0;
    stateStart.setEval_value();

    //insert initial state as first node in the frontier set (initialize frontier); also add to FIFO_tie_breaker tracking array
    FIFO_tie_breaker_HashTable.insert(stateStart.tie_breaker);
    frontierSet_PQ.insert(stateStart);

    //set loop search limitation: allows for controlling and being notifiedof  how large of a solution you want to find or if there is some loop that occurs
    searchLoop_Limitation = 2500;

    //ready to search:
    StateNode solutionNode;
    bool solution_found = false;
    solution_found = A_Star_search(windy_maze_array, solutionNode);

    //print expansion order and solution trace:
    if (solution_found) {
        output_expansionOrder(windy_maze_array);
        output_traced_solution(windy_maze_array, solutionNode);
    }
    else
        cout << "\n\n~NO SOLUTION FOUND. POSSIBLE ERROR, POSSIBLE Search Depth Limitation or no more nodes to search.~\n\n";

    cout << "\nProgram Completed. Thank you. Exiting 0.\n";
    system("pause");
    return 0;
}


//FUNCTION DEFINITIONS//
//////////////////////////////////////
enum PathCostDirection { WEST = 2, NORTH = 3, EAST = 2, SOUTH = 1 };

int calcHeuristic(const StateNode& goal_state, const StateNode& curr_state) {


    int heuristic = 0;


    //cout << "\ninitial state: (" << curr_state.row_location << ", " << curr_state.col_location << ")\n";
    //cout << "\goal state: (" << goal_state.row_location << ", " << goal_state.col_location << ")\n";


    //calculate heuristic (Manhattan distance) for the state = difference in row location + difference in col location

    //if goal row is LESS than current row, then we need to move N (into the N wind) = cost of 3 per northward step:
    if ((goal_state.row_location - curr_state.row_location) < 0)
        heuristic += NORTH * abs(goal_state.row_location - curr_state.row_location);

    //else if goal row is GREATER than current row, then we need to move S (with the N wind) = cost of 1 per southward step:
    if ((goal_state.row_location - curr_state.row_location) > 0)
        heuristic += SOUTH * abs(goal_state.row_location - curr_state.row_location);

    //finally, add the column component of the heuristic value; for W and E, we have a side wind with cost = 2:
    heuristic += 2 * abs(goal_state.col_location - curr_state.col_location); 

    return heuristic;

}
void output_maze_table(vector<vector<int>>& table, const int& numRows, const int& numCols) {
    for (int i = 0; i < numRows; i++) {
        cout << "\t";
        for (int j = 0; j < numCols; j++) {
                
            if (table[i][j] == INT_MIN)
                cout << "#\t";

            else if (table[i][j] == INT_MAX)
                cout << "G\t";

            else if (i == init_row_location && j == init_col_location)
                cout << "i\t";
            else
                cout << setw(2) << setfill('0') << table[i][j] << '\t';
            
        }
        cout << endl << endl;
    }
}
int expandNode_WNES_order(vector<vector<int>>& table, StateNode& parentNode) {

    StateNode childNode;
    int numChildren_generated = 0;

    /* below is instruction of how to expand in each direction: W, N, E, S

        *expand to WEST  -->  j - 1 (j >= 1) && j-1 != internal obstacle
        *expand to NORTH -->  i - 1 (i >= 1) && i-1 != internal obstacle
        *expand to EAST  -->  j + 1 (j < numCols - 1 ) && j+1 != internal obstacle
        *expand to SOUTH -->  i + 1 (i < numRows - 1) && i+1 != internal obstacle

    */

    //case: check WEST of parent (expansion node) location and ensure there is no wall or obstacle
    if ( (parentNode.col_location >= 1) && (table[parentNode.row_location][parentNode.col_location - 1] != INT_MIN) ) {

        childNode = parentNode;
        childNode.col_location = parentNode.col_location - 1; //child node is WEST of parent node

              //**next 7 lines**:
        //calculate and set child heuristic value
        //set g(n) == pathcost == parent cost + W move cost
        //g(n) and h(n) set; now set f(n) = g(n) + f(n).
        //hash and track parent --> hash parent in explore set hash table so we know that it has been explored, and set child node to point to that hashed parent.
        //set hash_string for newly created child node so that it can later be hashed when it is explored
        //check to see if a node has already been added with the childNode's EvalFunction value; increment insert order value if so to maintain FIFO (see < and > functions for StateNode)
        //add child to frontier set (priority queue)
        childNode.heuristicVal = calcHeuristic(stateGoal, childNode);
        childNode.pathCost = parentNode.pathCost + WEST;
        childNode.setEval_value();
        childNode.parent = exploreSet_HashTable.getOBJ_reference(parentNode);
        childNode.setHash_string();
        if (FIFO_tie_breaker_HashTable.contains(childNode.tie_breaker)) {
            FIFO_tie_breaker_HashTable.getOBJ_reference(childNode.tie_breaker)->insert_order_val++; //increase order
            childNode.tie_breaker.insert_order_val = FIFO_tie_breaker_HashTable.getOBJ_reference(childNode.tie_breaker)->insert_order_val; //set child node to that new increased order
        }
        else FIFO_tie_breaker_HashTable.insert(childNode.tie_breaker);
        frontierSet_PQ.insert(childNode);
        numChildren_generated++;
    }
    //case: check NORTH of parent (expansion node) location and ensure there is no wall or obstacle
    if ((parentNode.row_location >= 1) && (table[parentNode.row_location - 1][parentNode.col_location] != INT_MIN)) {

        childNode = parentNode;
        childNode.row_location = parentNode.row_location - 1; //child node is NORTH of parent node

              //**next 7 lines**:
        //calculate and set child heuristic value
        //set g(n) == pathcost == parent cost + N move cost
        //g(n) and h(n) set; now set f(n) = g(n) + f(n).
        //hash and track parent --> hash parent in explore set hash table so we know that it has been explored, and set child node to point to that hashed parent.
        //set hash_string for newly created child node so that it can later be hashed when it is explored
        //check to see if a node has already been added with the childNode's EvalFunction value; increment insert order value if so to maintain FIFO (see < and > functions for StateNode)
        //add child to frontier set (priority queue)
        childNode.heuristicVal = calcHeuristic(stateGoal, childNode);
        childNode.pathCost = parentNode.pathCost + NORTH;
        childNode.setEval_value();
        childNode.parent = exploreSet_HashTable.getOBJ_reference(parentNode);
        childNode.setHash_string();
        if (FIFO_tie_breaker_HashTable.contains(childNode.tie_breaker)) {
            FIFO_tie_breaker_HashTable.getOBJ_reference(childNode.tie_breaker)->insert_order_val++; //increase order
            childNode.tie_breaker.insert_order_val = FIFO_tie_breaker_HashTable.getOBJ_reference(childNode.tie_breaker)->insert_order_val; //set child node to that new increased order
        }
        else FIFO_tie_breaker_HashTable.insert(childNode.tie_breaker);
        frontierSet_PQ.insert(childNode);
        numChildren_generated++;
    }
    //case: check EAST of parent (expansion node) location and ensure there is no wall or obstacle
    if ((parentNode.col_location < (maze_Cols - 1)) && (table[parentNode.row_location][parentNode.col_location + 1] != INT_MIN)) {

        childNode = parentNode;
        childNode.col_location = parentNode.col_location + 1; //child node is EAST of parent node

              //**next 7 lines**:
        //calculate and set child heuristic value
        //set g(n) == pathcost == parent cost + E move cost
        //g(n) and h(n) set; now set f(n) = g(n) + f(n).
        //hash and track parent --> hash parent in explore set hash table so we know that it has been explored, and set child node to point to that hashed parent.
        //set hash_string for newly created child node so that it can later be hashed when it is explored
        //check to see if a node has already been added with the childNode's EvalFunction value; increment insert order value if so to maintain FIFO (see < and > functions for StateNode)
        //add child to frontier set (priority queue)
        childNode.heuristicVal = calcHeuristic(stateGoal, childNode);
        childNode.pathCost = parentNode.pathCost + EAST;
        childNode.setEval_value();
        childNode.parent = exploreSet_HashTable.getOBJ_reference(parentNode);
        childNode.setHash_string();
        if (FIFO_tie_breaker_HashTable.contains(childNode.tie_breaker)) {
            FIFO_tie_breaker_HashTable.getOBJ_reference(childNode.tie_breaker)->insert_order_val++; //increase order
            childNode.tie_breaker.insert_order_val = FIFO_tie_breaker_HashTable.getOBJ_reference(childNode.tie_breaker)->insert_order_val; //set child node to that new increased order
        }
        else FIFO_tie_breaker_HashTable.insert(childNode.tie_breaker);
        frontierSet_PQ.insert(childNode);
        numChildren_generated++;
    }
    //case: check SOUTH of parent (expansion node) location and ensure there is no wall or obstacle
    if ((parentNode.row_location < (maze_Rows - 1)) && (table[parentNode.row_location + 1][parentNode.col_location] != INT_MIN)) {

        childNode = parentNode;
        childNode.row_location = parentNode.row_location + 1; //child node is SOUTH of parent node

              //**next 7 lines**:
        //calculate and set child heuristic value
        //set g(n) == pathcost == parent cost + S move cost
        //g(n) and h(n) set; now set f(n) = g(n) + f(n).
        //hash and track parent --> hash parent in explore set hash table so we know that it has been explored, and set child node to point to that hashed parent.
        //set hash_string for newly created child node so that it can later be hashed when it is explored
        //check to see if a node has already been added with the childNode's EvalFunction value; increment insert order value if so to maintain FIFO (see < and > functions for StateNode)
        //add child to frontier set (priority queue)
        childNode.heuristicVal = calcHeuristic(stateGoal, childNode);
        childNode.pathCost = parentNode.pathCost + SOUTH;
        childNode.setEval_value();
        childNode.parent = exploreSet_HashTable.getOBJ_reference(parentNode);
        childNode.setHash_string();
        if (FIFO_tie_breaker_HashTable.contains(childNode.tie_breaker)) {
            FIFO_tie_breaker_HashTable.getOBJ_reference(childNode.tie_breaker)->insert_order_val++; //increase order
            childNode.tie_breaker.insert_order_val = FIFO_tie_breaker_HashTable.getOBJ_reference(childNode.tie_breaker)->insert_order_val; //set child node to that new increased order
        }
        else FIFO_tie_breaker_HashTable.insert(childNode.tie_breaker);
        frontierSet_PQ.insert(childNode);
        numChildren_generated++;
    }
    return numChildren_generated;
}
void output_expansionOrder(vector<vector<int>> table_copy){

    int expansion = 0;

    cout << "NODE EXPANSION ORDER:\n\n";
    for (int i = 0; i < expansionOrder_vector.size(); i++) {

        if (i == expansionOrder_vector.size() - 1)
            cout << "EXPANSION #" << expansion << ": **GOAL STATE!**" << endl;
        else
            cout << "EXPANSION #" << expansion << endl;

        cout << "location: (" << expansionOrder_vector[i].row_location << ", " << expansionOrder_vector[i].col_location << ")\n";
        cout << "[path cost-->g(n): " << expansionOrder_vector[i].pathCost << "]\n[heuristic-->h(n): " << expansionOrder_vector[i].heuristicVal << "]\n";
        cout << "[EvalFunc-->g(n) + h(n): " << expansionOrder_vector[i].EvalFunction << "]";

        //build a table so that we can output the solution visually when this for for the traversal loop ends
        table_copy[expansionOrder_vector[i].row_location][expansionOrder_vector[i].col_location] = expansion;

        expansion++;
        cout << endl << endl << endl;
    }

    output_maze_table(table_copy, maze_Rows, maze_Cols); //finally, output the table for visual representation of the expansion order

}
void output_traced_solution(vector<vector<int>> table_copy, StateNode& terminal) {

    StateNode* tree_traversal_ptr = exploreSet_HashTable.getOBJ_reference(terminal);
    vector<StateNode*> traversal;
    int trace = 0;

    cout << "SOLUTION TRACE:\n\n";
    if (exploreSet_HashTable.get_num_rehash_fx_called()) {

        cout << "**Solution found, but cannot print out traced solution back to root.**"
            << "\n~Rehash Fx called : shifted memory and corrupted solution trace.~"
            << "\n~Please change (increase) initialize_hashTableSize global varibale and re - run the program.~\n";
        return;
    }

    //use this while loop to traverse the solution trace, and then output it in the order from start state to goal 
    //(essentially, this loop reverses the trace since the initial data we have is from the goal state).
    while (tree_traversal_ptr != nullptr) {

        traversal.push_back(tree_traversal_ptr);
        tree_traversal_ptr = tree_traversal_ptr->parent;
    }

    for (int i = traversal.size() - 1; i >= 0; i--) {

        if (i == 0)
            cout << "TRACE #" << trace << ": **GOAL STATE!**" << endl;
        else
            cout << "TRACE #" << trace << endl;
       
        cout << "location: (" << traversal[i]->row_location << ", " << traversal[i]->col_location << ")\n";
        cout << "[path cost-->g(n): " << traversal[i]->pathCost << "]\n[heuristic-->h(n): " << traversal[i]->heuristicVal << "]\n";
        cout << "[EvalFunc-->g(n) + h(n): " << traversal[i]->EvalFunction << "]";

        //build a table so that we can output the solution visually when this for for the traversal loop ends
        table_copy[traversal[i]->row_location][traversal[i]->col_location] = trace;

        trace++;
        cout << endl << endl << endl;
    }

    output_maze_table(table_copy, maze_Rows, maze_Cols); //finally, output the table for visual representation of the trace
}
bool A_Star_search(vector<vector<int>>& table, StateNode& solutionNode) {

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
            //in West North East South order; (check west tile, then north, then east, then south)
        if (!exploreSet_HashTable.contains(parentNode)) {

            //add node to explored set: insert into hash table (if it is not already; hash function handles this)
            exploreSet_HashTable.insert(parentNode);
            //expand selected node
            expandNode_WNES_order(table, parentNode);
            //add to expand order vector so that it can be printed out
            expansionOrder_vector.push_back(parentNode);
        }
        searchLoop_Limitation--; //limit the numbder of times this loop can run in case of a very large solution search, or in case of a loop occurence

    } while (searchLoop_Limitation != 0);

    cout << "\n\n~LOOP OCCURENCE, OR SEARCH TREE TOO LARGE TO FIND A SOLUTION.~\n~CHANGE searchLoop_Limitation global variable and re-run program for deeper search.~\n\n";

    return false;

}
void welcome_menu(void) {

    cout << "Welcome to the A* Search and Iterative Deepening Search (IDS) Program. Written By Demetrius Johnson.\nFor CIS-479 at UM-Dearborn with Prof Dr. Shenquan Wang. Fall 2022.\n";
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
