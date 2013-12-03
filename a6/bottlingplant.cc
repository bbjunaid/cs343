#include "bottlingplant.h"
#include "printer.h"
#include "nameserver.h"
#include "truck.h"
#include "MPRNG.h"

extern MPRNG mprng;     // global random number generator declared in driver.cc

/*  The bottling plant creates a truck which exists for the lifetime of the bottling plant until
    it is closing. When it starts, the plant performs productions runs where for each flavour,
    a random number of bottles between 0 and maxShipperPerFlavour are generated.

    Then the plant waits for the truck to call get shipment. This copies over the production
    cargo from the plant to the truck allowing it to do deliviers.
*/
void BottlingPlant::main() {
    // Truck created at initialization
    Truck truck ( m_prt, m_nameServer, *this, m_numVendingMachines, m_maxStockPerFlavour );

    m_prt.print( Printer::BottlingPlant, 'S' );           // Print starting message

    for ( ;; )
    {
        // If destructor is called, plant is closing
        _Accept( ~BottlingPlant ) {
            m_isClosing = true;  // flag variable which allows truck to detect closing plant
            _Accept( getShipment ) {   // unblock the truck before closing 
            }
            break;
        } _Else {
            // Yield before producing to simulate production
            yield( m_timeBetweenShipments );

            // Production run
            unsigned int bottles = 0;
            for ( unsigned int i = 0; i < VendingMachine::NUM_FLAVOURS; i += 1 ){
                m_cargo[i] = mprng( 0, m_maxShippedPerFlavour);
                bottles += m_cargo[i];
            }
            m_prt.print( Printer::BottlingPlant, 'G', bottles ); // print total bottles produced
        }
        // Let the truck pick up shipment
        _Accept( getShipment ) {
            m_prt.print( Printer::BottlingPlant, 'P' );       // Print shipment pickd up 
        } 
    }
    m_prt.print( Printer::BottlingPlant, 'F' );     // finishing message
}

// Constructor
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

// The truck calls this function. If the plant is not closing, we copy over the cargo to
// the trucks array element by element. Otherwise, return false to allow the truck to
// finish itself
bool BottlingPlant::getShipment( unsigned int cargo[] ) {
    if ( !m_isClosing ) {
        for ( unsigned int i = 0; i < VendingMachine::NUM_FLAVOURS; i++ )
            cargo[i] = m_cargo[i];
        return false;
    }
    return true;
}
