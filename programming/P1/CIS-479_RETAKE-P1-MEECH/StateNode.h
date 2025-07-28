
//Author: Demetrius E Johnson
//Purpose: 
//Date Created: 7/15/22
//Date Modified: 



#ifndef STATENODE_H
#define STATENODE_H
#include <string>
#include <stdlib.h>
#include "FIFO.h"
class StateNode
{
public:
	int row_location;
	int col_location;
	int heuristicVal;
	int pathCost;
	int EvalFunction;
	StateNode* parent;
	std::string hash_string;

	FIFO tie_breaker; //use this variable to keep track of redundant expansions and maintain FIFO among tie breaker nodes

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