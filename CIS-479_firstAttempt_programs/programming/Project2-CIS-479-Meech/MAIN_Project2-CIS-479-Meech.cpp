//MAIN_Project2-CIS-479-Meech.cpp : This file contains the 'main' function. Program execution begins and ends there.
//Author: Demetrius E Johnson
//Purpose: Implement Robot Localization with the HMM (Hidden Markov Model) algorithm - CIS-479-Project2
//Special Notes: Adapated some of my Value Iteration Bellman Equation Program
//Date Created: 8/02/2022
//Date Modified: 8/09/2022

#include <iostream>
#include <iomanip>
#include <vector>
using namespace std;
///////DATA STRUCTURES && ENUMERATORS////////////////////////////////////////////////////


//note:
//static keyword will make the variables go in the .DATA section at compile time and not on the run time stack --> which requires an instnace of table to be called
//static variables therefore are shared by all objects of the class AND can be accessed even before an object of the class is declared.
struct table {

    const static int numRows = 6; 
    const static int numCols = 7;
    float tablePos_locationProb_prior[numRows][numCols];
    float tablePos_locationProb_posterior[numRows][numCols];
    void setPrior_toPosterior(void) {
    
        for (int i = 0; i < numRows; i++)
            for (int j = 0; j < numCols; j++)
                tablePos_locationProb_prior[i][j] = tablePos_locationProb_posterior[i][j];
    }
   
};

enum class Direction_Relative { NO_DIREC = 0, FORWARD = 1, LEFT = 2, RIGHT = 3 };
enum class Direction_Cardinal { WEST = 1, NORTH = 2, EAST = 3, SOUTH = 4 };
enum Location {OBSTACLE = 1, OPEN_SQUARE = 0};
//////////////////GLOBAL VARIABLES//////////////////////////////////////////
table table_struct_global;

//moving transition probabilities:
const float forward_probability = (float)0.8;     //move forward in desired direction = 80%
const float left_probability = (float)0.1;    //drift left in undesired direction = 10%
const float right_probability = (float)0.1;   //drift right in undesired direction = 10%

//obstacle sensor transition probabilities:
const float sensing_OBJ_correctly_probability = (float)0.85; //correctly sense square as an obstacle = 85%
const float sensing_OBJ_incorrectly_probability = (float)0.15; //false negative(no obstacle) - incorrectly sense square as NOT an obstacle (open square) = 15%

//open-square (non-obstacle) sensor transition probabilities:
const float sensing_no_OBJ_correctly_probability = (float)0.95; //correctly sense square as NOT an obstacle = 95%
const float sensing_no_OBJ_incorrectly_probability = (float)0.05; //false positive(is an obstacle) - incorrectly sense square as an obstacle = 5%

//use this variable as a flag array for West, North, East, and South respectively as to whether an obstacle was sensed or not; i.e. [0,0,0,0] == false for all directions == no object detected in all directions
Location evidenceArray_WNES[4] = { OPEN_SQUARE };
Direction_Cardinal robot_movement;
int num_Iterations = 0;

/////////////////////FUNCTION DECLARATIONS/////////////////////////////////
bool is_obstacleLocation(int row, int col);
void outputTable(table& table_struct);
void outputEvidenceArray(void);
void updateProb_sensing_filtering(table& table_struct, Location* sensor_evidence_bit_WNES);
void updatProb_moving_prediction(table& table_struct, Direction_Cardinal move_direction);
void getNeighbors_WNES(int row, int col, Location* neighbors);

//use these for the prediction probability updates
void prob_moving_WEST(int row, int col, table& table_struct);
void prob_moving_NORTH(int row, int col, table& table_struct);
void prob_moving_EAST(int row, int col, table& table_struct);
void prob_moving_SOUTH(int row, int col, table& table_struct);


