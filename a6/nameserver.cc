#include "nameserver.h"
#include "printer.h"
#include "vendingmachines.h"

void NameServer::main() {
}

NameServer::NameServer( Printer &prt, unsigned int numVendingMachines, unsigned int numStudents )
: m_prt( prt )
, m_numVendingMachines( numVendingMachines )
, m_numStudents( numStudents )
, m_numRegisteredMachines( 0 )
    m_registeredMachines = new VendingMachine*[numVendingMachines];
}

void NameServer::VMregister( VendingMachine *vendingmachine ) {
}

VendingMachine *NameServer::getMachine( unsigned int id ) {
}

VendingMachine **NameServer::getMachineList() {
}

