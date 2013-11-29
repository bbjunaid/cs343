#include "watcardoffice.h"

#include "printer.h"
#include "bank.h"
#include "MPRNG.h"

extern MPRNG mprng;

void WATCardOffice::main() {
    m_prt.print( Printer::WATCardOffice, 'S' );
    
    for ( ;; ) {
        _Accept( create ) {
            requests.push( m_newJob );
            courier.signalBlock();
            // Print creation rendezvous complete
            m_prt.print( Printer::WATCardOffice, 'C', m_newJob->args.sid, m_newJob->args.amount );
        } or 
        _Accept( transfer ) {
            requests.push( m_newJob );
            courier.signalBlock();
            // Print transfer rendezvous complete
            m_prt.print( Printer::WATCardOffice, 'T', m_newJob->args.sid, m_newJob->args.amount );
        } or
        _Accept( requestWork ) {
            m_prt.print( Printer::WATCardOffice, 'W' );
        }

    }

    m_prt.print( Printer::WATCardOffice, 'F' );
}

WATCardOffice::WATCardOffice( Printer &prt, Bank &bank, unsigned int numCouriers ) : 
    m_prt(prt), m_bank(bank), m_numCouriers(numCouriers) {

    // Create courier tasks
    for ( unsigned int i; i < m_numCouriers; i += 1 ) {
        couriers.push_back( new Courier( *this ) );
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
    // Block courier until a job is ready
    courier.wait();
    Job* job = requests.front();
    requests.pop();
    return job;
}

WATCardOffice::Courier::Courier( WATCardOffice &office ) 
: m_office(office) {}

void WATCardOffice::Courier::main() {
    m_office.m_prt.print( Printer::Courier, 'S' );
    for ( ;; ) {
        // Request work - may get blocked
        Job* job = m_office.requestWork();

        // Withdraw from the back
        m_office.m_bank.withdraw( job->args.sid, job->args.amount );

        // Deposit after a funds transfer
        job->args.card->deposit( job->args.amount );
        
        // a courier can lose a student's watcard during the transfer for the new create
        // There is a 1 in 6 chance WATCard is lost
        if ( mprng( 1, 6 ) % 6 == 0 ) {
            job->result.exception( new Lost );
            // Delete current WATCard
        }
        job->result.delivery( job->args.card );
        // TODO exit condition?
    }
    m_office.m_prt.print( Printer::Courier, 'F' );         // Print finished message
}