int main()
{
    cout
        << "Welcome to the Robot Localization with HMM (Hidden Markov Model) Algorithm\nThe AI Robot aims to find itself in a windy maze."
        << "\nFor CIS-479 with Doctor Shengquan Wang"
        << "\nWritten 08-02-2022 to 08-09-2022";


//initialize location probability values:
    //note --> there are 6 rows (i) and 7 columns (j), and a total of 5 obstacles on the grid world maze.
    //The robot knows it is not on an obstacle square, and we assume
    //the robot knows how many total obstacles are in the maze and has a map of the maze (total squares) - and it is trying to locate itself on the map.
    //6 x 7 = 42 total squares; 42 - 5 --> total squares - obstacle sqaures = 37 possible locations the robot could be at initialization; 
    //thus 1/37 = 0.0270 == 2.70%  --> it is in any 1 of the open 37 squares.
    //So, we initialize all non-obstacles squares wtih (1/37)
    //(for sake of speed and efficiency, obstacle squares will be over written in next set of lines with the obstacle value (-1)):
    for (int i = 0; i < table_struct_global.numRows; i++) {

        for (int j = 0; j < table_struct_global.numCols; j++) {
            
            table_struct_global.tablePos_locationProb_prior[i][j] = ( (float)1 / (float)37 ); //need to cast as a float to do floating point division
            table_struct_global.tablePos_locationProb_posterior[i][j] = ((float)1 / (float)37);
        }
    }

    //initialize obstacle locations in table (# == -1 == wall/obstacle square)  (- == blank sqaure)
    /*
    i.e.: .

              j

              0   1   2   3   4   5   6
          
     i    0   -   -   -   -   -   -   -

          1   -   -   -   -   -   #   -

          2   -   #   -   -   #   -   -

          3   -   #   -   -   #   -   -

          4   -   -   -   -   -   -   -

          5   -   -   -   -   -   -   -

    */
    int obstacleVal = -1;
    //Now overwrite obstacle --> obstacle locations = -1 to initialize prior probability table
    table_struct_global.tablePos_locationProb_prior[2][1] = obstacleVal;
    table_struct_global.tablePos_locationProb_prior[3][1] = obstacleVal;
    table_struct_global.tablePos_locationProb_prior[2][4] = obstacleVal;
    table_struct_global.tablePos_locationProb_prior[3][4] = obstacleVal;
    table_struct_global.tablePos_locationProb_prior[1][5] = obstacleVal;

    //Now overwrite obstacle --> obstacle locations = -1 to initialize posterior probability table
    table_struct_global.tablePos_locationProb_posterior[2][1] = obstacleVal;
    table_struct_global.tablePos_locationProb_posterior[3][1] = obstacleVal;
    table_struct_global.tablePos_locationProb_posterior[2][4] = obstacleVal;
    table_struct_global.tablePos_locationProb_posterior[3][4] = obstacleVal;
    table_struct_global.tablePos_locationProb_posterior[1][5] = obstacleVal;


    //Output table to show initial location probabilities:
    cout << "\n\n\n";
    cout << "\n\nINITIAL LOCATION PROBABILITY TABLE (as a %) [UPDATE ITERATION #" << num_Iterations << "]:\n\n";
    outputTable(table_struct_global);

    //First evidence given is [0,0,0,0]; so initialize sensor evidence array to all 0s [W = 0, N=0, E=0, S=0]:
    for (int i = 0; i < 4; i++)
        evidenceArray_WNES[i] = OPEN_SQUARE;

    /////////SENSING [0,0,0,0]:
    
    //Perform filtering calculations in order to update probabilities based on the evidence:
    //Afterwards, update prior as = calculated posterior, for use in the next posterior calculation
    updateProb_sensing_filtering(table_struct_global, evidenceArray_WNES);
    table_struct_global.setPrior_toPosterior();
    outputEvidenceArray();
    cout << "LOCATION PROBABILITY TABLE AFTER FILTERING (as a %) [UPDATE ITERATION #" << num_Iterations << "]:\n\n";
    outputTable(table_struct_global);
    //////////////MOVING [NORTH]:

    //Perform prediction calculations to update probability after Action N:
    //Afterwards, update prior as = calculated posterior, for use in the next posterior calculation
    robot_movement = Direction_Cardinal::NORTH;
    updatProb_moving_prediction(table_struct_global, robot_movement);
    table_struct_global.setPrior_toPosterior();
    cout << "\nRobot Moving NORTH...";
    cout << "LOCATION PROBABILITY TABLE AFTER PREDICTION (as a %) [UPDATE ITERATION #" << num_Iterations << "]:\n\n";
    outputTable(table_struct_global);


    //Perform more sensing and moving updates:
    //We can perform as many updates as we like and get closer
    //and closer to the convergence value == robot's (most likely) location:

    int numUpdates = 1;
    int userMovement_input;
    int userEvidence_input[4];
    while (numUpdates != 0) {

        numUpdates--;
        num_Iterations++;
        if (numUpdates == 0) {
            cout << "Enter an INTEGER to specify the number of Sensor and Moving Updates to perform (enter 0 to exit program): ";
            cin >> numUpdates;
            if (numUpdates == 0)
                break;
        }

        //get user input for evidence
        cout << "Enter 4 INTEGER values (one at a time) (0=open to 1=object) to represent if an object was sensed [W,N,E,S]\n";
        for (int i = 0; i < 4; i++) {
            cout << "sense_WNES_" << i + 1 << ": ";
            cin >> userEvidence_input[i];
            evidenceArray_WNES[i] = (Location)userEvidence_input[i];
        }
        /////////SENSING [user input]:

        //Perform filtering calculations in order to update probabilities based on the evidence:
        //Afterwards, update prior as = calculated posterior, for use in the next posterior calculation
        updateProb_sensing_filtering(table_struct_global, evidenceArray_WNES);
        table_struct_global.setPrior_toPosterior();
        outputEvidenceArray();
        cout << "LOCATION PROBABILITY TABLE AFTER FILTERING (as a %) [UPDATE ITERATION #" << num_Iterations << "]:\n\n";
        outputTable(table_struct_global);

        cout << "Enter an INTEGER for the movement direction for the robot (W=1, N=2, E=3, S=4): ";
        cin >> userMovement_input;
        robot_movement = (Direction_Cardinal)userMovement_input;

        //////////////MOVING [userInput]:

        //Perform prediction calculations to update probability after Action :
        //Afterwards, update prior as = calculated posterior, for use in the next posterior calculation
        updatProb_moving_prediction(table_struct_global, robot_movement);
        table_struct_global.setPrior_toPosterior();
        switch(robot_movement) {
        
            case Direction_Cardinal::WEST:
                cout << "\nRobot Moving WEST...";
                break;
            case Direction_Cardinal::NORTH:
                cout << "\nRobot Moving NORTH...";
                break;
            case Direction_Cardinal::EAST:
                cout << "\nRobot Moving EAST...";
                break;
            case Direction_Cardinal::SOUTH:
                cout << "\nRobot Moving SOUTH...";
                break;

        }
        cout << "\n\nLOCATION PROBABILITY TABLE AFTER PREDICTION (as a %)[UPDATE ITERATION #" << num_Iterations << "]:\n\n";
        outputTable(table_struct_global);
    }

    return 0;
}


