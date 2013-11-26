#include "vendingmachine.h"
#include "printer.h"
#include "nameserver.h"

void VendingMachine::main() {
}

VendingMachine::VendingMachine( Printer &prt, NameServer &nameServer, unsigned int id, unsigned int sodaCost, unsigned int maxStockPerFlavour ) 
: m_prt( prt )
, m_nameServer( nameServer )
, m_id( id )
, m_sodaCost( sodaCost )
, m_maxStockPerFlavour( maxStockPerFlavour ) {}

/*Status VendingMachine::buy( Flavours flavour, WATCard &card ) {
}

unsigned int *VendingMachine::inventory() {
}

void VendingMachine::restocked() {
}

_Nomutex unsigned int VendingMachine::cost() {
}

_Nomutex unsigned int VendingMachine::getId() {
}
*/
