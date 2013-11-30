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
, m_maxPurchases(maxPurchases) {
    
}

void Student::main() {
    
    WATCard::FWATCard fCard;                // Future watcard
    VendingMachine::Flavours favSoda;     // Student's favourite soda
    unsigned int numBottles;              // Student's number of purchases
    VendingMachine::Status buyStatus;       // The status of the purchase made by the student
    VendingMachine* m_vendingMachine;       // Vending machine assigned by nameServer that purchases

    numBottles = mprng( 1, m_maxPurchases );          // Select a random number of bottles to purchase
    favSoda = (VendingMachine::Flavours)mprng( 3 );   // Select a random flavour
    

    m_prt.print( Printer::Student, 'S', favSoda, numBottles );  // Print student start message
    fCard = m_cardOffice.create( m_id, 5 ); // Create a WATCard via the WATCardOffice with a $5 balance 

    // Obtain the location of a vending machine from the name server
    // TODO m_vendingMachine = m_nameServer.getMachine( m_id );
    // Print vending machine selected
    m_prt.print( Printer::Student, 'V', m_vendingMachine->getId() );    

    // Periodically buy favourite soda
    unsigned int numPurchased = 0;                      // Counter for number of successful purchases
    for ( ; numPurchased != numBottles ; ) {
        yield( mprng( 1, 10 ) );                        // Before buying, yield a random number of times in the range [1, 10]
        for ( ;; ) {                                    // Loop until vending machine returns a status: retry simulation
            try {
                buyStatus = m_vendingMachine->buy( favSoda, *fCard() );  // Attempt to buy favourite soda
                break;                                  // If no exception thrown, move on to to examine purchase status
            } catch ( WATCardOffice::Lost ) {
                m_prt.print( Printer::Student, 'L' );   // Print WATCard lost message
                fCard = m_cardOffice.create( m_id, 5 ); // Create a new WATCard via the WATCardOffice with a $5 balance
            }
        }

        switch (buyStatus) {
            case VendingMachine::BUY:                   // Successful buy
                numPurchased += 1;                      // Indicate a successful purchase
                m_prt.print( Printer::Student, 'B', fCard()->getBalance() );       // Print bought a soda
                break;
            case VendingMachine::STOCK:                 // Out of stock
                // TODO m_vendingMachine = m_nameServer.getMachine( m_id ); // Obtain a new vending machine from the name server
                m_prt.print( Printer::Student, 'V', m_vendingMachine->getId() );    // Print vending machine selected
                break;
            case VendingMachine::FUNDS:                 // Insufficient funds
                fCard = m_cardOffice.transfer( m_id, m_vendingMachine->cost() + 5, fCard() );
                break;
        }
    }

    m_prt.print( Printer::Student, 'F');    // Student finished purchasing
                                            // Print finished
}

