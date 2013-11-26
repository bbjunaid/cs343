/* 
    Interface for the Elf Task
*/

#ifndef _ELF
#define _ELF

#include <uC++.h>
#include "q2workshop.h"
#include "q2printer.h"

_Task Elf {
    void main();

  public:
    enum { CONSULTING_GROUP_SIZE = 3 }; // number of elves for a consultation with Santa
    Elf( unsigned int id, Workshop &wrk, Printer &prt, unsigned int numConsultations )
    : m_id( id ), m_wrk( wrk ), m_prt( prt ), m_numConsultations( numConsultations ) {}

  private:
    unsigned int m_id;
    Workshop &m_wrk;
    Printer &m_prt;
    unsigned int m_numConsultations;
};

#endif //_ELF
