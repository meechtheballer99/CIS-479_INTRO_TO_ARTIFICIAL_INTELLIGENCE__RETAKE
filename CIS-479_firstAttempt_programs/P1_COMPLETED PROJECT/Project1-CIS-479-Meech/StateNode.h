
//Author: Demetrius E Johnson
//Purpose: 
//Date Created: 7/15/22
//Date Modified: 



#ifndef STATENODE_H
#define STATENODE_H
#include <string>
#include <stdlib.h>

class StateNode
{
public:
	int heuristicVal;
	int pathCost;
	int EvalFunction;
	int table[3][3];
	int blankTile_row_location;
	int blankTile_col_location;
	StateNode* parent;
	std::string hash_string;

	StateNode();
	void setEval_value(void);
	void setHash_string(void);
	bool operator< (const StateNode& RHoperand);
	bool operator> (const StateNode& RHoperand);
	bool operator== (const StateNode& RHoperand) const;  //needed to make the == and != operator const qualify calling object (this->object) since Quadratic Probing findPos function uses const for calling obj
	bool operator!= (const StateNode& RHoperand) const;
	StateNode& operator= (const StateNode& RHoperand);
};





#endif // !1