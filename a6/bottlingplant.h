#ifndef _BOTTLINGPLANT_H__
#define _BOTTLINGPLANT_H__

#include "vendingmachine.h"

// forward declarations
_Monitor Printer;
_Task NameServer;
_Task Truck;

_Task BottlingPlant {
    void main();
  public:
    BottlingPlant( Printer &prt, NameServer &nameServer, unsigned int numVendingMachines,
                 unsigned int maxShippedPerFlavour, unsigned int maxStockPerFlavour,
                 unsigned int timeBetweenShipments );
    ~BottlingPlant();
    bool getShipment( unsigned int cargo[] );

  private:
    Printer &m_prt;
    NameServer &m_nameServer;
    unsigned int m_numVendingMachines;
    unsigned int m_maxShippedPerFlavour;
    unsigned int m_maxStockPerFlavour;
    unsigned int m_timeBetweenShipments;
    unsigned int m_cargo[VendingMachine::NUM_FLAVOURS];
    Truck *m_truck; 
    bool m_isClosing;
};

#endif // _BOTTLINGPLANT_H__

