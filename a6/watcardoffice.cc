#include "watcardoffice.h"

#include "printer.h"
#include "bank.h"
#include "MPRNG.h"

extern MPRNG mprng;

void WATCardOffice::main() {
    m_prt.print( Printer::WATCardOffice, 'S' );
    
    for ( ;; ) {
        _Accept ( ~WATCardOffice ) {
            osacquire( cout ) << "Inside accept destructor of WATCard office" << endl;
            for ( unsigned int i = 0; i < m_numCouriers; i++ ) {
                _Accept( requestWork ) {
                } _Else {
                }
            }
            osacquire( cout ) << "About to delete couriers" << endl;
            for( unsigned int i; i < m_numCouriers; i++ )
                delete couriers[i];
            osacquire( cout ) << "Deleted couriers" << endl;
            break;
        }
        or _Accept( create ) {
            m_requests.push( m_newJob );
            // Print creation rendezvous complete
            m_prt.print( Printer::WATCardOffice, 'C', m_newJob->args.sid, m_newJob->args.amount );
        } or _Accept( transfer ) {
            m_requests.push( m_newJob );
            // Print transfer rendezvous complete
            m_prt.print( Printer::WATCardOffice, 'T', m_newJob->args.sid, m_newJob->args.amount );
        } or _When( m_requests.size() > 0 ) _Accept( requestWork ) {
            m_prt.print( Printer::WATCardOffice, 'W' );
        }

    }
    m_prt.print( Printer::WATCardOffice, 'F' );
}

WATCardOffice::~WATCardOffice() {
    osacquire( cout ) << "I'm finally finishing watcard office detructor" << endl;
}

WATCardOffice::WATCardOffice( Printer &prt, Bank &bank, unsigned int numCouriers ) 
: m_prt(prt)
, m_bank(bank)
, m_numCouriers(numCouriers) {
    couriers.resize( numCouriers );
    for ( unsigned int i; i < m_numCouriers; i += 1 ) {
        couriers.push_back( new Courier( i, *this, prt, bank ) );
    }
}


WATCard::FWATCard WATCardOffice::create( unsigned int sid, unsigned int amount ) {
    Job *job = new Job( Args( sid, amount, new WATCard() ) );
    m_newJob = job;
    return job->result;
}

WATCard::FWATCard WATCardOffice::transfer( unsigned int sid, unsigned int amount, WATCard *card ) {
    Job *job = new Job( Args( sid, amount, card ) ); 
    m_newJob = job;
    return job->result;
}

WATCardOffice::Job *WATCardOffice::requestWork() {
    // When Courier get's unblocked at the very end, there will be no jobs
    if ( m_requests.empty () ) 
        return NULL; 

    Job* job = m_requests.front();
    m_requests.pop();
    return job;
}


//-----------------------------------------------------------------------------
// Implmentation of Courier Task
//-----------------------------------------------------------------------------

WATCardOffice::Courier::Courier( unsigned int id, WATCardOffice &office, Printer &prt, Bank &bank )
: m_id( id )
, m_office(office)
, m_prt( prt )
, m_bank( bank )
{}

void WATCardOffice::Courier::main() {
    m_prt.print( Printer::Courier, m_id, 'S' );
    for ( ;; ) {
        _Accept( WATCardOffice::~Courier ) {
            osacquire( cout ) << "Destructor of Courier!!!" << endl;
            break;
        }
        _Else {
            // Request work - may get blocked
            osacquire( cout ) << "Requesting work" << endl;
            Job* job = m_office.requestWork();

            if ( job == NULL ) {
                osacquire( cout ) << "Job is NULL" << endl;
                continue;
            }
            osacquire( cout ) << "Done Requesting work" << endl;

            // Print start funds transfer
            osacquire( cout ) << "About to print" << endl;
            m_prt.print( Printer::Courier, m_id, 't', job->args.sid, job->args.amount );
            osacquire( cout ) << "Done print" << endl;
            // Withdraw from the back
            osacquire( cout ) << "About to withdraw" << endl;
            m_bank.withdraw( job->args.sid, job->args.amount );
            osacquire( cout ) << "Done withdrawing" << endl;

            // Deposit after a funds transfer
            job->args.card->deposit( job->args.amount );
            // Print complete funds transfer
            m_prt.print( Printer::Courier, m_id, 'T', job->args.sid, job->args.amount );
            
            if ( mprng( 5 ) == 0 ) {                     // There is a 1 in 6 chance WATCard is lost
                job->result.exception( new Lost );              // Insert Lost exception intor student's WATCard
                delete job->args.card;                          // Delete current WATCard
            }
            else {
                job->result.delivery( job->args.card );         // Deliver future upon a successful transfer of funds
            }
            delete job;
        }
    }
    m_prt.print( Printer::Courier, m_id, 'F' );          // Print finished message
}

