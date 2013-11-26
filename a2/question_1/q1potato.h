/*
    Interface for the Potato class
*/

#ifndef _POTATO
#define _POTATO

// generated random number between min, max inclusive
unsigned int randr(unsigned int min, unsigned int max); 

class Potato {
  public:
    Potato( unsigned int maxTicks = 10 ); // constructor
	void reset( unsigned int maxTicks = 10 ); // reset the counter    
	bool countdown(); // decrement the counter and return whether it reached 0
  private:
	const int m_maxTicks; // max number of ticks that the potato can hold
	int m_numTicks;       // number of remaining ticks 
};

#endif // _POTATO
