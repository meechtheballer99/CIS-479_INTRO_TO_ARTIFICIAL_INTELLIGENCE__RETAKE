
//Author: Demetrius E Johnson
//Purpose: creat a min heap priority queue template class so I can use it for my MST program and for future uses
//Date Created: 7/25/21
//Date Modified: 7/21/22
//New adaptation: added functions to support my CIS-479 P1 windy maze puzzle A* search program
//As of 7/21/22: have fully tested this module through a test module program (minHeapPQ_testModule_prj). No known issues. Works as expected.



 //note: minHeapPQ is a template class. There are two solutions to avoid FATAL LINK ERROR:
//1) Place do not include cpp file in the project, only include the header, and add #include .cpp at end of header file, 
//then just #include the .h file in the main program source file.
//2)just #include the .h and the cpp files of the header in the main program source file.
//**for this situation, I tested both cases (they both work!), and decided to go with option 1 for this test project.

#ifndef MINHEAPPQ_H
#define MINHEAPPQ_H
#include <vector>
//pq using an array
//min will always be at root
//children will be greater than their parent
//i has children 2i and 2i+1 (left and right child, respectively)
//i has parent i/2
//start of a priority queue using a heap structure is at index i = 1 (not index 0; index 0 is place holder)


template<typename T>
class minHeapPQ
{

private:
	std::vector<T> heapArray;		//store heap in this array
	std::vector<T>* vectorPtr;		//use this to point to a specific element in the heap in order to perform swap operations and track parent/child
	int currentPos;					//the current element position (i)
	int parent;						//keep track of a given elements parent (i/2)
	int leftChild;					//keep track of a given elements left child (2*i)
	int rightChild;					//keep track of a given elements right child (2*i + 1)
	T nullObject;					//use this to return null objects and to set first element in heap as a place holder

public:
	minHeapPQ();					//default constructor
	void insert(T insertElement);	//insert elements into PQ heap array
	T deleteMin(void);				//delete the root; return the element that was stored at the deleted root
	bool isEmpty(void);				//determine if queue is empty or not
	T currentMin(void);				//return  a copy the current minimum of the heap without popping it from the heap; returns default constructor copy if heap is empty.
	//perhaps add some heap initialization algorithm to this template class I have written in the future to make this class even more useful!
};

//#include "minHeapPQ.cpp" //because this is a template class; we need this include after the class declaration and to exclude the .cpp file from the project for the program to compile proprly
						//Alternative method: If I would have kept the .cpp included, 
						//I would need to remove this include and simply add it to the file using the .h include of this class but also add the .cpp back to the project
#include "minHeapPQ.cpp"
#endif 


 //note: minHeapPQ is a template class. There are two solutions to avoid FATAL LINK ERROR:
//1) Place do not include cpp file in the project, only include the header, and add #include .cpp at end of header file, 
//then just #include the .h file in the main program source file.
//2)just #include the .h and the cpp files of the header in the main program source file.
//**for this situation, I tested both cases (they both work!), and decided to go with option 1 for this test project.