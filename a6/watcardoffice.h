//-----------------------------------------------------------------------------
// Header for the definition of WATCardOffice administrator task
//-----------------------------------------------------------------------------

#ifndef _WATCARDOFFICE_H__
#define _WATCARDOFFICE_H__

#include "watcard.h"
#include <queue>
#include <vector>

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

    _Task Courier {                          // Worker tasks transfers funds from bank to student's watcard on behalf of the office
        void main();
      public:
        // Courier gets instance of the office and a unique id among couriers
        Courier( unsigned int id, WATCardOffice &office, Printer &prt, Bank &bank );

      private:
        // Courier's private task variables
        unsigned int m_id;
        WATCardOffice &m_office;
        Printer &m_prt;
        Bank &m_bank;
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
    std::queue<Job *> m_requests;           // Queue of jobs used by office to hand off new work to the courier
    Job *m_newJob;                          // Pointer to a newly created job that office hands off to courier
    std::vector<Courier*> couriers;         // Vector holding pointer to created couriers. Couriers are dynamicaly created as their number is not known statically
};

#endif // _WATCARDOFFICE_H__

