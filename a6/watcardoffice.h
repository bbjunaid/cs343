//-----------------------------------------------------------------------------
// Header for the definition of WATCardOffice administrator task
//-----------------------------------------------------------------------------

#ifndef _WATCARDOFFICE_H__
#define _WATCARDOFFICE_H__

#include "watcard.h"

_Monitor Printer;
_Monitor Bank;

_Task WATCardOffice {
   struct Job {                             // marshalled arguments and return future
        int args;                           // call arguments (TODO YOU DEFINE "Args")
        WATCard::FWATCard result;           // return future
        Job( int args ) : args( args ) {}
    }; 

   void main();
  public:
    _Event Lost {};                         // uC++ exception type, like "struct"
    WATCardOffice( Printer &prt, Bank &bank, unsigned int numCouriers );
    WATCard::FWATCard create( unsigned int sid, unsigned int amount );
    WATCard::FWATCard transfer( unsigned int sid, unsigned int amount, WATCard *card );
    Job *requestWork();

  private:
    Printer &m_prt;
    Bank &m_bank;
    unsigned int m_numCouriers;
    _Task Courier {};                       // communicates with bank
};

#endif // _WATCARDOFFICE_H__

