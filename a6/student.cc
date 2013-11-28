#include "student.h"

#include "MPRNG.h"
#include "printer.h"
#include "nameserver.h"
#include "watcardoffice.h"
#include "vendingmachine.h"
#include "watcard.h"

extern MPRNG mprng;                  // Declaration of global pseudo-random number generator (same for all components)

Student::Student( Printer &prt, NameServer &nameServer, WATCardOffice &cardOffice, unsigned int id, 
             unsigned int maxPurchases )
: m_prt(prt)
, m_nameServer(nameServer)
, m_cardOffice(cardOffice)
, m_id(id)
, m_maxPurchases(maxPurchases) {}

void Student::main() {
    
    // Periodically buy ther their favourite 
    // Variables needed
    unsigned int numBottles;
    VendingMachine::Flavours favSoda;
    WATCard::FWATCard fCard;             // future watcard
    VendingMachine::Status buyStatus;
    // The vending machine assigned by nameServer that purchases can be made from
    VendingMachine* m_vendingMachine;   

    // Select a random number of bottles to purchase
    numBottles = mprng( 1, m_maxPurchases );

    // Select a random flavour
    favSoda = (VendingMachine::Flavours)mprng( 3 );
    
    // Print student start message
    m_prt.print( Printer::Student, 'S', favSoda, numBottles );

    // Create a WATCard via the WATCardOffice with a $5 balance
    fCard = m_cardOffice.create( m_id, 5 );

    // Obtain the location of a vending machine from the name server
    // TODO m_vendingMachine = m_nameServer.getMachine( m_id );
    // Print vending machine selected
    // TODO m_prt.print( Printer::Student, 'V', m_vendingMachine->getId() );

    for ( unsigned int i = 0; i < numBottles; i += 1 ) {
        for ( ;; ) {            // Loop until vending machine returns a status
            try {
                // TODO buyStatus = m_vendingMachine->buy( favSoda, fCard() );  // Student buys favourite soda
                yield( mprng( 1, 10 ) );
                break;
            } catch ( WATCardOffice::Lost ) {
                // Print WATCard lost message
                m_prt.print( Printer::Student, 'L' );
                // Create a new WATCard via the WATCardOffice with a $5 balance
                fCard = m_cardOffice.create( m_id, 5 );
            }
        }

        switch (buyStatus) {
            case VendingMachine::BUY:                                   // Successful buy
                // Print bought a soda
                // TODO m_prt.print( Printer::Student, 'B', fCard->getBalance() );
                break;
            case VendingMachine::STOCK:                                 // Out of stock
                // Obtain a new vending machine from the name server
                // TODO m_vendingMachine = m_nameServer.getMachine( m_id );
                // Print vending machine selected
                // TODO m_prt.print( Printer::Student, 'V', m_vendingMachine->getId() );
                break;
            case VendingMachine::FUNDS:                                 // Insufficient funds
                // TODO fCard = m_cardOffice.transfer( m_id, m_vendingMachine->cost() + 5, fCard );
                break;
        }
    }

    // Student finished purchasing
    // Print finished
    m_prt.print( Printer::Student, 'F');
}

