#include "student.h"

#include "MPRNG.h"
#include "printer.h"
#include "nameserver.h"
#include "watcardoffice.h"
#include "vendingmachine.h"
#include "watcard.h"

extern MPRNG prng;                  // Declaration of global pseudo-random number generator (same for all components)

Student::Student( Printer &prt, NameServer &nameServer, WATCardOffice &cardOffice, unsigned int id, \
             unsigned int maxPurchases )
: m_prt(prt)
, m_nameServer(nameServer)
, m_cardOffice(cardOffice)
, m_id(id)
, m_maxPurchases {}

void Student::main() {
    /*
    // Periodically buy ther their favourite 
    // Variables needed
    unsigned int numBottles;
    VendingMachine::Flavours favSoda;
    FWATCard fCard; 
    VendingMachine::Status buyStatus;

    // select a random number of bottles to purchase
    numBottles = prng( 1, m_MaxPurchases );

    // select a random flavour
    favSoda = prng( 3 );
    
    m_prt.print( Printer::Student, 'S', favSoda, numBottles );

    // Create a WATCard via the WATCardOffice with a $5 balance
    fCard = m_cardOffice.create( m_id, 5 );

    // Obtain the location of a vending machine from the name server
    m_vendingMachine = getMachine( m_id );
    m_prt.print( Printer::Student, 'V', m_vendingMachine->getId() );

    WATCard card;

    for ( unsigned int i = 0; i < numBottles; i += 1 ) {
        yield( prng( 1, 10 ) );
        // Student buys favourite soda
        buyStatus = vendingMachine->buy( favSoda, card );
        switch (buyStatus) {
            case BUY:                                   // Successful buy
                // Print bought a soda
                m_prt.print( Printer::Student, 'B', card.getBalance() );
                break;
            case STOCK:                                 // Out of stock
                // Obtain a new vending machine from the name server
                m_vendingMachine = getMachine( m_id );
                // Print vending machine selected
                m_prt.print( Printer::Student, 'V', m_vendingMachine->getId() );
                break;
            case FUNDS:                                 // Insufficient funds
                fCard = m_cardOffice.transfer( id, vendingMachine->cost() + 5, card);
                break;
        }
    }

    // Student finished purchasing
    // Print finished
    m_prt.print( Printer::Student, 'F');
    */
}

