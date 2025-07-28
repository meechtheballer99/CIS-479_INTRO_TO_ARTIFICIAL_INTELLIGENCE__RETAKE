// CIS-479_HW4_Q1_MarkovChain_Meech.cpp : This file contains the 'main' function. Program execution begins and ends there.
//Author: Demetrius Johnson
// Purpose: Find probabilities from a Markov Chain data model; for CIS 479 SUMMER II With Dr. Shengquan Wang
//Date Created: 8/13/22
//Date Modified: 8/15/22


/*


We consider a 3-state (a, b, c) Markov Chain. 

We observe the following sequence of data: 
b a a b a b c c b c a b c c a b c c a c b c a b c c b b b a b c. (# of data points collected = 32).

Please use MLE and Laplace Smoothing with k = 2 to learn the following transition probabilities: 
P(a|a), P(b|a), P(c|a), P(a|b), P(b|b), P(c|b), P(a|c), P(b|c), P(c|c).


MLE of 𝑝_𝑚 under Laplace Smoothing with k:
𝑝_𝑚^∗=(𝑁_𝑚+𝑘)/(𝑁+𝑘∗𝑀)
𝑁_𝑚 is the occurrence of data in Category m. 
𝑁 is the total number of data items.


*/

#include <iostream>
#include <vector>
#include <string>
using namespace std;

int main()
{
	cout << "\n HW4_Q1: Find probabilities from a Markov Chain data model; for CIS 479 SUMMER II With Dr. Shenguan Wang\n--BY Demetrius Johnson\n\n";

	string data_string = "b a a b a b c c b c a b c c a b c c a c b c a b c c b b b a b c";

	//delete all spaces in the data squence:
	for (int i = 0; i < data_string.size(); i++) {
	
		if (data_string[i] == ' ')
			data_string.erase(data_string.begin() + i); //.begin() function returns an iterator data type so that we can delete char at a given position in the array using overloaded erase(iterator) version of the function
	}

	float a_a = 0, a_b = 0, a_c = 0, 
		b_a = 0, b_b = 0, b_c = 0, 
		c_a = 0, c_b = 0, c_c = 0;
	
	//now get total number of occurences for each type of sequence:

	for (int i = 0; i < data_string.size() - 1; i++) { //do size - 1 because last data in sequence has no data after itself

		
		switch (data_string[i]) {
		
			
		case 'a':
			switch (data_string[i + 1]) {


			case 'a':
				a_a++;
				break;

			case 'b':
				a_b++;
				break;

			case 'c':
				a_c++;
				break;

			}
			break;

		case 'b':
			switch (data_string[i + 1]) {


			case 'a':
				b_a++;
				break;

			case 'b':
				b_b++;
				break;

			case 'c':
				b_c++;
				break;

			}
			break;

		case 'c':
			switch (data_string[i + 1]) {


			case 'a':
				c_a++;
				break;

			case 'b':
				c_b++;
				break;
			case 'c':
				c_c++;
				break;

			}
			break;

		}


	}


	cout << "Number of a to a transitions: " << a_a << endl;
	cout << "Number of a to b transitions: " << a_b << endl;
	cout << "Number of a to c transitions: " << a_c << endl;
	cout << "SUM -->Number of a to X transitions: " << a_a + a_b + a_c << endl;
	cout << "Number of b to a transitions: " << b_a << endl;
	cout << "Number of b to b transitions: " << b_b << endl;
	cout << "Number of b to c transitions: " << b_c << endl;
	cout << "SUM -->Number of b to X transitions: " << b_a + b_b + b_c << endl;
	cout << "Number of c to a transitions: " << c_a << endl;
	cout << "Number of c to b transitions: " << c_b << endl;
	cout << "Number of c to c transitions: " << c_c << endl;
	cout << "SUM -->Number of c to X transitions: " << c_a + c_b + c_c << endl << endl;

	
	/*

	Use Laplace Smoothing, k = 2; M = 3 
	(M--> number of categories for a random variable; a, b or c are possible values for the random variable in this case)
	
	𝑝_𝑚^∗=(𝑁_𝑚+𝑘)/(𝑁+𝑘∗𝑀)
	with k = 2, M = 3

	
	*/

	float P_a_given_a = 0, P_a_given_b = 0, P_a_given_c = 0,
		P_b_given_a = 0, P_b_given_b = 0, P_b_given_c = 0,
		P_c_given_a = 0, P_c_given_b = 0, P_c_given_c = 0;

	int k = 2, M = 3;

	P_a_given_a = (a_a + k) / (a_a + a_b + a_c + k * M);
	P_b_given_a = (a_b + k) / (a_a + a_b + a_c + k * M);
	P_c_given_a = (a_c + k) / (a_a + a_b + a_c + k * M);
	
	P_a_given_b = (b_a + k) / (b_a + b_b + b_c + k * M);
	P_b_given_b = (b_b + k) / (b_a + b_b + b_c + k * M);
	P_c_given_b = (b_c + k) / (b_a + b_b + b_c + k * M);

	P_a_given_c = (c_a + k) / (c_a + c_b + c_c + k * M);
	P_b_given_c = (c_b + k) / (c_a + c_b + c_c + k * M);
	P_c_given_c = (c_c + k) / (c_a + c_b + c_c + k * M);




	cout	<< "k (smoothing factor) = " << k
			<< "\nM (# category of random variable = 3 [a, b, or c]: " << M << endl
			<< "Use P_m = (N_m + k) / (N + k*M)\n\n";
			

	cout << "P(a|a): " << P_a_given_a << endl;
	cout << "P(b|a): " << P_b_given_a << endl;
	cout << "P(c|a): " << P_c_given_a << endl;
	cout << "P(a|b): " << P_a_given_b << endl;
	cout << "P(b|b): " << P_b_given_b << endl;
	cout << "P(c|b): " << P_c_given_b << endl;
	cout << "P(a|c): " << P_a_given_c << endl;
	cout << "P(b|c): " << P_b_given_c << endl;
	cout << "P(c|c): " << P_c_given_c << endl << endl;


	
	
	system("pause");

	return 0;
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
