//Hash Table function provided by Professor Dr. Junhua Guo -- UM-DEARBORN -- 
//Modfied by Demetrius Johnson --for CIS-350 Summer 2021 and CIS-479 Summer 2022 and CIS-479 Fall 2022 
// most recent modification:  9/19/22
//this version of the class was adapted for the Windy Puzzle Program 1 for CIS-479 with Doctor Shenquan Wang
#include "QuadraticProbing.h"
#include <iostream>
using namespace std;

/**
 * Internal method to test if a positive number is prime.
 * Not an efficient algorithm.
 */
bool isPrime( int n )
{
    if( n == 2 || n == 3 )
        return true;

    if( n == 1 || n % 2 == 0 )
        return false;

    for( int i = 3; i * i <= n; i += 2 )
        if( n % i == 0 )
            return false;

    return true;
}

/**
 * Internal method to return a prime number at least as large as n.
 * Assumes n > 0.
 */
int nextPrime( int n )
{
    if( n <= 0 )
        n = 3;

    if( n % 2 == 0 )
        n++;

    for( ; !isPrime( n ); n += 2 )
        ;

    return n;
}

/**
 * A hash routine for string objects.
 */
int hash1( const string & key )
{
    int hashVal = 0;

    for(unsigned int i = 0; i < key.length( ); i++ )
        hashVal = 37 * hashVal + key[ i ];

    return hashVal;
}

/**
 * A hash routine for ints.
 */
int hash1( int key )
{
    return key;
}

/**
 * A hash routine for StateNode objects.
 */
int hash1(const StateNode& key)
{
    int hashVal = 0;
    for (unsigned int i = 0; i < key.hash_string.length(); i++)
        hashVal = 17 * hashVal + key.hash_string[i]; //use a prime number for the multiplication value: i.e. 2, 3, 17, 37, 59, 89, 97
                                                        //if you expect large strings, then use a smaller prime number
    return hashVal;
}

/**
 * A hash routine for FIFO.
 */
int hash1(const FIFO& key)
{
    return key.evalFx_val;
}
