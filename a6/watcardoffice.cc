#include "watcardoffice.h"

#include "printer.h"
#include "bank.h"
#include "MPRNG.h"

extern MPRNG mprng;

void WATCardOffice::main() {
    m_prt.print( Printer::WATCardOffice, 'S' );
    
    for ( ;; ) {
        _Accept( create ) {
            Job *job = new Job( Args( m_sid, m_amount, new WATCard() ) );
            requests.push( job );
            courier.signalBlock();
            // Print creation rendezvous complete
            m_prt.print( Printer::WATCardOffice, 'C', m_sid, m_amount );
        } or 
        _Accept( transfer ) {
            Job *job = new Job( Args( m_sid, m_amount, m_card ) );
            requests.push( job );
            courier.signalBlock();
            // Print transfer rendezvous complete
            m_prt.print( Printer::WATCardOffice, 'T', m_sid, m_amount );
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
        couriers.push_back( new Courier );
    }
}

WATCard::FWATCard WATCardOffice::create( unsigned int sid, unsigned int amount ) {
    m_sid = sid;
    m_amount = amount;
    WATCard::FWATCard fCard;
    return fCard;
}

WATCard::FWATCard WATCardOffice::transfer( unsigned int sid, unsigned int amount, WATCard *card ) {
    m_sid = sid;
    m_amount = amount;
    m_card = card;
    WATCard::FWATCard fCard;
    return fCard;
}

WATCardOffice::Job *WATCardOffice::requestWork() {
    // Block courier until a job is ready
    courier.wait();
    Job* job = requests.front();
    requests.pop();
    return job;
}

void WATCardOffice::Courier::main() {
    //m_prt.print( Printer::Courier, 'S' );
    for ( ;; ) {
        // Request work - may get blocked
        Job* job;
        // TODO Job* job = requestWork();

        // Withdraw from the back
        // TODO m_bank.withdraw( job->args.sid, job->args.amount );

        // Call deposit after a funds transfer
        job->args.card->deposit( job->args.amount );
        
        // a courier can lose a student's watcard during the transfer for the new create
        // There is a 1 in 6 chance WATCard is lost
        if ( mprng( 1, 6 ) % 6 == 0 ) {
            job->result.exception( new Lost );
            // Delete current WATCard
        }
        job->result.delivery( job->args.card );
    }
    //m_prt.print( Printer::Courier, 'F' );         // Print finished message
}

