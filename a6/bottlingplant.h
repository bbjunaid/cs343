#ifndef _BOTTLINGPLANT_H__
#define _BOTTLINGPLANT_H__

// forward declarations
_Monitor Printer;
_Task NameServer;

_Task BottlingPlant {
    void main();
  public:
    BottlingPlant( Printer &prt, NameServer &nameServer, unsigned int numVendingMachines,
                 unsigned int maxShippedPerFlavour, unsigned int maxStockPerFlavour,
                 unsigned int timeBetweenShipments );
    bool getShipment( unsigned int cargo[] );

  private:
    Printer &m_ptr;
    NameServer &m_nameServer;
    unsigned int m_numVendingMachines;
    unsigned int m_maxShipperPerFlavour;
    unsigned int m_maxStockPerFlavour;
    unsigned int m_timeBetweenShipments;
};

#endif // _BOTTLINGPLANT_H__

