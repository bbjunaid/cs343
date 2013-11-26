#ifndef _PRINTER_H__
#define _PRINTER_H__

_Monitor Printer {
  public:
    enum Kind { Parent, WATCardOffice, NameServer, Truck, BottlingPlant, Student, Vending, Courier };
    Printer( unsigned int numStudents, unsigned int numVendingMachines, unsigned int numCouriers );
    void print( Kind kind, char state );
    void print( Kind kind, char state, int value1 );
    void print( Kind kind, char state, int value1, int value2 );
    void print( Kind kind, unsigned int lid, char state );
    void print( Kind kind, unsigned int lid, char state, int value1 );
    void print( Kind kind, unsigned int lid, char state, int value1, int value2 );

  private:
    unsigned int m_numStudents;
    unsigned int m_numVendingMachines;
    unsigned int m_numCouriers;
};

#endif // _PRINTER_H__

