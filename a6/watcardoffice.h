//-----------------------------------------------------------------------------
// Header for the definition of WATCardOffice administrator task
//-----------------------------------------------------------------------------

#ifndef _WATCARDOFFICE_H__
#define _WATCARDOFFICE_H__

#include "watcard.h"
#include <queue>

_Monitor Printer;
_Monitor Bank;

_Task WATCardOffice {
   
    struct Args {
        unsigned int sid;
        unsigned int amount;
        WATCard *card;
        Args( unsigned int sid, unsigned int amount, WATCard *card) \
        : sid( sid ), amount( amount ), card(card) {}
    };
    struct Job {                             // marshalled arguments and return future
        Args args;                           // call arguments
        WATCard::FWATCard result;            // return future
        Job( Args args ) : args( args ) {}
    }; 

    _Task Courier {                 // Worker tasks transfers funds from bank to student's watcard on behalf of the office
        void main();
      public:
        Courier( unsigned int id, WATCardOffice &office );

      private:
        WATCardOffice &m_office;
        unsigned int m_id;
    };

    void main();
  public:
    _Event Lost {};                         // uC++ exception type, like "struct"
    WATCardOffice( Printer &prt, Bank &bank, unsigned int numCouriers );
    ~WATCardOffice();
    WATCard::FWATCard create( unsigned int sid, unsigned int amount );
    WATCard::FWATCard transfer( unsigned int sid, unsigned int amount, WATCard *card );
    Job *requestWork();

  private:
    Printer &m_prt;
    Bank &m_bank;
    unsigned int m_numCouriers;
    WATCard *m_card;
    uCondition courier;
    std::queue<Job *> m_requests;
    Job *m_newJob;
    Courier** couriers;
};

#endif // _WATCARDOFFICE_H__

