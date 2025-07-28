
//Author: Demetrius E Johnson
//Purpose: creat a min heap priority queue template class so I can use it for my MST program and for future uses
//Date Created: 7/25/21
//Date Modified: 8/02/21

#include "minHeapPQ.h"

//pq using an array
//min will always be at root
//chidlren will be greater than their parent
//i has children 2i and 2i+1 (left and right child, respectively)
//i has parent i/2
//start of a priority queue using a heap structure is at index i = 1 (not index 0; index 0 is place holder)
//note: since start of heap is nullObject placeholder (index 0 = null element), then size of heap == will always return 1; thus true heap size is heapSize - 1

template<typename T>
minHeapPQ<T>::minHeapPQ() {

	vectorPtr = &heapArray;
	parent = -1;
	leftChild = -1;
	rightChild = -1;
	currentPos = -1;
	heapArray.push_back(nullObject); //element 0 is a placeholder holding a T object that will be ignored /treated as null

}

template<typename T>
void minHeapPQ<T>::insert(T insertElement) {

	//remember, element 1 is a placeholder; so size == 1 means element 0 == nullObject
	if (heapArray.size() == 1) { //case: first element to be added to queue; simply add it and exit function

		heapArray.push_back(insertElement);
		return;
	}
	else {//case: size of heap array > 1:

		//insert the element at the end and initialize the parent and children values relative to the size of the heap upon calling the function:
		heapArray.push_back(insertElement);	//insert 
		currentPos = heapArray.size() - 1;	//track current position of inserted element

		//acquire and keep track of the parent of the inserted element:

		if ((heapArray.size() - 1) % 2 == 0)//if last element location is even, then do reverse left child function to find parent since (any odd or even number) * 2 == even number
			parent = currentPos / 2;
		else
			parent = (currentPos - 1) / 2;  //else, last element location is odd, then do the reverse right child function to find parent since (any even or odd number) * 2 + 1 == odd number


		while (heapArray.at(currentPos) < heapArray.at(parent)) {	//check: is the parent greater than the child? if so: swap the parent and child so that the smaller child is moved up in the queue / PQ tree

			//swap operation (bubble min value up the array/heap):

			T parentHolder = heapArray[parent];				//store the parent
			heapArray[parent] = heapArray[currentPos];		//overwrite parent with the child
			heapArray[currentPos] = parentHolder;			//overwrite child with the parent
			currentPos = parent;							//new location of inserted element is now at the location that the parent was at (swap operation completed.)

			//now update parents in the event that loop needs to run again and bubble up the inserted element another level:

			if (currentPos % 2 == 0)//currentPos == evem. then do reverse left child function to find parent since (any odd or even number) * 2 == even number
				parent = currentPos / 2;
			else
				parent = (currentPos - 1) / 2;  //else, currentPos == odd, then do the reverse right child function to find parent since (any even or odd number) * 2 + 1 == odd number

			if (currentPos == 1) { return; }				//element has been moved to root; has no parent; exit function as there is no more swaps possible
		}
		//loop will exit when inserted element is in its correct position in the min heap
	}
	return; //element inserted...exit function
}

