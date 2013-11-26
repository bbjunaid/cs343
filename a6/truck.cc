#include "truck.h"
#include "printer.h"
#include "nameserver.h"
#include "bottlingplant.h"

void Truck::main() {
}

Truck::Truck( Printer &prt, NameServer &nameServer, BottlingPlant &plant,
       unsigned int numVendingMachines, unsigned int maxStockPerFlavour )
: m_prt( prt )
, m_nameServer( nameServer )
, m_plant( plant )
, m_numVendingMachines( numVendingMachines )
, m_maxStockPerFlavour( maxStockPerFlavour ) {}

