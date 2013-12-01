#include "truck.h"
#include "printer.h"
#include "nameserver.h"
#include "bottlingplant.h"
#include "MPRNG.h"

extern MPRNG mprng;

void Truck::main() {
    VendingMachine** machineList;
    unsigned int* machineInventory;
    unsigned int sodasInShipment = 0;
    unsigned int bottlesNotReplenished = 0;

    m_prt.print( Printer::Truck, 'S' );

    machineList = m_nameServer.getMachineList();

    for ( ;; ) {
        yield( mprng(1,10) ); // yield to get coffee

        // Override the cargo by obtaining new shipment from plant
        if ( m_plant.getShipment( m_cargo ) )
            break;

        sodasInShipment = 0;
        for ( unsigned int i = 0; i < VendingMachine::NUM_FLAVOURS; i++ )
            sodasInShipment += m_cargo[i];  

        m_prt.print( Printer::Truck, 'P', sodasInShipment );
       
        // Restock the vending machines until either all the vending
        // machines are cycled or there is no soda left on the truck 
        for ( unsigned int i = 0; i < m_numVendingMachines && sodasInShipment != 0; i++ ) {
            bottlesNotReplenished = 0; 
            m_prt.print( Printer::Truck, 'd', i, sodasInShipment );
            machineInventory = machineList[i]->inventory();
            // go through all flavors and fill up vending machines as long
            // as they haven't read their max stock and the truck has sodas of that flavour
            // also keep track of the bottles that weren't replenished for each flavour
            for ( unsigned int j = 0; j < VendingMachine::NUM_FLAVOURS; j++ ) {
                while ( m_cargo[j] != 0 && machineInventory[j] < m_maxStockPerFlavour ) {
                    machineInventory[j] += 1;
                    m_cargo[j] -= 1;
                    sodasInShipment -= 1;
                }
                bottlesNotReplenished += ( m_maxStockPerFlavour - machineInventory[j] );
            }
            m_prt.print( Printer::Truck, 'U', i, bottlesNotReplenished );
            m_prt.print( Printer::Truck, 'D', i, sodasInShipment );
            machineList[i]->restocked();
        } 
    }

    m_prt.print( Printer::Truck, 'F' );
}

Truck::Truck( Printer &prt, NameServer &nameServer, BottlingPlant &plant,
       unsigned int numVendingMachines, unsigned int maxStockPerFlavour )
: m_prt( prt )
, m_nameServer( nameServer )
, m_plant( plant )
, m_numVendingMachines( numVendingMachines )
, m_maxStockPerFlavour( maxStockPerFlavour )
{}
