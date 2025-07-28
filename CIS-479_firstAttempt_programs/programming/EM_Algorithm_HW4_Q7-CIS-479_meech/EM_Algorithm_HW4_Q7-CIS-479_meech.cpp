// EM_Algorithm_HW4_Q7-CIS-479_meech.cpp : This file contains the 'main' function. Program execution begins and ends there.
//Author: Demetrius Johnson (Meech)
//Purpose: Create the Exepectation Maximization (EM) algorithm in order to answer question 7 from HW4, CIS-479 SUMMER II with DR. Shengquan Wang
//Date created: 8/17/22
//Date Modified: 8/18/22

#include <iostream>
#include <stdlib.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>
#include <iomanip>
using namespace std;


/*

We have the following six 1D data_set points: 1.6, 2.1, 1.8, 5.3, 6.7, 4.7. 
We consider EM with two components with the following initial setup: 
π_1 = 0.4, µ_1 = 1, (σ_1)^2 = 2, π_2 = 0.6, µ_2 = 2, (σ_2)^2 = 2, 
where π_k = weight of a cluster on all data_set pts, µ_k = mean value of a cluster, σ_k = standard deviation of a cluster.
Please find the updated values of these 6 variables after one iteration using EM.
*/

/*
////////////////////NOTES BEGIN//////////////////////////////////////////////

Guassian Distribution function with d demensions, where T is the transpose matrix (necessary for dimensions of d=3 or greater), µ=mean, 𝜮=standard deviation:

P(x | µ, 𝜮) =  𝑁(𝝁,𝜮)(𝒙) = (  exp⁡( (−1/2) * (𝒙−𝝁)^𝑇 * Σ^(−1) * (𝒙−𝝁) )  ) / sqrt( (2𝜋)^𝑑 * |𝜮|) --> |𝜮| = magintiude of matrix 𝜮, Σ^(−1) = inverse of matrix Σ

-->for 1 dimension (d=1), the Gaussian Distribution Function simplifies to:
P(x | µ, 𝜮) =  𝑁(𝝁,𝜮)(𝒙) = (  exp⁡( (−1/2) * (𝒙−𝝁)^(2) * Σ^(-2) )  ) / sqrt( (2𝜋) * 𝜮^(2) )


Thus, for Guassian Learning with MLE (max likelihood estimation):

𝑝(𝒙_1,…,𝒙_𝑁 ) = ∏_𝑛( 𝑁(𝝁,𝜮)(𝒙_𝑛) ), where 𝑁(𝝁,𝜮)(𝒙_n) represents the distribution function given a population N as shown above.

NOTE: N refers to distribution given a population for the above f(x), it is not multiplied; it just represents an f(x) given a mean and standard deviation for any input x essentially.
NOTE linear algebra concept: (𝒙_𝑛−𝝁)^𝑇  * Σ^(−1) * (𝒙_𝑛−𝝁) is a d_length column vector (the trasnposed d_length row vector) * Σ = dxd square matrix * d_length row vector = a scalar number = p(x).
Note that: exp() means e^()

Also: 𝑝(𝒙_1,…,𝒙_𝑁 ) = ∏_𝑛( 𝑁(𝝁,𝜮)(𝒙_𝑛) ) simplifies to = ( (2𝜋)^𝑑 * |𝜮| )^(−𝑁/2) * exp(  (−1/2) * (∑_𝑛( (𝒙_𝑛−𝝁)^𝑇  * Σ^(−1) * (𝒙_𝑛−𝝁) )  ) --> -N/2 refers to -1 * (population / 2), |𝜮| = magintiude of matrix 𝜮

For 1 dimension (d=1), the above equations simplify to:

𝑝(𝒙_1,…,𝒙_𝑁 ) = ∏_𝑛( 𝑁(𝝁,𝜮)(𝒙_𝑛) ) = ( (2𝜋) * 𝜮^2 )^(−𝑁/2) * exp(  (−1/2) * (∑_𝑛( (𝒙_𝑛−𝝁)^(2)  * Σ^(−2) )  ) --> -N/2 refers to -1 * (population / 2)

Another way to write the above function:

𝑝(𝒙_1,…,𝒙_𝑁 ) = ∏_𝑛( 𝑁(𝝁,𝜮)(𝒙_𝑛) ) = ( (2𝜋) * standard_deviation^2 )^(−population/2) * e^(  (−1/2) * (sum of all square errors  * standard_deviation^(−2) )  )

or

𝑝(𝒙_1,…,𝒙_𝑁 ) = ∏_𝑛( 𝑁(𝝁,𝜮)(𝒙_𝑛) ) = ( (2𝜋) * variance^2 )^(−population/2) * e^(  (−1/2) * (sum of all square errors  * variance^(−1) )  )


Remember:
Variance = (standard deviation)^2 = MSE (mean square error) = 𝜎^2 = (1/𝑁) * ∑_𝑛(𝑥_𝑛−𝜇)^2
Mean = µ = (1/N) * ∑_𝑛(x_n)


Gaussian Mixture Model:

# P(𝒙) = ∑_𝑘( P(𝒙│C=k) * P(C=k) ), where
	^ P(𝒙│C=k) = 𝑁(𝝁_𝑘,𝜮_𝑘 )(𝒙) is a Gaussian source
	^ P(C=k) = 𝜋_𝑘 is weight of each soft cluster
# Latent variable 𝜋_𝑘; unknown parameters 𝝁_𝑘 and 𝜮_𝑘  (here, 𝝁_𝑘 refers to mean, 𝜮_𝑘 refers to the stanard deviation, σ_k).

In statistics, latent variables are variables that are not directly observed
but are rather inferred through a mathematical model from other variables that are observed.
Mathematical models that aim to explain observed variables
in terms of latent variables are called latent variable models.

So how to obtain the latent variables 𝝁_𝑘 and 𝜮_𝑘 ? --> Use the EM (Expectation Maximization) Algorithm:

E-step:
Assign each data_set item into each cluster with a probability.
M-step:
Update the mean and variance of each Gaussian source.

Compare the above EM steps to the K-Means algorithm (EM uses soft clusters; K-means uses hard clusters):

Step 1:
Assign each data_set item into the nearest centroid.
Step 2:
Update centroids


----///EM (Expectaion Maximization) Algorithm (for 1D case):///----

Initialization: first, set (choose) random values of 𝜋_𝑘, 𝝁_𝑘,𝜮_𝑘  (similar to selecting a start location for all centroids in the K-Means algorithm)

E-step: Obtain 𝑝_(𝑘,𝑛) for data_set item 𝒙_𝑛 in soft cluster k.

--> 𝑝_(𝑘,𝑛) = P(C=k│𝒙_𝑛 ), which is proportional to P(𝒙_𝑛│C=k) * P(C=k) = 𝑁(𝝁_𝑘,𝜮_𝑘 )(𝒙_𝑛 ) * 𝜋_𝑘
--> C refers to soft custer random variable (composed of k categories = number of soft clusters).

--> 𝑝_(𝑘,𝑛) is proportional to 𝜋_𝑘 * 𝑁(𝝁_𝑘,𝜮_𝑘 )(𝒙_𝑛 )

--> 𝑝_(𝑘,𝑛) = 𝑝_(𝑘,𝑛) / (∑_𝑘( 𝑝_(𝑘,𝑛) )


M-step: Then we refit each soft cluster k to the entire data_set set with each data_set weighted by 𝑝_(𝑘,𝑛), where
	𝜋_𝑘 = weight of k, 𝝁_𝑘 = mean of k, 𝜮_𝑘 = standard deviation of k.

-->𝜋_𝑘 = (∑_𝑛( 𝑝_(𝑘,𝑛) )/𝑁

-->𝝁_𝑘 = ( ∑_𝑛( 𝑝_(𝑘,𝑛) * 𝒙_𝑛 ) ) / (  ∑_𝑛( 𝑝_(𝑘,𝑛) )  )

-->𝜮_𝑘 = (  ∑_𝑛( 𝑝_(𝑘,𝑛) * (𝒙_𝑛−𝝁_𝑘 )^(2) )  ) / (∑_𝑛( 𝑝_(𝑘,𝑛) )


////////////////////NOTES END//////////////////////////////////////////////
*/

