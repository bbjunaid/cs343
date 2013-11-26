/* 
    Interface for the Reindeer Task
*/

#ifndef _REINDEER
#define _REINDEER

#include <uC++.h>
#include "q2workshop.h"
#include "q2printer.h"

_Task Reindeer {
    void main();

  public:
    enum { MAX_NUM_REINDEER = 5 }; // number of reindeer in system for delivering toys
    Reindeer( unsigned int id, Workshop &wrk, Printer &prt, unsigned int numDeliveries )
    : m_id( id ), m_wrk( wrk ), m_prt( prt ), m_numDeliveries( numDeliveries ) {}

  private:
    unsigned int m_id;
    Workshop &m_wrk;
    Printer &m_prt;
    unsigned int m_numDeliveries;
};

#endif // _REINDEER
