/* 
   Interface for the Philosopher Task
*/

#ifndef _PHIL
#define _PHIL

#include <uC++.h>

#if defined( TABLETYPE_TASK )
_Task Table;
#else
_Monitor Table;
#endif
_Monitor Printer;

_Task Philosopher {
    void main();

  public:
    enum States { Thinking = 'T', Hungry = 'H', Eating ='E',\
                  Waiting = 'W', Finished = 'F', Barging = 'B' };
    Philosopher( unsigned int id, unsigned int noodles, Table &table, Printer &prt );

  private:
    const unsigned int m_id;    // philosopher id
    unsigned int m_noodles;     // num noodles to eat
    Table& m_table;             // table for synchornization 
    Printer& m_prt;             // printer for displaying messages
};

#endif //_PHIL
