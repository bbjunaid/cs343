#include "nameserver.h"
#include "printer.h"

void NameServer::main() {
}

NameServer::NameServer( Printer &prt, unsigned int numVendingMachines, unsigned int numStudents )
: m_prt( prt )
, m_numVendingMachines( numVendingMachines )
, m_numStudents( numStudents ) {}

/*void NameServer::VMregister( VendingMachine *vendingmachine ) {
}

VendingMachine *NameServer::getMachine( unsigned int id ) {
}

VendingMachine **NameServer::getMachineList() {
}*/

