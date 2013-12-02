#include "bottlingplant.h"
#include "printer.h"
#include "nameserver.h"
#include "truck.h"
#include "MPRNG.h"

extern MPRNG mprng;

void BottlingPlant::main() {
    // Truck created at initialization
    Truck truck ( m_prt, m_nameServer, *this, m_numVendingMachines, m_maxStockPerFlavour );

    m_prt.print( Printer::BottlingPlant, 'S' );           // Print starting message

    for ( ;; )
    {
        _Accept( ~BottlingPlant ) {
            m_isClosing = true;
            _Accept( getShipment ) {
            }
            break;
        } _Else {
            // Perform a production run
            // Yield before producing to simulate production
            yield( m_timeBetweenShipments );

            // Production
            unsigned int bottles = 0;
            for ( unsigned int i = 0; i < VendingMachine::NUM_FLAVOURS; i += 1 ){
                m_cargo[i] = mprng( 0, m_maxShippedPerFlavour);
                bottles += m_cargo[i];
            }
            m_prt.print( Printer::BottlingPlant, 'G', bottles );
        }

        _Accept( getShipment ) {
            m_prt.print( Printer::BottlingPlant, 'P' );       // Print shipment pickd up 
        } 
    }
    m_prt.print( Printer::BottlingPlant, 'F' );
}

BottlingPlant::~BottlingPlant() {
    osacquire( cout ) << "In destructor of bottling plant" << endl;
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
, m_isClosing(false) {}

bool BottlingPlant::getShipment( unsigned int cargo[] ) {
    if ( !m_isClosing ) {
        for ( unsigned int i = 0; i < VendingMachine::NUM_FLAVOURS; i++ )
            cargo[i] = m_cargo[i];
        return false;
    }
    return true;
}

