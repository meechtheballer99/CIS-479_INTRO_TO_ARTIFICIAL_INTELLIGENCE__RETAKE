//Value_Iteration_BellmanEquation_meech.cpp : This file contains the 'main' function. Program execution begins and ends there.
//Author: Demetrius Johnson
//Purposse:     //Value Iteration Program using the Bellman Equation\n-for CIS-479 with Doctor Shengquan Wang
                //Program Written to demonstrate Lecutre Slide 7 - Planning and Decision Making (slides 42-50)
//STATUS: WORKS; NO KNOWN ISSUES
//Revised for HW3 Q4 (retake): 11/12/22

/*


We still assume the wind comes from the north
and the cost of one step is defined as follows (the reward will be the negation of the cost): 
1 for moving southward; 
2 for moving eastward or westward; 3 for moving northward.
The reward function R(s,a) will be the negative of the cost.
We also assume that the reward reaching at the goal is 80. 
Since the reward function R(s; a) here depends on both the state 
and the action taken at this state, all utility equations should be revised as:

U(s) ← max_a (R(s,a) + γ Σs' P(s'|s,a) U(s'))

We choose γ=1. We assume the initial utility at any state is 0 except for 80 at the goal state. 
We perform one-iteration update of the utility of all states in this order: 
a1, a2, ..., a5, b1, b2, ..., b5, c1, ..., c5, d1, d2, ..., d5. 
Please give the updated utility at each state.


*/
                
//Date Created: 08/05/22
//Date Modified: 11/12/22

#include <iostream>
#include <iomanip>
#include <vector>
#include <cstdlib>

///////DATA STRUCTURES && ENUMERATORS////////////////////////////////////////////////////

struct table {

    float utility_val[4][5];
    int init_rewardVal = 0;
    int numRows = 4;
    int numCols = 5;
    float endState_array = 80;
};

enum Direction_Reward {WEST = -2, NORTH = -3, EAST = -2, SOUTH = -1};

//////////////////GLOBAL VARIABLES//////////////////////////////////////////
table table_struct_global;
float forward_probability = 0.6;
float left_probability = 0.2;
float right_probability = 0.2;
int num_Bellman_iterations = 0;
//int endState_2 = -100;
/////////////////////FUNCTION DECLARATIONS/////////////////////////////////
void outputTable_utility(table &table_struct);
float expected_utility_WEST(int row, int col);
float expected_utility_NORTH(int row, int col);
float expected_utility_EAST(int row, int col);
float expected_utility_SOUTH(int row, int col);
void perform_BellmanUpdate(void);
void perform_BellmanUpdate_diff_order(void);
bool is_obstacle(int row, int col);
bool is_end_state(int row, int col);


int main()
{
    std::cout
        << "Welcome to the Value Iteration Program using the Bellman Equation\n-for CIS-479_RETAKE with Doctor Shengquan Wang"
        << "\n-Program Written to demonstrate answer question 4 from HW3\n-Fall 2022 - By Demetrius Johnson (meech)"
        << "\n-Gamma (discount factor) = 1";

  
    //initialize table (# = wall square = -FLT_MAX)
    /*
    
      
            j

       i    0    0    0    0    0

            0    #    #    0    0

            0    0    0    #    0

            0    0    0    0    0
    
    
    
    */
     //initialize all table values to initial utility value to make it easier...
    for (int i = 0; i < 3; i++) {
    
        for (int j = 0; j < 4; j++) {
        
            table_struct_global.utility_val[i][j] = table_struct_global.init_rewardVal;
        
        }
    }

    //now set wall value(s) to smallest possible float value (from cstdlib library):
    table_struct_global.utility_val[1][1] = -FLT_MAX;
    table_struct_global.utility_val[1][2] = -FLT_MAX;
    table_struct_global.utility_val[2][3] = -FLT_MAX;


    //now set end state locations: goal and anti-goal, +100 and -100, respectively
    table_struct_global.utility_val[2][2] = table_struct_global.endState_array;

    //output table
    std::cout << "\n\n\n";
    outputTable_utility(table_struct_global);


    //Perform Bellman Update, which calculates the utility of each state (non linear time complexity),
    //which is based on the selected initial utility value.
    //We can perform as many updates as we like and get closer and closer to the convergence value:
    
    int numUpdates = 1;
    while (numUpdates != 0) {

        //perform_BellmanUpdate(); //standard order: start at row 0 --> to last row, and for each row start at col 0 --> to last col.
        
        //use the update function that makes it is so that we start at the last row of the table instead of first row:
        perform_BellmanUpdate_diff_order();
        numUpdates--;
        num_Bellman_iterations++;
        if (numUpdates == 0) {
            std::cout << "Enter an integer to specify the number of Bellman Updates to perform (enter 0 to exit program): ";
            std::cin >> numUpdates;
        }

        //output table
        std::cout << "\n\n\n";
        outputTable_utility(table_struct_global);
    }

    return 0;
}


