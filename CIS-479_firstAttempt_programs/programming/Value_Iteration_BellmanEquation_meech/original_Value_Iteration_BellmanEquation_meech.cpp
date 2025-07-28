//Value_Iteration_BellmanEquation_meech.cpp : This file contains the 'main' function. Program execution begins and ends there.
//Author: Demetrius Johnson
//Purposse:     //Value Iteration Program using the Bellman Equation\n-for CIS-479 with Doctor Shengquan Wang
                //Program Written to demonstrate Lecutre Slide 7 - Planning and Decision Making (slides 42-50)
                
//Date Created: 7/28/22
//Date Modified: 7/28/22

#include <iostream>
#include <iomanip>
#include <vector>

///////DATA STRUCTURES && ENUMERATORS////////////////////////////////////////////////////

struct table {

    int tablePos_reward[3][4];
    float utility_val[3][4];
    int rewardVal = -3;
    int selected_utility_val = 0;
};

enum class Direction { NO_DIREC = 0, FORWARD = 1, LEFT = 2, RIGHT = 3 };
enum class Direction_Cardinal { WEST = 1, NORTH = 2, EAST = 3, SOUTH = 4 };

//////////////////GLOBAL VARIABLES//////////////////////////////////////////
table table_struct_global;
float forward_probability = 0.8;
float left_probability = 0.1;
float right_probability = 0.1;
int num_Bellman_iterations = 0;
int endState_1 = 100;
int endState_2 = -100;
/////////////////////FUNCTION DECLARATIONS/////////////////////////////////
void outputTable_reward(table &table_struct);
void outputTable_utility(table &table_struct);
float expected_utility_WEST(int row, int col);
float expected_utility_NORTH(int row, int col);
float expected_utility_EAST(int row, int col);
float expected_utility_SOUTH(int row, int col);
void perform_BellmanUpdate(void);

int main()
{
    std::cout
        << "Welcome to the Value Iteration Program using the Bellman Equation\n-for CIS-479 with Doctor Shengquan Wang"
        << "\n-Program Written to demonstrate Lecutre Slide 7 - Planning and Decision Making (slides 42-50)\n-Summer II 2022 - By Demetrius Johnson (meech)"
        << "\n-Gamma (discount facotr) = 1";
    
    
    //initialize utility values:
    for (int i = 0; i < 3; i++) {

        for (int j = 0; j < 4; j++) {

            table_struct_global.utility_val[i][j] = 0;

        }
    }
    //now set end state locations: goal and anti-goal, +100 and -100, respectively
    table_struct_global.utility_val[0][3] = endState_1;
    table_struct_global.utility_val[1][3] = endState_2;

  
    //initialize table (0 = wall square)
    /*
    i.e.: reward value = -3.
      
            j

       i   -3   -3   -3   +100

           -3    0   -3   -100

           -3 -  3 -  3    -3
    
    
    
    */
     //initialize all table values to reward value to make it easier...
    for (int i = 0; i < 3; i++) {
    
        for (int j = 0; j < 4; j++) {
        
            table_struct_global.tablePos_reward[i][j] = table_struct_global.rewardVal;
        
        }
    }

    //now set wall value(s):
    table_struct_global.tablePos_reward[1][1] = 0;

    //now set end state locations: goal and anti-goal, +100 and -100, respectively
    table_struct_global.tablePos_reward[0][3] =  endState_1;
    table_struct_global.tablePos_reward[1][3] = endState_2;

    //output table
    std::cout << "\n\n\n";
    outputTable_reward(table_struct_global);
    outputTable_utility(table_struct_global);


    //Perform Bellman Update, which calculates the utility of each state (non linear time complexity),
    //which is based on the selected initial utility value.
    //We can perform as many updates as we like and get closer and closer to the convergence value:
    
    int numUpdates = 1;
    while (numUpdates != 0) {

        perform_BellmanUpdate();
        numUpdates--;
        num_Bellman_iterations++;
        if (numUpdates == 0) {
            std::cout << "Enter an integer to specify the number of Bellman Updates to perform (enter 0 to exit program): ";
            std::cin >> numUpdates;
        }

        //output table
        std::cout << "\n\n\n";
        outputTable_reward(table_struct_global);
        outputTable_utility(table_struct_global);
    }

    return 0;
}


