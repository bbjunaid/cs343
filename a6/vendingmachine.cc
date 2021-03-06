#include "vendingmachine.h"
#include "printer.h"
#include "nameserver.h"
#include "watcard.h"

// Vending machine task code
// Register vending machine
// Give priority to restocking (by the truck) so that restocking can happen before a student buys
// Do no let student buy if restocking is happening (externally scheduled)
void VendingMachine::main() {
    m_prt.print( Printer::Vending, m_id, 'S', m_sodaCost );
    // register with the nameServer
    m_nameServer.VMregister( this );
    for ( ;; ) {
        _Accept ( ~VendingMachine ) {
            break;
        } 
        or _Accept( inventory ) {
            m_restocking = true;                            // indicate restocking
        }
        or _Accept( restocked ) {
            m_restocking = false;                           // indicate restocking complete
        }
        or _When(!m_restocking) _Accept( buy ) {     // Do not accept student calls to buy when restocking
        }
    }
    m_prt.print( Printer::Vending, m_id, 'F' );
}

// VendingMachine constructor
// Initialize internal variables
// Reset number of sodas stocked
VendingMachine::VendingMachine( Printer &prt, NameServer &nameServer, unsigned int id,\
                                unsigned int sodaCost, unsigned int maxStockPerFlavour ) 
: m_prt( prt )
, m_nameServer( nameServer )
, m_id( id )
, m_sodaCost( sodaCost )
, m_maxStockPerFlavour( maxStockPerFlavour )
, m_restocking(false) {
    for ( unsigned int i = 0; i < NUM_FLAVOURS; i += 1 ) {
        m_stockedSoda[i] = 0;
    }
}

// Student calls buy to obtain one of their favourite soda
VendingMachine::Status VendingMachine::buy( Flavours flavour, WATCard &card ) {
    if ( m_stockedSoda[(unsigned int)flavour] == 0 ) {
        return STOCK;
    }

    if ( card.getBalance() < m_sodaCost ) {
        return FUNDS;
    }

    m_stockedSoda[(unsigned int)flavour] -= 1;
    card.withdraw( m_sodaCost );
    m_prt.print( Printer::Vending, m_id, 'B', (unsigned int)flavour,\
                 m_stockedSoda[(unsigned int)flavour] );
    return BUY;
}

// The truck calls inventory to return a pointer to an array containing the 
// amount of each kind of soda currently in the vending machine
unsigned int *VendingMachine::inventory() {
    m_prt.print( Printer::Vending, m_id, 'r' );
    return m_stockedSoda;
}

void VendingMachine::restocked() {
    m_prt.print( Printer::Vending, m_id, 'R' );
}

// Non-mutex to return the cost of a vending machine's sode
_Nomutex unsigned int VendingMachine::cost() {
    return m_sodaCost;
}

// Non-mutex to return the vending machine's id 
_Nomutex unsigned int VendingMachine::getId() {
    return m_id;
}

