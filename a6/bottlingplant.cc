#include "bottlingplant.h"
#include "printer.h"
#include "nameserver.h"
#include "truck.h"
#include "MPRNG.h"

extern MPRNG mprng;

void BottlingPlant::main() {
    // Truck created at initialization
    // Perform a production run

    m_prt.print( Printer::BottlingPlant, 'S' );           // Print starting message

    for ( ;; )
    {
        // Yield before producing to simulate production
        yield( m_timeBetweenShipments );

        // Production
        for ( unsigned int i = 0; i < m_numVendingMachines; i += 1 ){
            m_cargo[i] = mprng( 1, m_maxShippedPerFlavour);
        }

        _Accept( getShipment ) {
            m_prt.print( Printer::BottlingPlant, 'P' );       // Print shipment pickd up 
        } or _Accept( ~BottlingPlant ) {
            m_isClosing = true;
            break;
        }
    }

    m_prt.print( Printer::BottlingPlant, 'F' );

}

BottlingPlant::BottlingPlant( Printer &prt, NameServer &nameServer, unsigned int numVendingMachines,
             unsigned int maxShippedPerFlavour, unsigned int maxStockPerFlavour,
             unsigned int timeBetweenShipments )
: m_prt( prt )
, m_nameServer( nameServer )
, m_numVendingMachines( numVendingMachines )
, m_maxShippedPerFlavour( maxShippedPerFlavour )
, m_maxStockPerFlavour( maxStockPerFlavour )
, m_timeBetweenShipments( timeBetweenShipments )
, m_cargo( new unsigned int[numVendingMachines] )
, m_truck( new Truck( prt, nameServer, *this, numVendingMachines, maxStockPerFlavour ) ) 
, m_isClosing(false) {}

bool BottlingPlant::getShipment( unsigned int cargo[] ) {
    if ( !m_isClosing ) {
        cargo = m_cargo;
        return false;
    }

    return true;
}