///////////////////////////FUNCTION DEFINITIONS/////////////////////////////////////////
void outputTable_utility(table &table_struct) {


    //print table:
    std::cout << "\n\nUTILITY TABLE - BELLMAN UPDATE ITERATION #" << num_Bellman_iterations <<":" <<"\n\n";
    for (int i = 0; i < table_struct_global.numRows; i++) {

        for (int j = 0; j < table_struct_global.numCols; j++) {

            if (is_obstacle(i, j)) { //OUTPUT WALL LOCATION

                std::cout
                    << std::right           //define alignment 
                    << std::setprecision(1) //output only 1 decimal place
                    << std::fixed           //align decimal, print with set precision
                    << std::setfill(' ')    //fill unused space with specified char
                    << std::setw(12)        //define width of output (min # of char positions a variable will consume)
                    << "[ ] ";
            }
            else {
                std::cout
                    << std::setprecision(1) //output only 1 decimal place
                    << std::fixed           //align decimal, print with set precision
                    << std::right           //define alignment 
                    << std::setw(10)         //define width of output (min # of char positions a variable will consume)
                    << std::setfill(' ')    //fill rest with chosen char
                    << table_struct.utility_val[i][j]
                    << "  ";
            }
        }
        std::cout << std::endl << std::endl << std::endl << std::endl;
    }
}

