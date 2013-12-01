#include "watcardoffice.h"

#include "printer.h"
#include "bank.h"
#include "MPRNG.h"

extern MPRNG mprng;

void WATCardOffice::main() {
    m_prt.print( Printer::WATCardOffice, 'S' );
    
    for ( ;; ) {
        _Accept( create ) {
            m_requests.push( m_newJob );
            courier.signalBlock();
            // Print creation rendezvous complete
            m_prt.print( Printer::WATCardOffice, 'C', m_newJob->args.sid, m_newJob->args.amount );
        } or 
        _Accept( transfer ) {
            m_requests.push( m_newJob );
            courier.signal();
            // Print transfer rendezvous complete
            m_prt.print( Printer::WATCardOffice, 'T', m_newJob->args.sid, m_newJob->args.amount );
        } or
        _Accept( requestWork ) {
            m_prt.print( Printer::WATCardOffice, 'W' );
        }

    }
    m_prt.print( Printer::WATCardOffice, 'F' );
}

WATCardOffice::WATCardOffice( Printer &prt, Bank &bank, unsigned int numCouriers ) 
: m_prt(prt)
, m_bank(bank)
, m_numCouriers(numCouriers) {
    couriers.resize( numCouriers );
    for ( unsigned int i; i < m_numCouriers; i += 1 ) {
        couriers.push_back( new Courier( i, *this ) );
    }
}

WATCardOffice::~WATCardOffice() {
    for( unsigned int i; i < m_numCouriers; i++ )
        delete couriers[i];
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
    courier.wait();                 // Block courier until a job is ready
    Job* job = m_requests.front();
    m_requests.pop();
    return job;
}


//-----------------------------------------------------------------------------
// Implmentation of Courier Task
//-----------------------------------------------------------------------------

WATCardOffice::Courier::Courier( unsigned int id, WATCardOffice &office ) 
: m_office(office)
, m_id( id )
{}

void WATCardOffice::Courier::main() {
    m_office.m_prt.print( Printer::Courier, m_id, 'S' );
    for ( ;; ) {
        _Accept( ~Courier ) {
            break;
        }
        _Else {
            // Request work - may get blocked
            Job* job = m_office.requestWork();

            // Print start funds transfer
            m_office.m_prt.print( Printer::Courier, m_id, 't', job->args.sid, job->args.amount );
            // Withdraw from the back
            m_office.m_bank.withdraw( job->args.sid, job->args.amount );

            // Deposit after a funds transfer
            job->args.card->deposit( job->args.amount );
            // Print complete funds transfer
            m_office.m_prt.print( Printer::Courier, m_id, 'T', job->args.sid, job->args.amount );
            
            if ( mprng( 1, 6 ) % 6 == 0 ) {                     // There is a 1 in 6 chance WATCard is lost
                job->result.exception( new Lost );              // Insert Lost exception intor student's WATCard
                delete job->args.card;                          // Delete current WATCard
            }
            else {
                job->result.delivery( job->args.card );         // Deliver future upon a successful transfer of funds
            }
            delete job;
        }
    }
    m_office.m_prt.print( Printer::Courier, m_id, 'F' );          // Print finished message
}

