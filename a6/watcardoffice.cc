#include "watcardoffice.h"

#include "printer.h"
#include "bank.h"

void WATCardOffice::main() {
}

WATCardOffice::WATCardOffice( Printer &prt, Bank &bank, unsigned int numCouriers ) : 
    m_prt(prt), m_bank(bank), m_numCouriers(numCouriers) {

    for ( unsigned int i; i < m_numCouriers; i += 1 ) {
        // Create courier tasks
    }
}

/*WATCard::FWATCard WATCardOffice::create( unsigned int sid, unsigned int amount ) {
}

WATCard::FWATCard WATCardOffice::transfer( unsigned int sid, unsigned int amount, WATCard *card ) {
}

Job *WATCardOffice::requestWork() {
    // Block courier until a job is ready
}*/

