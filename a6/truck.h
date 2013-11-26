#ifndef _TRUCK_H__
#define _TRUCK_H__

// foward declarations
_Monitor Printer;
_Task NameServer;
_Task BottlingPlant;

_Task Truck {
    void main();
  public:
    Truck( Printer &prt, NameServer &nameServer, BottlingPlant &plant,
           unsigned int numVendingMachines, unsigned int maxStockPerFlavour );

  private:
    Printer &m_prt;
    NameServer &m_nameServer;
    BottlingPlant &m_plant;
    unsigned int m_numVendingMachines;
    unsigned int m_maxStockPerFlavour;
};

#endif // _TRUCK_H__

