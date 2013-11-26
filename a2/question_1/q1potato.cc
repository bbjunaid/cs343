#include "q1potato.h"
#include <iostream>

using namespace std;

/*
    Given a minimum and maximum range, returns a random number in that range inclusive
    of the min and max bounds
*/
unsigned int randr( unsigned int min, unsigned int max) {	
	return rand() % max + min;
}

/* 
    Constructor takes in a maximum number of ticks.
*/
Potato::Potato( unsigned int maxTicks )
	: m_maxTicks( maxTicks )
{ }

/* 
    Resets the counter to random value and prints appropriate message.
*/
void Potato::reset( unsigned int maxTicks ) { 
    m_numTicks = randr( 1, m_maxTicks ); 
	cout << "  POTATO will go off after " << m_numTicks << " tosses" << std::endl;
} 

/* 
    Decrements counter and returns whether the counte reached 0 or not.
*/    
bool Potato::countdown() { 
	m_numTicks--; 
    return m_numTicks > 0 ? false : true; 
}