template<typename T>
T minHeapPQ<T>::deleteMin(void) {

	if (heapArray.size() == 1) { return nullObject; }	//case: exit function if queue is empty; no minimum to pop and delete


	T minVal = heapArray[1];							//set minVal to the root (element 1), which stores the min value in the heap, so that we can return it
	heapArray[1] = heapArray[heapArray.size() - 1];		//overwrite front of array (root of PQ tree) with the last element in the array; technique to avoid having to resize array (which is an O(N) operation)
	heapArray.erase(heapArray.end() - 1);				//remove last element: vector.erase() function will not resize capacity of array as long as elements from the end are deleted; array size decreased by 1
	bool noRightChild = false;							//need the no right or left child check for the swap while loop 
	bool noLeftChild = false;


	//now we must keep the tree a complete binary tree; thus we have to adjust the element moved to the root down the tree as necessary (walk down the tree by doing swaps -> O(logN)):

	//root = element 1; left child = 2*1 = 2 and right child = 2*1 +1 = 3
	currentPos = 1;							//current position of element (1 = root) that will be walked down the heap to keep tree as a complete binary tree
	leftChild = 2 * currentPos;				//starting left child position
	rightChild = (2 * currentPos) + 1;		//starting right child position

	//switch statement only allows variable initialization outside of itself or in the default section only
	T minSwapElement;
	int swapLocation;
	switch (int heapSize = heapArray.size()) {

	case 1:	//element already popped from queue; thus if size is 1 then that means only the place holder nullobject is in the queue; simply return minVal
	case 2: //there were only 2 elements in the queue (array size == 3); after the min element was deleted, then there is only 1 element in the queue; no swaps necessary; simply return minVal
		return minVal;
	case 3: //after deletion, size of array == 3; thus there are only 2 elements in the queue, we simply need to compare these two elements and determine if a swap is necessary
		rightChild = 1;//set right child == root so that it is not used in the comparison for default case
	default:
		while //if there exists a child larger than parent...
		(heapArray[currentPos] > heapArray[leftChild] || heapArray[currentPos] > heapArray[rightChild])	
		{
			
			//first, check if no right or left child; default swap element and 
			//location to compare will be whichever is the only child that exists.
			//else do: if left < right, assign swap element that left value, otherwise,
			//assign it the right value (the smallest between the two which are both smaller than their parent):
			if (noLeftChild) {
				minSwapElement = heapArray[rightChild];
				swapLocation = rightChild;
			}
			else if (noRightChild) {
				minSwapElement = heapArray[leftChild];
				swapLocation = leftChild;
			}
			else {
				minSwapElement = (heapArray[leftChild] < heapArray[rightChild]) ? heapArray[leftChild] : heapArray[rightChild];
				swapLocation = (heapArray[leftChild] < heapArray[rightChild]) ? leftChild : rightChild;
			}
			//perform swap:
			heapArray[swapLocation] = heapArray[currentPos];	//smallest child assigned to parent value
			heapArray[currentPos] = minSwapElement;				//parent value assigned to smallest child

			//next lines: set new currentPos and the respective children for the next while loop (if necessary):
			currentPos = swapLocation;

			//only change child location if it exists (don't go out of bounds in the array used for the heap/queue); 
			//otherwise set their value to 1 (the root == smallest value) so that in next iteration of loop they will essentially be ignored
			//remember: we do size - 1 since first element in array/heap is simply a placeholder:

			if (heapArray.size() - 1 >= 2 * currentPos) {
				leftChild = 2 * currentPos; 
				noLeftChild = false;
			}
			else { noLeftChild = true; }

			if (heapArray.size() - 1 >= (2 * currentPos) + 1) { 
				rightChild = (2 * currentPos) + 1; 
				noRightChild = false;
			}
			else { noRightChild = true; }


			//case: no more children for the currentPos to compare/swap with; tree balanced; exit loop
			if (noLeftChild && noRightChild) { break; }

		}
	}

	return minVal;	//tree is now balanced; we can return the deleted root / minValue in the queue (front of queue)
}

template<typename T>
bool minHeapPQ<T>::isEmpty(void) {

	//element 0 is a placeholder; thus if size == 1 then the heap is considered empty
	if (heapArray.size() == 1) { return true; }
	if (heapArray.size() > 1) { return false; }
	//I did both if statements to ensure function/heap class is functioning properly; size should never go below 1 (place holder always occupies element 0)
}

//return  a copy the current minimum of the heap without popping it from the heap. Returns default constructor copy if heap is empty.
template<typename T>  
T minHeapPQ<T>::currentMin(void) {

	T copy;
	if (heapArray.size() > 1)
		copy = heapArray[1]; //remember, for this minHeap library, element 0 has a place holder object; first element == min == element 1

	return copy;

}


//note: minHeapPQ is a template class. There are two solutions to avoid FATAL LINK ERROR:
//1) Place do not include cpp file in the project, only include the header, and add #include .cpp at end of header file, 
//then just #include the .h file in the main program source file.
//2)just #include the .h and the cpp files of the header in the main program source file.
//**for this situation, I tested both cases (they both work!), and decided to go with option 1 for this test project.