/* 
    Interface for the Table Object Providing Mutual Exclusion/Synchronization
    for eating Philosophers.
*/

#ifndef _TABLE
#define _TABLE

#include <uC++.h>
#include "q2printer.h"
#include <list>

_Monitor Printer;                       // forward declaration

#if defined( TABLETYPE_INT )            // internal scheduling monitor solution
_Monitor Table {
    uCondition *m_philConds;            // conditions which philosophers wait on 

#elif defined( TABLETYPE_INTB )         // internal scheduling monitor solution with barging
_Monitor Table {
    uCondition waiting;                 // only one condition variable (you may change the variable name)
    bool m_isSignalled;                 // has a call to signal been made. used to distinguish a barger 
    void wait();                        // barging version of wait
    void signal();                      // signal function corresponding to wait
    list<unsigned int> m_bargers;      // queue of bargers
    unsigned int m_numSignalled;        // number of signal calls
#elif defined( TABLETYPE_AUTO )         // automatic-signal monitor solution
#include "AutomaticSignal.h"
_Monitor Table {
  private:
    AUTOMATIC_SIGNAL;
#elif defined( TABLETYPE_TASK )         // internal/external scheduling task solution
_Task Table {
  protected:
    void main();                        // server main
  private:
    uCondition *m_philConds;            // conditions which philosophers wait on
    unsigned int m_currId;              // currently executing client (philosopher)
#else
#error unsupported table
#endif
    // helper function to get the id of left and right philosopher
    void getAdjacentPhils( unsigned int id, int &leftId, int &rightId ); 
  public:                               // common interface
    Table( const unsigned int NoOfPhil, Printer &prt );
    ~Table();
    void pickup( unsigned int id );     // philosopher picks up left and right fork
    void putdown( unsigned int id );    // philosopher drops left and right fork

  private:
    const unsigned int m_numPhil;       // number of philosophers 
    Printer &m_prt;                     // printer for displaying messages
    bool *m_forkTaken;                  // true if fork used by philosopher, false otherwise
                                        // left fork: m_forkTaken[id], right fork: m_forkTaken[id+1]
};

#endif //_TABLE
