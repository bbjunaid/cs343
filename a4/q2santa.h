/* 
    Interface for the Santa Task
*/

#ifndef _SANTA
#define _SANTA

#include <uC++.h>
#include "q2workshop.h"
#include "q2printer.h"

_Task Santa {
    void main();

  public:
    Santa( Workshop &wrk, Printer &prt ) : m_wrk(wrk), m_prt(prt) {}
  
  private:
    Workshop &m_wrk;
    Printer &m_prt;
};

#endif // _SANTA
