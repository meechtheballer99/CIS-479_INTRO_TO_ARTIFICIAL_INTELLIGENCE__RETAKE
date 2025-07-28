//Author: Demetrius E Johnson
//Purpose: 
//Date Created: 7/15/22
//Date Modified: 


#include "StateNode.h"

StateNode::StateNode() {

	heuristicVal = -1;
	pathCost = -1;
	EvalFunction = -1;
	hash_string.resize(9);
	parent = nullptr; //pointer works so that you can trace back to root; but rehash function causes this program to fail; safegaurd placed in main program.

}
void StateNode::setEval_value(void) { EvalFunction = (pathCost + heuristicVal); }

//convert table values for a given node to a uniquely ordered string value
//so that it can be used in the hash function (a part of the Quadratic Probing class)
void StateNode::setHash_string(void) {

	
	int string_location = 0;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {

			hash_string[string_location] = table[i][j] + 48;  //ASCII characters '0' - '9' begin at decimal 48
			string_location++;
		}
	}
}

/////OPERATOR OVERLOADING///////
bool StateNode::operator< (const StateNode& RHoperand) {

	if (EvalFunction < RHoperand.EvalFunction)
		return true;
	else
		return false;
}
bool StateNode::operator> (const StateNode& RHoperand) {

	if (EvalFunction > RHoperand.EvalFunction)
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

	heuristicVal = RHoperand.heuristicVal;
	pathCost = RHoperand.pathCost;
	EvalFunction = RHoperand.EvalFunction;
	hash_string = RHoperand.hash_string;
	blankTile_row_location = RHoperand.blankTile_row_location;
	blankTile_col_location = RHoperand.blankTile_col_location;
	parent = RHoperand.parent;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			table[i][j] = RHoperand.table[i][j];
		}
	}

	return *this;
}