/////GLOBAL VAR////////

static const int n_numData = 6;
static const int k_clusters = 2;
int update_iteration = 0;
vector<float> data_set{ 1.6, 2.1, 1.8, 5.3, 6.7, 4.7 };
float weight[k_clusters];
float stand_dev[k_clusters];
float mean[k_clusters];

/////FUNCTION DEFINITIONS//////
float Prob_Dens_Fx_GaussianDistr_1D(float mean, float stan_dev, float x);
void EM_Algorithm(void);

///MAIN FUNCTION////
int main()
{


	//Initialize k clusters:

	//notice weight_1 + weight_2 = 1 = 100%, since the sum of all weights from a weighted average should of course = 1;
		//probability of all data_set points depends on distr function of all clusters, which all clsuters have some weight which sum to 1:

	weight[0] = 0.4, mean[0] = 1, stand_dev[0] = sqrt(2); //soft cluster k1
	weight[1] = 0.6, mean[1] = 2, stand_dev[1] = sqrt(2); //soft cluster k2

	cout << "Welcome to the EM (Exepectation Maximization Aglorithm) Program, by Demetrius Johnson meech"
		"\nThis is to demonstrate the algorithm and answer HW4_Q7 for UM-Dearborn CIS-479 SUMMER II w/Dr. Shengquan Wang";
	cout << "\n\n***NOTICE: As you run more and more iterations, the weights for each soft cluster will converge to 1/num_clusters,\nAND the mean and standard deviation of each soft cluster will converge to some unique values***\n\n";

	//run algorithm
	EM_Algorithm();

	int userChoice;
	cout << "Enter a postiive integer to say the number of EM updates you would like to perform; enter a negative integer to exit: ";
	cin >> userChoice;

	while (userChoice >= 0) {
		for (int i = 0; i < userChoice; i++)
			EM_Algorithm();
		cout << "Enter a postiive integer to say the number of EM updates you would like to perform; enter a negative integer to exit: ";
		cin >> userChoice;
	}
	
	
	cout << "\n\nUser Choice is a negative integer: " << userChoice << " ...EXITING PROGRAM...\n\n";
	system("pause");
	return 0;

}