float expected_utility_WEST(int row, int col) {

    float localReward, forward_util, left_util, right_util;

    //WEST == FORWARD-->(col - 1), LEFT-->(row + 1), RIGHT-->(row - 1):

    //get local reward value from current location - based on the action taken. 
    localReward = WEST;
    //forward
        //check wall bounce back case, otherwise use (col - 1)
    if ( (col - 1) < 0 || is_obstacle(row, col - 1) )
        forward_util = forward_probability * table_struct_global.utility_val[row][col];
    else
        forward_util = forward_probability * table_struct_global.utility_val[row][col - 1];
    //left
        //check bounce back case, otherwise use (row + 1)
    if ( (row + 1) >= table_struct_global.numRows || is_obstacle(row + 1, col) )
        left_util = left_probability * table_struct_global.utility_val[row][col];
    else
        left_util = left_probability * table_struct_global.utility_val[row + 1][col];
    //right
       //check bounce back case, otherwise use (row - 1)
    if ( (row - 1) < 0 || is_obstacle(row - 1, col) )
        right_util = right_probability * table_struct_global.utility_val[row][col];
    else
        right_util = right_probability * table_struct_global.utility_val[row - 1][col];
    //now add all values to get expected value:
    return (localReward + forward_util + left_util + right_util);



}
float expected_utility_NORTH(int row, int col) {

    float localReward, forward_util, left_util, right_util;

    //NORTH == FORWARD-->(row - 1), LEFT-->(col - 1), RIGHT-->(col + 1):

     //get local reward value from current location - based on the action taken. 
    localReward = NORTH;
    //forward
        //check bounce back case, otherwise use (row - 1)
    if ( (row - 1) < 0 || is_obstacle(row - 1, col) )
        forward_util = forward_probability * table_struct_global.utility_val[row][col];
    else
        forward_util = forward_probability * table_struct_global.utility_val[row - 1][col];
    //left
        //check bounce back case, otherwise use (col - 1)
    if ( (col - 1) < 0 || is_obstacle(row, col - 1) )
        left_util = left_probability * table_struct_global.utility_val[row][col];
    else
        left_util = left_probability * table_struct_global.utility_val[row][col - 1];
    //right
       //check bounce back case, otherwise use (col + 1)
    if ( (col + 1) >= table_struct_global.numCols || is_obstacle(row, col + 1) )
        right_util = right_probability * table_struct_global.utility_val[row][col];
    else
        right_util = right_probability * table_struct_global.utility_val[row][col + 1];
    //now add all values to get expected value:
    return (localReward + forward_util + left_util + right_util);



}
float expected_utility_EAST(int row, int col) {

    float localReward, forward_util, left_util, right_util;

    //EAST == FORWARD-->(col + 1), LEFT-->(row - 1), RIGHT-->(row + 1):

  //get local reward value from current location - based on the action taken. 
    localReward = EAST;
    //forward
        //check wall bounce back case, otherwise use (col + 1)
    if ( (col + 1) >= table_struct_global.numCols || is_obstacle(row, col + 1) )
        forward_util = forward_probability * table_struct_global.utility_val[row][col];
    else
        forward_util = forward_probability * table_struct_global.utility_val[row][col + 1];
    //left
        //check bounce back case, otherwise use (row - 1)
    if ( (row - 1) < 0 || is_obstacle(row - 1, col) )
        left_util = left_probability * table_struct_global.utility_val[row][col];
    else
        left_util = left_probability * table_struct_global.utility_val[row - 1][col];
    //right
       //check bounce back case, otherwise use (row + 1)
    if ( (row + 1) >= table_struct_global.numRows || is_obstacle(row + 1, col) )
        right_util = right_probability * table_struct_global.utility_val[row][col];
    else
        right_util = right_probability * table_struct_global.utility_val[row + 1][col];
    //now add all values to get expected value:
    return (localReward + forward_util + left_util + right_util);


}
float expected_utility_SOUTH(int row, int col) {

    float localReward, forward_util, left_util, right_util;

    //SOUTH == FORWARD-->(row + 1), LEFT-->(col + 1), RIGHT-->(col - 1)

  //get local reward value from current location - based on the action taken. 
    localReward = SOUTH;
    //forward
        //check bounce back case, otherwise use (row + 1)
    if ( (row + 1) >= table_struct_global.numRows || is_obstacle(row + 1, col) )
        forward_util = forward_probability * table_struct_global.utility_val[row][col];
    else
        forward_util = forward_probability * table_struct_global.utility_val[row + 1][col];
    //left
        //check bounce back case, otherwise use (col + 1)
    if ( (col + 1) >= table_struct_global.numCols || is_obstacle(row, col + 1) )
        left_util = left_probability * table_struct_global.utility_val[row][col];
    else
        left_util = left_probability * table_struct_global.utility_val[row][col + 1];
    //right
       //check bounce back case, otherwise use (col - 1)
    if ( (col - 1) < 0 || is_obstacle(row, col - 1) )
        right_util = right_probability * table_struct_global.utility_val[row][col];
    else
        right_util = right_probability * table_struct_global.utility_val[row][col - 1];
    //now add all values to get expected value:
    return (localReward + forward_util + left_util + right_util);

}
void perform_BellmanUpdate(void) {

    for (int row = 0; row < table_struct_global.numRows; row++) {

        for (int col = 0; col < table_struct_global.numCols; col++) {

            float argMax_utility;
            std::vector<float> expectedVal;

            //skip finding utility for end states, or if at wall obstacle location
            if (is_obstacle(row, col) || is_end_state(row, col))
                continue; //continue will return to the condition at top of for-loop
          

            //Now, we must do a Bellman Update for each direction (move up or down, change row; move left or right, change col):


            //WEST == FORWARD-->(col - 1), LEFT-->(row + 1), RIGHT-->(row - 1):
            expectedVal.push_back(expected_utility_WEST(row, col));

            //NORTH == FORWARD-->(row - 1), LEFT-->(col - 1), RIGHT-->(col + 1):
            expectedVal.push_back(expected_utility_NORTH(row, col));

            //EAST == FORWARD-->(col + 1), LEFT-->(row - 1), RIGHT-->(row + 1):
            expectedVal.push_back(expected_utility_EAST(row, col));

            //SOUTH == FORWARD-->(row + 1), LEFT-->(col + 1), RIGHT-->(col - 1):
            expectedVal.push_back(expected_utility_SOUTH(row, col));

            //do comparison to find the max utility of all directions evaluated:
            argMax_utility = expectedVal[0];
            for (int i = 0; i < expectedVal.size(); i++) {

                if (argMax_utility < expectedVal[i])
                    argMax_utility = expectedVal[i];
            }

            //finally, update the current state with the new Bellman Update max utility value
            table_struct_global.utility_val[row][col] = argMax_utility;
        }
    }
}
void perform_BellmanUpdate_diff_order(void) {
    //simple change the row order: make is so that we start at the last row of the table instead of first row:
    for (int row = table_struct_global.numRows - 1; row >= 0; row--) {

        for (int col = 0; col < table_struct_global.numCols; col++) {

            float argMax_utility;
            std::vector<float> expectedVal;

            //skip finding utility for end states, or if at wall obstacle location
            if (is_obstacle(row, col) || is_end_state(row, col))
                continue; //continue will return to the condition at top of for-loop


            //Now, we must do a Bellman Update for each direction (move up or down, change row; move left or right, change col):


            //WEST == FORWARD-->(col - 1), LEFT-->(row + 1), RIGHT-->(row - 1):
            expectedVal.push_back(expected_utility_WEST(row, col));

            //NORTH == FORWARD-->(row - 1), LEFT-->(col - 1), RIGHT-->(col + 1):
            expectedVal.push_back(expected_utility_NORTH(row, col));

            //EAST == FORWARD-->(col + 1), LEFT-->(row - 1), RIGHT-->(row + 1):
            expectedVal.push_back(expected_utility_EAST(row, col));

            //SOUTH == FORWARD-->(row + 1), LEFT-->(col + 1), RIGHT-->(col - 1):
            expectedVal.push_back(expected_utility_SOUTH(row, col));

            //do comparison to find the max utility of all directions evaluated:
            argMax_utility = expectedVal[0];
            for (int i = 0; i < expectedVal.size(); i++) {

                if (argMax_utility < expectedVal[i])
                    argMax_utility = expectedVal[i];
            }

            //finally, update the current state with the new Bellman Update max utility value
            table_struct_global.utility_val[row][col] = argMax_utility;
        }
    }
}

bool is_obstacle(int row, int col) {

    if (table_struct_global.utility_val[row][col] == -FLT_MAX)
        return true;
    else
        return false;
}
bool is_end_state(int row, int col) {

    if (table_struct_global.utility_val[row][col] == table_struct_global.endState_array)
        return true;
    else
        return false;
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
