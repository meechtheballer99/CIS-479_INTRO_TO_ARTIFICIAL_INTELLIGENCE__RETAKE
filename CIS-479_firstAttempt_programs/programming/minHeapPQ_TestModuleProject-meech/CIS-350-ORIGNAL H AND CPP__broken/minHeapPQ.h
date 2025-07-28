

#ifndef MINHEAPPQ_H
#define MINHEAPPQ_H
#include <vector>
//pq using an array
//min will always be at root
//chidlren will be greater than their parent
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
	//perhaps add some heap initialization algorithm to this template class I have written in the future to make this class even more useful!
};

#include "minHeapPQ.cpp" //because this is a template class; we need this include after the class declaration and to exclude the .cpp file from the project for the program to compile proprly
						//Alternative method: If I would have kept the .cpp included, 
						//I would need to remove this include and simply add it to the file using the .h include of this class but also add the .cpp back to the project
#endif 
