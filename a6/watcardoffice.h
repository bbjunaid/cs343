//-----------------------------------------------------------------------------
// Header for the definition of WATCardOffice administrator task
//-----------------------------------------------------------------------------

#ifndef _WATCARDOFFICE_H__
#define _WATCARDOFFICE_H__

#include "watcard.h"
#include <vector>

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
    uCondition courier;
    _Task Courier { // communicates with bank
        void main() {
            /*
            for ( ;; ) {
                m_prt.print( Printer::Courier, 'S' );
                // Request work - may get blocked
                Job* job = requestWork();

                // Call deposit after a funds transfer
                job->result.deposit( amount );
                
                // a courier can lose a student's watcard during the transfer for the new create
                // There is a 1 in 6 chance WATCard is lost
                if ( mprng( 1, 6 ) % 6 == 0 ) {
                    job->result.exception( new Lost );
                    // Delete current WATCard
                }
            }
            m_prt.print( Printer::Courier, 'F' );         // Print finished message
            */
        }
    };
    std::vector<Courier *> couriers;
};

#endif // _WATCARDOFFICE_H__

