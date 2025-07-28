
//Author: Demetrius E Johnson
//Purpose: create a min heap priority queue template class so I can use it for my MST program and for future uses
//Date Created: 7/25/21
//Date Modified: 9/19/22
//New adaptation: added functions to support my CIS-479 P1 windy maze puzzle A* search program

#ifndef FIFO_H
#define FIFO_H


struct FIFO {

	int evalFx_val;
	int insert_order_val;

	//default constructor
	FIFO() {
	
		evalFx_val = -1;
		insert_order_val = -1;
	
	}

	//operator overloads
	bool operator== (const FIFO& RHoperand) const {

		if (evalFx_val == RHoperand.evalFx_val)
			return true;
		else
			return false;
	}
	bool operator!= (const FIFO& RHoperand) const {

		if (evalFx_val != RHoperand.evalFx_val)
			return true;
		else
			return false;
	}

	FIFO& operator= (const FIFO& RHoperand) {

		evalFx_val = RHoperand.evalFx_val;
		insert_order_val = RHoperand.insert_order_val;

		return *this;
	}
};

#endif
