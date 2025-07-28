//Author: Demetrius E Johnson
//Purpose: adapted from my program I wrote in SUMMER II CIS-479 with Dr. Wang
//Date Created: 7/15/22
//Date Modified: 9/19/22
//note: each location in the windy maze is a state.

#include "StateNode.h"

//default constructor
StateNode::StateNode() {
	
	row_location = -1;
	col_location = -1;
	heuristicVal = -1;
	pathCost = -1;
	EvalFunction = -1;
	tie_breaker.evalFx_val = -1;
	tie_breaker.insert_order_val = -1;
	hash_string.resize(2); //I will convert the coordinates of a state into a string of size 2. For example, location (i,j) = (1,2), the string= "12".
	parent = nullptr; //pointer works so that you can trace back to root; but rehash function causes this program to fail; safegaurd placed in main program.

}

//set eval function = path cost + hueristic value
void StateNode::setEval_value(void) { 
	
	
	EvalFunction = (pathCost + heuristicVal);
	tie_breaker.evalFx_val = EvalFunction; //also set this so we can use it for the FIFO hash table tracker
}

//convert table values for a given node to a uniquely ordered string value
//so that it can be used in the hash function (a part of the Quadratic Probing class)
void StateNode::setHash_string(void) {

		hash_string[0] = row_location + 48;  //ASCII characters '0' - '9' begin at decimal 48
		hash_string[1] = col_location + 48;  //ASCII characters '0' - '9' begin at decimal 48

}

/////OPERATOR OVERLOADING///////
bool StateNode::operator< (const StateNode& RHoperand) {

	//standard case:
	if (EvalFunction < RHoperand.EvalFunction)
		return true;
	//check case for when they are equal, now we need to check insert order to maintain FIFO precedence between nodes:
	else if (EvalFunction == RHoperand.EvalFunction && tie_breaker.insert_order_val < RHoperand.tie_breaker.insert_order_val)
		return true;
	else
		return false;
}
bool StateNode::operator> (const StateNode& RHoperand) {

	//standard case:
	if (EvalFunction > RHoperand.EvalFunction)
		return true;
	//check case for when they are equal, now we need to check insert order to maintain FIFO precedence between nodes:
	else if (EvalFunction == RHoperand.EvalFunction && tie_breaker.insert_order_val > RHoperand.tie_breaker.insert_order_val)
		return true;
	else
		return false;
}
bool StateNode::operator== (const StateNode& RHoperand) const {

	if (heuristicVal == RHoperand.heuristicVal)
		return true;
	else
		return false;
}
bool StateNode::operator!= (const StateNode& RHoperand) const {

	if (hash_string != RHoperand.hash_string)
		return true;
	else
		return false;
}
StateNode& StateNode::operator= (const StateNode& RHoperand) {

	row_location = RHoperand.row_location;
	col_location = RHoperand.col_location;
	heuristicVal = RHoperand.heuristicVal;
	pathCost = RHoperand.pathCost;
	EvalFunction = RHoperand.EvalFunction;
	tie_breaker.evalFx_val = RHoperand.tie_breaker.evalFx_val;
	tie_breaker.insert_order_val = RHoperand.tie_breaker.insert_order_val;
	hash_string = RHoperand.hash_string;
	parent = RHoperand.parent;

	return *this;
}



