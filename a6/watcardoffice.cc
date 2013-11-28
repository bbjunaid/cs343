#include "watcardoffice.h"

#include "printer.h"
#include "bank.h"

void WATCardOffice::main() {
    m_prt.print( Printer::WATCardOffice, 'S' );
    
    for ( ;; ) {
        _Accept( create ) {
        } or _Accept( transfer ) {

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
    WATCard::FWATCard fCard;
    //WATCard card;
    //fCard = &card;
    courier.signal();
    // Print creation rendezvous complete
    m_prt.print( Printer::WATCardOffice, sid, amount );
    return fCard;
}

WATCard::FWATCard WATCardOffice::transfer( unsigned int sid, unsigned int amount, WATCard *card ) {
    WATCard::FWATCard fCard;
    //WATCard card;
    //fCard = &card;
    courier.signal();
    // Print transfer rendezvous complete
    m_prt.print( Printer::WATCardOffice, sid, amount );
    return fCard;
}

WATCardOffice::Job *WATCardOffice::requestWork() {
    // Block courier until a job is ready
    Job* job;
    courier.wait();
    m_prt.print( Printer::WATCardOffice, 'W' );
    return job;
}

