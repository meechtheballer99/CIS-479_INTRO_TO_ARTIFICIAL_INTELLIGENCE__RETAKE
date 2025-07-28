//Hash Table function provided by Professor Dr. Junhua Guo -- UM-DEARBORN -- 
//Modfied by Demetrius Johnson --for CIS-350 Summer 2021 and CIS-479 Summer 2022
//this version of the class was adapted for the Windy Puzzle Program 1 for CIS-479 with Doctor Shenquan Wang
#ifndef QUADRATIC_PROBING_H
#define QUADRATIC_PROBING_H

#include <vector>
#include <string>
#include "StateNode.h"
using namespace std;

int nextPrime( int n );
int hash1( const string & key );
int hash1( int key );
int hash1(const StateNode& key);
// QuadraticProbing Hash table class
//
// CONSTRUCTION: an approximate initial size or default of 101
//
// ******************PUBLIC OPERATIONS*********************
// bool insert( x )                     --> Insert x
// HashedObj* getOBJ_reference( x )     --> return reference to hashed OBJ if in table
// bool remove( x )                     --> Remove x
// bool contains( x )                   --> Return true if x is present
// void makeEmpty( )                    --> Remove all items
// int hash( string str )               --> Global method to hash strings
// int get_num_rehash_fx_called( void ) --> return number of times rehash fx called


template <typename HashedObj>
class HashTable
{
  public:
    explicit HashTable( int size = 101 ) : array( nextPrime( size ) )
      { makeEmpty( ); }

    bool contains( const HashedObj & x ) const
    {
        //to check, run hash function for object to get hash value, then check if matching hash value AND object is in table.
        //quadratic probing: may require you to check several locations in hash table; you need both hash value and object stored at a location to match and location is ACTIVE
        //example 1): hash value matches, ACTIVE, but wrong object at location - apply quadratic probe function to check next location.
        //example 2): hash value matches, DELETED, but wrong object at location - apply quadratic probe function to check next location.
        //example 3): hash value matches, ACTIVE, and object matches; return true: object is in the table.
        //examnple 4): hash value matches, DELETED, and object matches; return false: object was in table, but not anymore.
        return isActive( findPos( x ) ); 
    }

    void makeEmpty( )
    {
        currentSize = 0;
        for( int i = 0; i < array.size( ); i++ )
            array[ i ].info = EMPTY;
    }

    bool insert( const HashedObj & x )
    {
            // Insert x as active
        int currentPos = findPos( x );
        if( isActive( currentPos ) )
            return false;

        array[ currentPos ] = HashEntry( x, ACTIVE );

            // Rehash; see Section 5.5
        if( ++currentSize > array.size( ) / 2 )
            rehash( );

        return true;
    }

    int get_num_rehash_fx_called(void) {

        return num_rehash_calls;

    }

    bool remove( const HashedObj & x )
    {
        int currentPos = findPos( x );
        if( !isActive( currentPos ) )
            return false;

        array[ currentPos ].info = DELETED;
        return true;
    }
    HashedObj* getOBJ_reference(const HashedObj & x)
    {
        if ( contains( x ) )
        {
            int tableLocation = findPos( x );
            return & array[ tableLocation ].element;
        }
        else
            return nullptr;
    }
    enum EntryType { ACTIVE, EMPTY, DELETED };

  private:
    struct HashEntry
    {
        HashedObj element;
        EntryType info;

        HashEntry( const HashedObj & e = HashedObj( ), EntryType i = EMPTY )
          : element( e ), info( i ) { }
    };

    vector<HashEntry> array;
    int currentSize;

    //track number of times rehash function was called. Useful for many other functions
    // -- esp adapting or correcting pointer race conditions for when 
    //rehash causes vector to resize casuing ptrs to point to bad memory
    int num_rehash_calls = 0; 

    bool isActive( int currentPos ) const
      { return array[ currentPos ].info == ACTIVE; }

    //notice const after function definition means that all elements from the this-> object which calls findPos function is const qualified;
    //therefore, array is a member of the calling object (this->object), and is const qualified.
    int findPos( const HashedObj & x ) const //findPos == find position to insert the hashed object; use quadratic probing insert method
    {
        int offset = 1;
        int currentPos = myhash( x );

          // Assuming table is half-empty, and table length is prime,
          // this loop terminates
        while( array[ currentPos ].info != EMPTY &&
                array[ currentPos ].element != x )
        {
                // Compute ith probe
                 //quadratic probing; simplified version of f(i^2); derived from f(i) - f(i-1), 
                 //which shows offset is increased by 2 + previous-offset value to get quadratic probe i^2 value
                 //cpu can process arithmetic much faster and efficiently than multiplication operations

            currentPos += offset;
            offset += 2;
            if( currentPos >= array.size( ) )
                currentPos -= array.size( );
        }

        return currentPos;
    }
    void rehash( ) //standard, generic re-hash function used by the default template of this quadratic probing class/program
    {
        num_rehash_calls++;
        vector<HashEntry> oldArray = array;

            // Create new double-sized, empty table; size needs to be a prime number for hash function to be efficient
        array.resize( nextPrime( 2 * oldArray.size( ) ) );
        for( int j = 0; j < array.size( ); j++ )
            array[ j ].info = EMPTY; //now, all DELETED (unused) buckets will be "freed" and availble for use as they will be set to EMPTY now

            // Copy table over
        currentSize = 0;
        for( int i = 0; i < oldArray.size( ); i++ )
            if( oldArray[ i ].info == ACTIVE )
                insert( oldArray[ i ].element );
    }
    int myhash( const HashedObj & x ) const
    {
        int hashVal = hash1( x );

        hashVal %= array.size( );
        if( hashVal < 0 )
            hashVal += array.size( );

        return hashVal;
    }
};

#endif