///////////////////////////FUNCTION DEFINITIONS/////////////////////////////////////////
void outputTable_reward(table &table_struct) {


    //print table:
    std::cout << "\n\nREWARD TABLE:\n\n";
    for (int i = 0; i < 3; i++) {

        for (int j = 0; j < 4; j++) {

            if (i == 1 && j == 1) { //OUTPUT WALL LOCATION

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
                    << table_struct.tablePos_reward[i][j]
                    << "  ";
            }
        }
        std::cout << std::endl << std::endl << std::endl << std::endl;
    }
}
void outputTable_utility(table& table_struct) {


    //print utility table:
    std::cout << "\n\nCONVERGING UTILITY TABLE [BELLMAN UPDATE ITERATION #" << num_Bellman_iterations << "]:\n\n";
    for (int i = 0; i < 3; i++) {

        for (int j = 0; j < 4; j++) {

            if (i == 1 && j == 1) { //OUTPUT WALL LOCATION
            
                std::cout
                    << std::right           //define alignment 
                    << std::setprecision(1) //output only 1 decimal place
                    << std::fixed           //align decimal, print with set precision
                    << std::setfill(' ')    //fill unused space with specified char
                    << std::setw(12)        //define width of output (min # of char positions a variable will consume)
                    << "[ ]  ";
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

    //get local reward value from current location
    localReward = table_struct_global.tablePos_reward[row][col];
    //forward
        //check wall bounce back case, otherwise use (col - 1)
    if ( (col - 1) < 0 || ( row == 1 && (col - 1) == 1 ) )
        forward_util = forward_probability * table_struct_global.utility_val[row][col];
    else
        forward_util = forward_probability * table_struct_global.utility_val[row][col - 1];
    //left
        //check bounce back case, otherwise use (row + 1)
    if ( (row + 1) > 2 || ( (row + 1) == 1 && col == 1 ) )
        left_util = left_probability * table_struct_global.utility_val[row][col];
    else
        left_util = left_probability * table_struct_global.utility_val[row + 1][col];
    //right
       //check bounce back case, otherwise use (row - 1)
    if ( (row - 1) < 0 || ( (row - 1) == 1 && col == 1 ) )
        right_util = right_probability * table_struct_global.utility_val[row][col];
    else
        right_util = right_probability * table_struct_global.utility_val[row - 1][col];
    //now add all values to get expected value:
    return (localReward + forward_util + left_util + right_util);



}
float expected_utility_NORTH(int row, int col) {

    float localReward, forward_util, left_util, right_util;

    //NORTH == FORWARD-->(row - 1), LEFT-->(col - 1), RIGHT-->(col + 1):

           //get local reward value from current location
    localReward = table_struct_global.tablePos_reward[row][col];
    //forward
        //check bounce back case, otherwise use (row - 1)
    if ( (row - 1) < 0 || ( (row - 1) == 1 && col == 1 ) )
        forward_util = forward_probability * table_struct_global.utility_val[row][col];
    else
        forward_util = forward_probability * table_struct_global.utility_val[row - 1][col];
    //left
        //check bounce back case, otherwise use (col - 1)
    if ( (col - 1) < 0 || ( row == 1 && (col - 1) == 1 ) )
        left_util = left_probability * table_struct_global.utility_val[row][col];
    else
        left_util = left_probability * table_struct_global.utility_val[row][col - 1];
    //right
       //check bounce back case, otherwise use (col + 1)
    if ( (col + 1) > 3 || ( row == 1 && (col + 1) == 1 ) )
        right_util = right_probability * table_struct_global.utility_val[row][col];
    else
        right_util = right_probability * table_struct_global.utility_val[row][col + 1];
    //now add all values to get expected value:
    return (localReward + forward_util + left_util + right_util);



}
float expected_utility_EAST(int row, int col) {

    float localReward, forward_util, left_util, right_util;

    //EAST == FORWARD-->(col + 1), LEFT-->(row - 1), RIGHT-->(row + 1):

            //get local reward value from current location
    localReward = table_struct_global.tablePos_reward[row][col];
    //forward
        //check wall bounce back case, otherwise use (col + 1)
    if ( (col + 1) > 3 || ( row == 1 && (col + 1) == 1 ) )
        forward_util = forward_probability * table_struct_global.utility_val[row][col];
    else
        forward_util = forward_probability * table_struct_global.utility_val[row][col + 1];
    //left
        //check bounce back case, otherwise use (row - 1)
    if ( (row - 1) < 0 || ( (row - 1) == 1 && col == 1 ) )
        left_util = left_probability * table_struct_global.utility_val[row][col];
    else
        left_util = left_probability * table_struct_global.utility_val[row - 1][col];
    //right
       //check bounce back case, otherwise use (row + 1)
    if ( (row + 1) > 2 || ( (row + 1) == 1 && col == 1 ) )
        right_util = right_probability * table_struct_global.utility_val[row][col];
    else
        right_util = right_probability * table_struct_global.utility_val[row + 1][col];
    //now add all values to get expected value:
    return (localReward + forward_util + left_util + right_util);


}
float expected_utility_SOUTH(int row, int col) {

    float localReward, forward_util, left_util, right_util;

    //SOUTH == FORWARD-->(row + 1), LEFT-->(col + 1), RIGHT-->(col - 1)

          //get local reward value from current location
    localReward = table_struct_global.tablePos_reward[row][col];
    //forward
        //check bounce back case, otherwise use (row + 1)
    if ( (row + 1) > 2 || ( (row + 1) == 1 && col == 1 ) )
        forward_util = forward_probability * table_struct_global.utility_val[row][col];
    else
        forward_util = forward_probability * table_struct_global.utility_val[row + 1][col];
    //left
        //check bounce back case, otherwise use (col + 1)
    if ( (col + 1) > 3 || ( row == 1 && (col + 1) == 1 ) )
        left_util = left_probability * table_struct_global.utility_val[row][col];
    else
        left_util = left_probability * table_struct_global.utility_val[row][col + 1];
    //right
       //check bounce back case, otherwise use (col - 1)
    if ( (col - 1) < 0 || ( row == 1 && (col - 1) == 1 ) )
        right_util = right_probability * table_struct_global.utility_val[row][col];
    else
        right_util = right_probability * table_struct_global.utility_val[row][col - 1];
    //now add all values to get expected value:
    return (localReward + forward_util + left_util + right_util);

}
void perform_BellmanUpdate(void) {

    for (int row = 0; row < 3; row++) {

        for (int col = 0; col < 4; col++) {

            float argMax_utility;
            std::vector<float> expectedVal;

            //skip finding utility for end states, or if at wall obstacle location
            if ((row == 0 && col == 3) || (row == 1 && col == 3) || (row == 1 && col == 1))
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










// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
