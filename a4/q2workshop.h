/* 
    Interface for the Workshop Monitor
*/

#ifndef _WORKSHOP
#define _WORKSHOP

#include <uC++.h>
#include "q2printer.h"

_Monitor Workshop {
  public:
    enum Status { Consulting, Delivery, Done };
    Workshop( Printer &prt, unsigned int N, unsigned int E, unsigned int D ); // printer, bound, elves, reindeer delivery
    Status sleep();                         // santa calls to nap; when Santa wakes status of next action
    void deliver( unsigned int id );        // reindeer call to deliver toys
    bool consult( unsigned int id );        // elves call to consult Santa,
                                            // true => consultation successful, false => consultation failed
    void doneConsulting( unsigned int id ); // block Santa/elves until meeting over
    void doneDelivering( unsigned int id ); // block Santa/reindeer until all toys are delivered
    void termination( unsigned int id );    // elves call to indicate termination
    
  private:
    Printer &m_prt;                         // printer
    const unsigned int m_bound;             // bound on raindeer priority over elves
    unsigned int m_boundCounter;            // remaining count of reindeer priority over elves
    unsigned int m_elfCount;                // number of elves remaining
    int m_numElvesToSignal;        // number of elves that have been signalled
                                            // used to release elves in the order they arrived
    unsigned int m_numDeliveries;           // number of deliveries remaining
    unsigned int m_deliverCount;            // number of tasks blocked in delivery condition
    unsigned int m_doneDeliverCount;        // number of tasks blocked in done delivery condition
    unsigned int m_consultCount;            // number of tasks blocked in consult condition
    unsigned int m_doneConsultCount;        // number of tasks blocked in done consult condition
    uCondition m_santaCond;                 // condition santa sleeps on until awoken by reindeer/elves
    uCondition m_deliverCond;               // condition raindeer block on for delivering
    uCondition m_doneDeliverCond;           // condition raindeer/santa block on for done delivery
    uCondition m_consultCond;               // condition elves block on for consulting
    uCondition m_doneConsultCond;           // condition elves/santa block on for done consulting
};

#endif // _WORKSHOP
