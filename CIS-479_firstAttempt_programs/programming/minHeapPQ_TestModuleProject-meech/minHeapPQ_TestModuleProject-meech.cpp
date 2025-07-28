// minHeapPQ_TestModuleProject-meech.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
//Author: Demetrius E Johnson
//Purpose: test the minHeapPQ library module so that I can make sure it can be integrated with other programs
//Date Created: 7/21/22
//Date Modified: 7/21/22


 //note: minHeapPQ is a template class. There are two solutions to avoid FATAL LINK ERROR:
//1) Place do not include cpp file in the project, only include the header, and add #include .cpp at end of header file, 
//then just #include the .h file in the main program source file.
//2)just #include the .h and the cpp files of the header in the main program source file.
//**for this sitation, I tested both cases (they both work!), and decided to go with option 1 for this test project.
#include "minHeapPQ.h"
#include <iostream>
#include <stdlib.h>
using namespace std;

minHeapPQ<int> pq;
vector<int> test;
int main()
{

	for (int i = 0; i < 15; i++)
		test.push_back(i);

	//test inserting numbers 0 to 14 and then popping them:
	cout << "\ntest inserting numbers 0 to 14 and then popping them:\n";
	for (int i = 0; i < 15; i++)
		pq.insert(test[i]);

	for (int i = 0; i < 15; i++)
		cout << pq.deleteMin() << endl;


	//test inserting numbers 14 to 0 and then popping them:
	cout << "\ntest inserting numbers 14 to 0 and then popping them:\n";
	for (int i = 14; i >= 0; i--)
		pq.insert(test[i]);

	for (int i = 14; i >= 0; i--)
		cout << pq.deleteMin() << endl;


	//test inserting 14 elements using i/2 and then popping them:
	cout << "\ntest inserting 14 numbers using i/2 and then popping them:\n";
	for (int i = 14; i >= 0; i--)
		pq.insert(test[i / 2]);

	for (int i = 14; i >= 0; i--)
		cout << pq.deleteMin() << endl;

	//test inserting 14 elements using i/3 and then popping them:
	cout << "\ntest inserting 14 numbers using i/3 and then popping them:\n";
	for (int i = 14; i >= 0; i--)
		pq.insert(test[i / 3]);

	for (int i = 14; i >= 0; i--)
		cout << pq.deleteMin() << endl;

	//test inserting numbers randomnly and then popping them:
	test.clear();
	for (int i = 0; i < 15; i++)
		test.push_back(i + rand());

	cout << "\ntest inserting numbers randomnly and then popping them:\n";
	for (int i = 14; i >= 0; i--)
		pq.insert(test[i]);

	for (int i = 14; i >= 0; i--)
		cout << pq.deleteMin() << endl;

	//test inserting numbers randomnly and then popping them:
	test.clear();
	for (int i = 1; i < 16; i++)
		test.push_back((rand() + 1) % i); //rand() random number generator function (from stdlib.h) can generate 0, so when using % or /, need to add one to the function first.

	cout << "\ntest inserting numbers randomnly and then popping them:\n";
	for (int i = 14; i >= 0; i--)
		pq.insert(test[i]);

	for (int i = 14; i >= 0; i--)
		cout << pq.deleteMin() << endl;

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
