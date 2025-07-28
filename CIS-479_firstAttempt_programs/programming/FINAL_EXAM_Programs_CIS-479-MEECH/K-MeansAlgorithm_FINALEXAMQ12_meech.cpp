// K-MeansAlgorithm_HW4_Q6-CIS-479_meech.cpp : This file contains the 'main' function. Program execution begins and ends there.
//Author: Demetrius Johnson (Meech)
//Purpose: Create a K-Means algorithm in order to answer question 6 from HW4, CIS-479 SUMMER II with DR. Shengquan Wang
//Date created: 8/16/22
//Date Modified: 8/17/22
/*

We have the following eight 2D data points:
X          Y
24.5     3.5
26.5     7.0
21.5     7.5
25.0    15.5
2.5     20.5
9.5     23.0
11.0    20.0
21.5     4.0
Two initial centroids (12.0, 12.5), (15.0, 15.5). Please find the next two centroids after one iteration using K-means
with K = 2 and Euclidean distance.



*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <iomanip>
using namespace std;

/////////////////FUNCTION DECLARATIONS, STRUCTS, ENUM DECLARATIONS, GLOBAL VAR//////////////////////////////
struct XY_coord { float x; float y; };
int numDataPts = 0;
int numUpdatesPerformed = 0;
int K = 2;

bool init_data_array(vector<XY_coord>& data_array);
void calc_Euclidean_distances(vector<XY_coord>& data_array, XY_coord& centroid, vector<float>& E_array);
void compare_Euclidean_distances(vector<float>& E1_array, vector<float>& E2_array);
void output_centroid_1(XY_coord& c1);
void output_centroid_2(XY_coord& c2);
void output_E_distances_c1(vector<float>& c1_E, XY_coord& c1, vector<XY_coord>& dataSet);
void output_E_distances_c2(vector<float>& c2_E, XY_coord& c2, vector<XY_coord>& dataSet);
void update_Centroid(XY_coord& c, vector<float>& E_array, vector<XY_coord>& dataSet);

//////////MAIN FUNCTION//////////////////
int main()
{
    
    cout << "Welcome to the K-Means Algorithm Program, by Demetrius Johnson (meech)\n"
            "This program was written in order to answer HW4_Q6\nCIS-479 SUMMER II w/Dr. Shengquan Wang\n";

   


    //initialize data points array
    vector<XY_coord> dataPts_array;
    if (!init_data_array(dataPts_array)) {
        system("pause");
        return -1;
    } //init must return true else failure to initialize and we exit program

    //output data set:
    cout << "DATA SET READ FROM FILE:\n";
    cout << (right, setw(6), setfill(' ')) << "\tx" << "\ty" << endl;
    for (int i = 0; i < numDataPts; i++)
        cout << (right, setw(6), setfill(' ')) << "\t" << dataPts_array[i].x << "\t" << dataPts_array[i].y << endl;


    //initialize k values (centroids); k =2. Also, output centroids:
    XY_coord centroid_1{ 0.7, 0.0 }, centroid_2{ 0.9, 0.0 };
    cout << "\nK (number of centroids) = " << K << endl;
    output_centroid_1(centroid_1);
    output_centroid_2(centroid_2);
    cout << endl;

    //Calculate Euclidean distance of a centroid from each point
    vector<float>centroid_1_E_distance(numDataPts);
    vector<float>centroid_2_E_distance(numDataPts);
    calc_Euclidean_distances(dataPts_array, centroid_1, centroid_1_E_distance);
    calc_Euclidean_distances(dataPts_array, centroid_2, centroid_2_E_distance);

    //output E distances
    output_E_distances_c1(centroid_1_E_distance, centroid_1, dataPts_array);
    output_E_distances_c2(centroid_2_E_distance, centroid_2, dataPts_array);

    //compare centroid distances for all points
    compare_Euclidean_distances(centroid_1_E_distance, centroid_2_E_distance);
    
    //output E distances after comparison so we can easily see which data pts belong to which centroid
    cout << "\nAfter Comparison (data pts that will be used to update a given centroid):\n";
    output_E_distances_c1(centroid_1_E_distance, centroid_1, dataPts_array);
    output_E_distances_c2(centroid_2_E_distance, centroid_2, dataPts_array);

    //update centroids based on the data points to which they belong
    update_Centroid(centroid_1, centroid_1_E_distance, dataPts_array);
    update_Centroid(centroid_2, centroid_2_E_distance, dataPts_array);
    numUpdatesPerformed++;

    cout << "New centroid values after update#" << numUpdatesPerformed << ":\n";
    output_centroid_1(centroid_1);
    output_centroid_2(centroid_2);
    cout << "\n\n----------------------END OF UPDATE#" << numUpdatesPerformed << "---------------------------------------\n\n";

    //repeat above process per user's desire. Centroids will converge to some value as more iterations are completed:
    int userChoice = 0;
    cout << "\nPerform another centroid update for all centroids?\nEnter a positive integer for number of updates to perform, or a negative integer to exit: ";
    cin >> userChoice;
    while (userChoice >= 0) {

        for (int i = 0; i < userChoice; i++) {
            cout << "\n\n----------------------BEGIN UPDATE#" << numUpdatesPerformed + 1 << "---------------------------------------\n";
            //Calculate Euclidean distance of a centroid from each point
            calc_Euclidean_distances(dataPts_array, centroid_1, centroid_1_E_distance);
            calc_Euclidean_distances(dataPts_array, centroid_2, centroid_2_E_distance);

            ///output E distances
            output_E_distances_c1(centroid_1_E_distance, centroid_1, dataPts_array);
            output_E_distances_c2(centroid_2_E_distance, centroid_2, dataPts_array);

            //compare centroid distances for all points
            compare_Euclidean_distances(centroid_1_E_distance, centroid_2_E_distance);

            //output E distances after comparison so we can easily see which data pts belong to which centroid
            cout << "\nAfter Comparison (data pts that will be used to update a given centroid):\n";
            output_E_distances_c1(centroid_1_E_distance, centroid_1, dataPts_array);
            output_E_distances_c2(centroid_2_E_distance, centroid_2, dataPts_array);

            //update centroids based on the data points to which they belong
            update_Centroid(centroid_1, centroid_1_E_distance, dataPts_array);
            update_Centroid(centroid_2, centroid_2_E_distance, dataPts_array);
            numUpdatesPerformed++;

            cout << "New centroid values after update#" << numUpdatesPerformed << ":\n";
            output_centroid_1(centroid_1);
            output_centroid_2(centroid_2);
            cout << "\n\n----------------------END OF UPDATE#" << numUpdatesPerformed << "---------------------------------------\n\n";
        }

        cout << "\nPerform another centroid update for all centroids?\nEnter a positive integer for number of updates to perform, or a negative integer to exit: ";
        cin >> userChoice;
    }

    cout << "\n###User Choice = " << userChoice << "; Exiting Program....\n";
    system("pause");
    return 0;

}



///////////////////////////FUNCTION DEFINITIONS///////////////////////////////////
bool init_data_array(vector<XY_coord>& data_array) {

    //open file stream and get data from file
    ifstream dataPtsFilePtr("DataPointsXY.txt");
    if (!dataPtsFilePtr.is_open()) {
        cout << "\n##data file not found. exiting program##";
        return false;
    }

    while (!dataPtsFilePtr.eof()) {

        string data_string;
        float coord;
        XY_coord dataPt;

        getline(dataPtsFilePtr, data_string, ' '); // get an X coordinate point
        coord = (stof(data_string)); //convert string to float;
        dataPt.x = coord;
        data_string.clear(); //reset string

        getline(dataPtsFilePtr, data_string, '\n'); // get a Y coordinate point
        coord = (stof(data_string)); //convert string to float;
        dataPt.y = coord;
        data_string.clear(); //reset string

        data_array.push_back(dataPt);
        numDataPts++; //keep track of number of data points added; may be useful for some functions
    }
    dataPtsFilePtr.close();

    return true;
}
void calc_Euclidean_distances(vector<XY_coord>& data_array, XY_coord& centroid, vector<float>& E_array) {

    for (int i = 0; i < numDataPts; i++) {

        float x_diff;
        float y_diff;

        //find centroid E distance from each data point:

        x_diff = data_array[i].x - centroid.x;
        y_diff = data_array[i].y - centroid.y;
        //use Pythagorean Theorem --> Euclidean (E) distance = sqrt(a^2 + b^2)
        E_array[i] = sqrt((pow(x_diff, 2) + pow(y_diff, 2)));

       
    }




}
void compare_Euclidean_distances(vector<float>& E1_array, vector<float>& E2_array) {

    //compare centroid distances for all points:

    for (int i = 0; i < numDataPts; i++) {
    
        if (E1_array[i] < E2_array[i])
            E2_array[i] = -FLT_MAX; //E2 will take on max negative value to delineate that E1 is closer to data point i
        else
            E1_array[i] = -FLT_MAX; //E1 will take on max negative value to delineate that E2 is closer to data point i
    }
}
void output_centroid_1(XY_coord& c1) {

    cout << "Centroid 1 = (" << c1.x << ", " << c1.y << ")\n";
}
void output_centroid_2(XY_coord& c2) {

    cout << "Centroid 2 = (" << c2.x << ", " << c2.y << ")\n";
}
void output_E_distances_c1(vector<float>& c1_E, XY_coord& c1, vector<XY_coord>& dataSet) {
    
    //only output the E distnace value if it is != -FLT_MAX so we can see which data point a given centroid belongs to:
    output_centroid_1(c1);
    cout << (right, setw(6), setfill(' ')) << "\tx" << "\ty" << "\tEuclidean Distance" << endl;
    for (int i = 0; i < numDataPts; i++)
        if (c1_E[i] != -FLT_MAX)
            cout << (right, setw(6), setfill(' '), setprecision(6)) << "\t" << dataSet[i].x << "\t" << dataSet[i].y << "\t" << c1_E[i] << endl;


}
void output_E_distances_c2(vector<float>& c2_E, XY_coord& c2, vector<XY_coord>& dataSet) {
   
    //only output the E distnace value if it is != -FLT_MAX so we can see which data point a given centroid belongs to:
    output_centroid_2(c2);
    cout << (right, setw(6), setfill(' ')) << "\tx" << "\ty" << "\tEuclidean Distance" << endl;
    for (int i = 0; i < numDataPts; i++)
        if (c2_E[i] != -FLT_MAX)
            cout << (right, setw(6), setfill(' '), setprecision(6)) << "\t" << dataSet[i].x << "\t" << dataSet[i].y << "\t" << c2_E[i] << endl;


}
void update_Centroid(XY_coord& c, vector<float>& E_array, vector<XY_coord>& dataSet) {

    //Update centroid's X and Y coordinate to be correspondingly the average of all points to which it belongs:
    float sum_x_coord = 0;
    float sum_y_coord = 0;
    float count = 0;
    for (int i = 0; i < numDataPts; i++) {
    
        if (E_array[i] != -FLT_MAX) {
            sum_x_coord += dataSet[i].x;
            sum_y_coord += dataSet[i].y;
            count++;
        }
    }

    c.x = (sum_x_coord / count);
    c.y = (sum_y_coord / count);
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