///////////////////////////FUNCTION DEFINITIONS/////////////////////////////////////////
bool is_obstacleLocation(int row, int col) {
    //check to see if the location at [row][col] is an maze obstacle or a wall boundary obstacle
    if (table_struct_global.tablePos_locationProb_prior[row][col] == -1 
        || row < 0 
        || col < 0 
        || row >= table_struct_global.numRows 
        || col >= table_struct_global.numCols)
        return true;
    else
        return false;
}
void outputTable(table& table_struct) {


    //print utility table:
    for (int i = 0; i < table_struct_global.numRows; i++) {

        for (int j = 0; j < table_struct_global.numCols; j++) {

            if (is_obstacleLocation(i, j)) { //OUTPUT OBSTACLE LOCATION

                cout
                    << right           //define alignment 
                    << setprecision(1) //(output only 1 decimal place) --> does not apply here since we are not outputting a number
                    << fixed           //align decimal, print with set precision
                    << setfill(' ')    //fill unused space with specified char
                    << setw(12)        //define width of output (min # of char positions a variable will consume)
                    << "[ ]  ";
            }
            else {
                cout
                    << setprecision(2) //output only 2 decimal place
                    << fixed           //align decimal, print with set precision
                    << right           //define alignment 
                    << setw(10)         //define width of output (min # of char positions a variable will consume)
                    << setfill(' ')    //fill rest with chosen char
                    << (100 * table_struct.tablePos_locationProb_prior[i][j]) //output as a percent, so multiply by 100
                    << "  ";
            }
        }
        cout << endl << endl << endl << endl;
    }
}
void outputEvidenceArray(void) {

    cout << "Robot sensor evidence collected [W,N,E,S]: [";
    for (int i = 0; i < 4; i++) {
        cout
            << evidenceArray_WNES[i]
            << ", ";
    }
    cout << "\b\b]" << endl;
}
void updateProb_sensing_filtering(table& table_struct, Location* sensor_evidence_bit_WNES) {


    //Filtering: P(St|Z1=z1, …, Zt=zt) ∝ P(Zt=zt|St) P(St|Z1=z1, …, Zt-1=zt-1)

    /* note that Z_t represents an evidence variable --> evidence from the neighbor sensor
        Zt is composed of 4 random variables for four directions: 
        Zt = (ZW,t,  ZN,t,  ZE,t, ZS,t)

        For each state St, conditionally independent:
        P(Zt|St) = P(ZW,t|St) P(ZN,t|St) P(ZE,t|St) P(ZS,t|St)


        note that: 
        
        P(Zt=zt|St)==  likelihood
        P(St|Z1=z1, …, Zt-1=zt-1) ==  prior
        P(St|Z1=z1, …, Zt=zt) == posterior
    
    */

    float sum_of_proportions = 0; //sum of proportions == SUM for all States (S_t) of [  P(Zt=zt|St) P(St|Z1=z1, …, Zt-1=zt-1)  ].
                                  //add all proportions to get total probability of evidence Z_t given S_t, 
                                  //then we can use this sum to get  P(St|Z1=z1, …, Zt=zt) == [  P(Zt=zt|St) P(St|Z1=z1, …, Zt-1=zt-1) / sum_of_proportions  ].
                                  //This is the same thing as saying: [1 of the proportions / sum_of_propotions]...
                                  //--> and we can do this for all states to find the probabiility the robot is at any given location given evidence(s) Z_t.
    
    Location neighbors_WNES[4]; //use this to get the true neighbor based om the true map the robot has access to to compare it with the sensor evidence

    for (int row = 0; row < table_struct.numRows; row++) {
        for (int col = 0; col < table_struct.numCols; col++) {
             
            if (is_obstacleLocation(row, col)) //no calculation necessary for obstacle case --> robot knows it cannot be there
                continue;

            //check neighboring squares for obstacles so we can determine which sensor error transistion probability to use at each possible state S_t
            getNeighbors_WNES(row, col, neighbors_WNES);

            //calculate likelihood --> P(Z_t | S_t) --> obstacle or open square probability at a given state S_t = s_t, based on evidence from sensor:
            float sensorProb_WNES[4];
            for (int i = 0; i < 4; i++) {

                //case: correctly sense an obstacle ==85%
                if (sensor_evidence_bit_WNES[i] == OBSTACLE && neighbors_WNES[i] == OBSTACLE)
                    sensorProb_WNES[i] = sensing_OBJ_correctly_probability;

                //case: incorrectly sense an obstacle as open square (false negative)  ==15%
                else if (sensor_evidence_bit_WNES[i] == OPEN_SQUARE && neighbors_WNES[i] == OBSTACLE)
                    sensorProb_WNES[i] = sensing_OBJ_incorrectly_probability;

                //case: correctly sense an open sqaure ==95%
                else if (sensor_evidence_bit_WNES[i] == OPEN_SQUARE && neighbors_WNES[i] == OPEN_SQUARE)
                    sensorProb_WNES[i] = sensing_no_OBJ_correctly_probability;

                //case: incorrectly sense an open sqaure as an obstacle (false positive) ==5%
                else if (sensor_evidence_bit_WNES[i] == OBSTACLE && neighbors_WNES[i] == OPEN_SQUARE)
                    sensorProb_WNES[i] = sensing_no_OBJ_incorrectly_probability;
            }

            //multiply all conditionall indepdent likelihoods to get total likelihood of a state S_t given given sensor evidence Z_t
            float state_totalLikelihood = 1;
            for (int i = 0; i < 4; i++)
                state_totalLikelihood *= sensorProb_WNES[i];

            //Now, multiply total likelihood by the prior and add that to the sum 
            sum_of_proportions += (state_totalLikelihood * table_struct.tablePos_locationProb_prior[row][col]);
        }
    }

    //now, sum of proportions has been acquired, we now can normalize and find the posterior probabilitl for all states: 
    // P(St|Z1=z1, …, Zt=zt) == [  P(Zt=zt|St) P(St|Z1=z1, …, Zt-1=zt-1) / sum_of_proportions  ] 

    for (int row = 0; row < table_struct.numRows; row++) {
        for (int col = 0; col < table_struct.numCols; col++) {

            if (is_obstacleLocation(row, col)) //no calculation necessary for obstacle case --> robot knows it cannot be there
                continue;

            //check neighboring squares for obstacles so we can determine which sensor error transistion probability to use at each possible state S_t
            getNeighbors_WNES(row, col, neighbors_WNES);

            //calculate likelihood --> P(Z_t | S_t) --> obstacle or open square probability at a given state S_t = s_t, based on evidence from sensor:
            float sensorProb_WNES[4];
            for (int i = 0; i < 4; i++) {

                //case: correctly sense an obstacle ==85%
                if (sensor_evidence_bit_WNES[i] == OBSTACLE && neighbors_WNES[i] == OBSTACLE)
                    sensorProb_WNES[i] = sensing_OBJ_correctly_probability;

                //case: incorrectly sense an obstacle as open square (false negative)  ==15%
                else if (sensor_evidence_bit_WNES[i] == OPEN_SQUARE && neighbors_WNES[i] == OBSTACLE)
                    sensorProb_WNES[i] = sensing_OBJ_incorrectly_probability;

                //case: correctly sense an open sqaure ==95%
                else if (sensor_evidence_bit_WNES[i] == OPEN_SQUARE && neighbors_WNES[i] == OPEN_SQUARE)
                    sensorProb_WNES[i] = sensing_no_OBJ_correctly_probability;

                //case: incorrectly sense an open sqaure as an obstacle (false positive) ==5%
                else if (sensor_evidence_bit_WNES[i] == OBSTACLE && neighbors_WNES[i] == OPEN_SQUARE)
                    sensorProb_WNES[i] = sensing_no_OBJ_incorrectly_probability;
            }

            //multiply all conditionall indepdent likelihoods to get total likelihood of a state S_t given given sensor evidence Z_t
            float state_totalLikelihood = 1;
            for (int i = 0; i < 4; i++)
                state_totalLikelihood *= sensorProb_WNES[i];

            //Now, multiply total likelihood by the prior 
           //Now, at this moment we have calculated one proportion; now, 
           //we normalize since we already have sum of all proportions, to get posterior for the current state at [row][col]
            table_struct.tablePos_locationProb_posterior[row][col] = (state_totalLikelihood * table_struct.tablePos_locationProb_prior[row][col]) / sum_of_proportions;
            

        }
    }

}
void updatProb_moving_prediction(table& table_struct, Direction_Cardinal move_direction) {

 
    //Prediction: P(St+1|Z1=z1, …, Zt=zt) = ∑sP(St+1|St=s) P(St|Z1=z1, …, Zt=zt)
    //The prediction is essentially just the sum of all probabilities that you can get to state S_t+1, given all possible prior states S_t.

    //first, reinitialize the posterior table, so that we can incrementally sweep across it and add the probability of getting to one state from one of the other possible states
    for (int i = 0; i < table_struct.numRows; i++) {

        for (int j = 0; j < table_struct.numCols; j++) {

            if (is_obstacleLocation(i, j))
                continue;
            else
                table_struct.tablePos_locationProb_posterior[i][j] = 0;
        }
    }

    //now sweep across all locations adding any probability from neighbor states that it can be reached from - when done, then all probability for reaching a state S_t+1 from all other possible states S_t will be complete
    for (int row = 0; row < table_struct.numRows; row++) {
        for (int col = 0; col < table_struct.numCols; col++) {

            if (is_obstacleLocation(row, col)) //no need to check impossible cases since robot cannot be at an obstacle location
                continue;

            switch (move_direction) {



            case Direction_Cardinal::WEST:

                prob_moving_WEST(row, col, table_struct);
                break;
            case Direction_Cardinal::NORTH:

                prob_moving_NORTH(row, col, table_struct);
                break;
            case Direction_Cardinal::EAST:

                prob_moving_EAST(row, col, table_struct);
                break;
            case Direction_Cardinal::SOUTH:

                prob_moving_SOUTH(row, col, table_struct);
                break;
            }
        }

    }
}
void getNeighbors_WNES(int row, int col, Location* neighbors) {
    //check to see if whether each neighbor is an obstacle or an open square:
    
    //get WEST neighbor
    if (is_obstacleLocation(row, col - 1))
        neighbors[0] = OBSTACLE;
    else
        neighbors[0] = OPEN_SQUARE;
    //get NORTH neighbor
    if (is_obstacleLocation(row - 1, col))
        neighbors[1] = OBSTACLE;
    else
        neighbors[1] = OPEN_SQUARE;

    //get EAST neighbor
    if (is_obstacleLocation(row, col + 1))
        neighbors[2] = OBSTACLE;
    else
        neighbors[2] = OPEN_SQUARE;

    //get SOUTH neighbor
    if (is_obstacleLocation(row + 1, col))
        neighbors[3] = OBSTACLE;
    else
        neighbors[3] = OPEN_SQUARE;

}
void prob_moving_WEST(int row, int col, table& table_struct) {


    //WEST == FORWARD-->(col - 1), LEFT-->(row + 1), RIGHT-->(row - 1):

    //forward 
        //check wall bounce back case, otherwise use (col - 1)
    if (is_obstacleLocation(row, col - 1))
        //current [row][col] location absorbs the probability
        table_struct.tablePos_locationProb_posterior[row][col] += (forward_probability * table_struct.tablePos_locationProb_prior[row][col]);
    else
        //otherwise, forward location absorbs the probability
        table_struct.tablePos_locationProb_posterior[row][col - 1] += (forward_probability * table_struct.tablePos_locationProb_prior[row][col]);
    //left
        //check bounce back case, otherwise use (row + 1)
    if (is_obstacleLocation(row + 1, col))
        //current [row][col] location absorbs the probability
        table_struct.tablePos_locationProb_posterior[row][col] += (left_probability * table_struct.tablePos_locationProb_prior[row][col]);
    else
        //otherwise, left location absorbs the probability
        table_struct.tablePos_locationProb_posterior[row + 1][col] += (left_probability * table_struct.tablePos_locationProb_prior[row][col]);
    //right
       //check bounce back case, otherwise use (row - 1)
    if (is_obstacleLocation(row - 1, col))
        table_struct.tablePos_locationProb_posterior[row][col] += (right_probability * table_struct.tablePos_locationProb_prior[row][col]);
    else
        //otherwise, right location absorbs the probability
        table_struct.tablePos_locationProb_posterior[row - 1][col] += (right_probability * table_struct.tablePos_locationProb_prior[row][col]);
}
void prob_moving_NORTH(int row, int col, table& table_struct) {


    //NORTH == FORWARD-->(row - 1), LEFT-->(col - 1), RIGHT-->(col + 1):

    //forward
        //check bounce back case, otherwise use (row - 1)
    if (is_obstacleLocation(row - 1, col))
        //current [row][col] location absorbs the probability
        table_struct.tablePos_locationProb_posterior[row][col] += (forward_probability * table_struct.tablePos_locationProb_prior[row][col]);
    else
        //otherwise, right location absorbs the probability
        table_struct.tablePos_locationProb_posterior[row - 1][col] += (forward_probability * table_struct.tablePos_locationProb_prior[row][col]);
    //left
        //check bounce back case, otherwise use (col - 1)
    if (is_obstacleLocation(row, col - 1))
        //current [row][col] location absorbs the probability
        table_struct.tablePos_locationProb_posterior[row][col] += (left_probability * table_struct.tablePos_locationProb_prior[row][col]);
    else
        //otherwise, right location absorbs the probability
        table_struct.tablePos_locationProb_posterior[row][col - 1] += (left_probability * table_struct.tablePos_locationProb_prior[row][col]);
    //right
       //check bounce back case, otherwise use (col + 1)
    if (is_obstacleLocation(row, col + 1))
        //current [row][col] location absorbs the probability
        table_struct.tablePos_locationProb_posterior[row][col] += (right_probability * table_struct.tablePos_locationProb_prior[row][col]);
    else
        //otherwise, right location absorbs the probability
        table_struct.tablePos_locationProb_posterior[row][col + 1] += (right_probability * table_struct.tablePos_locationProb_prior[row][col]);



}
void prob_moving_EAST(int row, int col, table& table_struct) {


    //EAST == FORWARD-->(col + 1), LEFT-->(row - 1), RIGHT-->(row + 1):

    //forward
        //check wall bounce back case, otherwise use (col + 1)
    if (is_obstacleLocation(row, col + 1))
        //current [row][col] location absorbs the probability
        table_struct.tablePos_locationProb_posterior[row][col] += (forward_probability * table_struct.tablePos_locationProb_prior[row][col]);
    else
        //otherwise, right location absorbs the probability
        table_struct.tablePos_locationProb_posterior[row][col + 1] += (forward_probability * table_struct.tablePos_locationProb_prior[row][col]);
    //left
        //check bounce back case, otherwise use (row - 1)
    if (is_obstacleLocation(row - 1, col))
        //current [row][col] location absorbs the probability
        table_struct.tablePos_locationProb_posterior[row][col] += (left_probability * table_struct.tablePos_locationProb_prior[row][col]);
    else
        //otherwise, right location absorbs the probability
        table_struct.tablePos_locationProb_posterior[row - 1][col] += (left_probability * table_struct.tablePos_locationProb_prior[row][col]);
    //right
       //check bounce back case, otherwise use (row + 1)
    if (is_obstacleLocation(row + 1, col))
        //current [row][col] location absorbs the probability
        table_struct.tablePos_locationProb_posterior[row][col] += (right_probability * table_struct.tablePos_locationProb_prior[row][col]);
    else
        //otherwise, left location absorbs the probability
        table_struct.tablePos_locationProb_posterior[row + 1][col] += (right_probability * table_struct.tablePos_locationProb_prior[row][col]);


}
void prob_moving_SOUTH(int row, int col, table& table_struct) {


    //SOUTH == FORWARD-->(row + 1), LEFT-->(col + 1), RIGHT-->(col - 1)

    //forward
        //check bounce back case, otherwise use (row + 1)
    if (is_obstacleLocation(row + 1, col))
        //current [row][col] location absorbs the probability
        table_struct.tablePos_locationProb_posterior[row][col] += (forward_probability * table_struct.tablePos_locationProb_prior[row][col]);
    else
        //otherwise, left location absorbs the probability
        table_struct.tablePos_locationProb_posterior[row + 1][col] += (forward_probability * table_struct.tablePos_locationProb_prior[row][col]);
    //left
        //check bounce back case, otherwise use (col + 1)
    if (is_obstacleLocation(row, col + 1))
        //current [row][col] location absorbs the probability
        table_struct.tablePos_locationProb_posterior[row][col] += (left_probability * table_struct.tablePos_locationProb_prior[row][col]);
    else
        //otherwise, right location absorbs the probability
        table_struct.tablePos_locationProb_posterior[row][col + 1] += (left_probability * table_struct.tablePos_locationProb_prior[row][col]);
    //right
       //check bounce back case, otherwise use (col - 1)
    if (is_obstacleLocation(row, col - 1))
        //current [row][col] location absorbs the probability
        table_struct.tablePos_locationProb_posterior[row][col] += (right_probability * table_struct.tablePos_locationProb_prior[row][col]);
    else
        //otherwise, right location absorbs the probability
        table_struct.tablePos_locationProb_posterior[row][col - 1] += (right_probability * table_struct.tablePos_locationProb_prior[row][col]);
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
