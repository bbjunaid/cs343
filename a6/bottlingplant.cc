#include "bottlingplant.h"
#include "printer.h"
#include "nameserver.h"

void BottlingPlant::main() {
}

BottlingPlant::BottlingPlant( Printer &prt, NameServer &nameServer, unsigned int numVendingMachines,
             unsigned int maxShippedPerFlavour, unsigned int maxStockPerFlavour,
             unsigned int timeBetweenShipments )
: m_ptr( prt )
, m_nameServer( nameServer )
, m_numVendingMachines( numVendingMachines )
, m_maxShipperPerFlavour( maxShippedPerFlavour )
, m_maxStockPerFlavour( maxStockPerFlavour )
, m_timeBetweenShipments( timeBetweenShipments ) {}

/*bool BottlingPlant::getShipment( unsigned int cargo[] ) {
}*/