///FUNCTION DECLARATIONS/////

float Prob_Dens_Fx_GaussianDistr_1D(float mean, float stan_dev, float x) {

	//-- > for 1 dimension(d = 1), the Gaussian Distribution Function simplifies to :
	//P(x | µ, 𝜮) = 𝑁(𝝁, 𝜮)(𝒙) = (exp⁡((−1 / 2) * (𝒙−𝝁) ^ (2) * Σ ^ (-2))) / sqrt( (2𝜋) * 𝜮 ^ (2) )

	return 
		
		(
			exp
			(
				( (float)-1 / (float)2 ) * pow( (x - mean), 2 ) * pow(stan_dev, -2)
			)
		) 
		
		/ 
		
		(
			sqrt
			(
				((float)2 * M_PI) * pow(stan_dev, 2)
			)
		);
}

void EM_Algorithm(void) {

	cout << "\n\nNumber of soft clusters = 2:\n\nweight_1 = " << weight[0] << ", mean_1 = " << mean[0] << ", stan_dev_1 = " << stand_dev[0];
	cout << "\nweight_2 = " << weight[1] << ", mean_2 = " << mean[1] << ", stan_dev_2 = " << stand_dev[1];
	cout << "\n\nSample Data is:\n";
	for (int n = 0; n < n_numData; n++)
		cout << "\t" << data_set[n] << endl;

	cout << "\n\n---------------------------E-step for Iteration#" << update_iteration << ":-----------------------------\n\n";
	//E-step: Obtain 𝑝_(𝑘,𝑛) for each data_set item 𝒙_𝑛 in soft cluster k.
//	-- > 𝑝_(𝑘, 𝑛) = P(C = k│𝒙_𝑛), which is proportional to P(𝒙_𝑛│C = k)* P(C = k) = 𝑁(𝝁_𝑘, 𝜮_𝑘)(𝒙_𝑛) * 𝜋_𝑘
	//where 𝑁(𝝁_𝑘, 𝜮_𝑘)(𝒙_𝑛) * 𝜋_𝑘 is a weighted Gaussian source k --> P(𝒙_𝑛│C = k) = 𝑁(𝝁_𝑘, 𝜮_𝑘)(𝒙_𝑛), and P(C=k) = 𝜋_𝑘

		//declare n given k 2D float vector (2D array)
	vector<vector<float>> probability_n_given_k(k_clusters, vector<float>(n_numData));

	//find P(x_n | k) for all points of each weighted Gaussian source k
	for (int k = 0; k < k_clusters; k++)
		for (int n = 0; n < n_numData; n++)
			probability_n_given_k[k][n] = weight[k] * Prob_Dens_Fx_GaussianDistr_1D(mean[k], stand_dev[k], data_set[n]);

	cout << "\nFind P(x_n|k) = weight_k*N(u_k,sigma_k)(x_n) for each point of each weighted Gaussian source k:\n\n";
	for (int k = 0; k < k_clusters; k++) {
		cout << "Data Points\t" << "Source " << k << " (mean=" << mean[k] << " stand_dev=" << stand_dev[k] << " weight=" << weight[k] << ")" << "-->P(x_n|k):\n";
		for (int n = 0; n < n_numData; n++)
			cout << setprecision(5) << "\t" << data_set[n] << "\t\t\t\t" << probability_n_given_k[k][n] << endl;
		cout << endl;
	}
	//now do Normalization for each data_set point to find  𝑝_(𝑘, 𝑛) = P(C = k│𝒙_𝑛) = 𝑝_(𝑘, 𝑛) / (∑_𝑘(𝑝_(𝑘, 𝑛))

		//declare k given n 2D float vector (2D array)
	vector<vector<float>> probability_k_given_n(k_clusters, vector<float>(n_numData));

	//find  𝑝_(𝑘, 𝑛) = P(C = k│𝒙_𝑛) = 𝑝_(𝑘, 𝑛) / (∑_𝑘(𝑝_(𝑘, 𝑛)) for all data_set points n in each cluster

	for (int k = 0; k < k_clusters; k++) {
		for (int n = 0; n < n_numData; n++) {

			float sum_P_n_given_k = 0;
			for (int kluster = 0; kluster < k_clusters; kluster++)
				sum_P_n_given_k += probability_n_given_k[kluster][n];

			probability_k_given_n[k][n] = probability_n_given_k[k][n] / sum_P_n_given_k;

			sum_P_n_given_k = 0; //reset sum for next loop

		}
	}

	cout << "\nFind P(C = k|x_n) = P(x_n|k) / SUM__x_n,k(P(x_n|k)) for each point of each weighted Gaussian source k:\n\n";
	for (int k = 0; k < k_clusters; k++) {
		cout << "Data Points\t" << "Source_" << k << " (mean=" << mean[k] << " stand_dev=" << stand_dev[k] << " weight=" << weight[k] << ")" << "-->P(C = k|x_n):\n";
		for (int n = 0; n < n_numData; n++)
			cout << setprecision(5) << "\t" << data_set[n] << "\t\t\t\t" << probability_k_given_n[k][n] << endl;
		cout << endl;
	}

	cout << "\n\n---------------------------M-step for Iteration#" << update_iteration << ":-----------------------------\n\n";
	//M-step: refit each soft cluster k to the entire data_set set with each data_set weighted by 𝑝_(𝑘, 𝑛), where
		//𝜋_𝑘 = weight of k, 𝝁_𝑘 = mean of k, 𝜮_𝑘 = variance of k.

		//update weights for all k clusters
		//-- > 𝜋_𝑘 = (∑_𝑛(𝑝_(𝑘, 𝑛)) / 𝑁 

	for (int k = 0; k < k_clusters; k++) {

		float sum_P_k_given_n = 0;

		for (int n = 0; n < n_numData; n++)
			sum_P_k_given_n += probability_k_given_n[k][n];

		weight[k] = sum_P_k_given_n / (float)n_numData;

		sum_P_k_given_n = 0; //reset sum for next loop

	}


	//update mean for all clusters k
	//-- > 𝝁_𝑘 = ( ∑_𝑛( 𝑝_(𝑘,𝑛) * 𝒙_𝑛 ) ) / (  ∑_𝑛( 𝑝_(𝑘,𝑛) )  )

	for (int k = 0; k < k_clusters; k++) {

		float sum_P_k_given_n_TIMES_x = 0;
		float sum_P_k_given_n = 0;


		for (int n = 0; n < n_numData; n++) {

			sum_P_k_given_n += probability_k_given_n[k][n];
			sum_P_k_given_n_TIMES_x += (probability_k_given_n[k][n] * data_set[n]);
		}

		mean[k] = sum_P_k_given_n_TIMES_x / sum_P_k_given_n;

		//reset sum variables for next iteration
		sum_P_k_given_n_TIMES_x = 0;
		sum_P_k_given_n = 0;
	}

	//update variance for all clusters k
	//-- > 𝜮_𝑘 = (  ∑_𝑛( 𝑝_(𝑘,𝑛) * (𝒙_𝑛−𝝁_𝑘 )^(2) )  ) / (∑_𝑛( 𝑝_(𝑘,𝑛) )

	for (int k = 0; k < k_clusters; k++) {

		float sum_P_k_given_n_TIMES_sq_error = 0;
		float sum_P_k_given_n = 0;


		for (int n = 0; n < n_numData; n++) {

			sum_P_k_given_n += probability_k_given_n[k][n];
			sum_P_k_given_n_TIMES_sq_error += (probability_k_given_n[k][n] * pow((data_set[n] - mean[k]), 2));
		}

		//we add in sqrt function since this function calculates variance; thus, standard deviation sqrt(variance)
		stand_dev[k] = sqrt(sum_P_k_given_n_TIMES_sq_error / sum_P_k_given_n);

		//reset sum variables for next iteration
		sum_P_k_given_n_TIMES_sq_error = 0;
		sum_P_k_given_n = 0;
	}


	cout << "*******New weight, mean, and standard deviation values after M-Step update iteration#" << update_iteration << "**********\n\n";
	for (int k = 0; k < k_clusters; k++) {

		cout << "Gaussian_Source_" << k << endl;
		cout << "weight: " << weight[k] << endl;
		cout << "mean: " << mean[k] << endl;
		cout << "standard deviation: " << stand_dev[k] << endl;
		cout << "variance = (standard deviation)^2: " << pow(stand_dev[k], 2) << endl << endl;
	}

	cout << "\n\n////////////////////////////////END OF EM ALGORITHM ITERATION#" << update_iteration << "////////////////////////////////////////////////////\n\n";
	update_iteration++;


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
