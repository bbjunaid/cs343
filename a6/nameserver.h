#ifndef _NAMESERVER_H__
#define _NAMESERVER_H__

// forward declare
_Monitor Printer;
_Task VendingMachine;

_Task NameServer {
    void main();
  public:
    NameServer( Printer &prt, unsigned int numVendingMachines, unsigned int numStudents );
    void VMregister( VendingMachine *vendingmachine );
    VendingMachine *getMachine( unsigned int id );
    VendingMachine **getMachineList();
  
  private:
    Printer &m_prt;
    unsigned int m_numVendingMachines;
    unsigned int m_numStudents;
};

#endif // _NAMESERVER_H__

