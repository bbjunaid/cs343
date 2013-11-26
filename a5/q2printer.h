/*
   Interface for the Printer Monitor
*/

#ifndef _PRINTER
#define _PRINTER

#include <uC++.h>
#include "q2phil.h" // to access Philosopher::States

_Monitor Printer {
  private:
    struct StateNode {
        char state;
        unsigned int bite;
        unsigned int noodles;
        bool isEmpty;
    };
    // helper function to get the id of left and right philosopher
    void getAdjacentPhils( unsigned int id, int &leftId, int &rightId ); 

  public:
    Printer( unsigned int numPhil );
    ~Printer() { 
        delete[] m_buffer; 
    }
    // update state and print if change in buffer
    void print( unsigned int id, Philosopher::States state );    
    // update state and bite/noodle information
    void print( unsigned int id, Philosopher::States state, unsigned int bite, unsigned int noodles );  
    void finish( unsigned int id );                 // finish flush message
    void flush();                                   // print the states in a line

  private:
    const unsigned int m_numPhil;   // number of philosophers
    bool m_isBufferEmpty;           // is the entire buffer empty
    StateNode *m_buffer;            // states buffer
};

#endif // _PRINTER

