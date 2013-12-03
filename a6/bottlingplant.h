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
    bool getShipment( unsigned int cargo[] );

  private:
    Printer &m_prt;                         // printer
    NameServer &m_nameServer;               // the nameserver which is passed to truck
    unsigned int m_numVendingMachines;      // number of vending machines
    unsigned int m_maxShippedPerFlavour;    // the max amount of production per flavour
    unsigned int m_maxStockPerFlavour;      // maximum number of stock in vending machine
                                            // per flavour. Used in the Truck.
    unsigned int m_timeBetweenShipments;    // Used to simulate productions
    unsigned int m_cargo[VendingMachine::NUM_FLAVOURS]; // cargo which is filled with production
    bool m_isClosing;                       // is the plant closing
};

#endif // _BOTTLINGPLANT_H__

