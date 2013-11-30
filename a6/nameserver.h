#ifndef _NAMESERVER_H__
#define _NAMESERVER_H__

#include <vector>

// forward declare
_Monitor Printer;
_Task VendingMachine;

_Task NameServer {
    void main();
  public:
    NameServer( Printer &prt, unsigned int numVendingMachines, unsigned int numStudents );
    ~NameServer();
    void VMregister( VendingMachine *vendingmachine );
    VendingMachine *getMachine( unsigned int id );
    VendingMachine **getMachineList();
  
  private:
    Printer &m_prt;
    unsigned int m_numVendingMachines;
    unsigned int m_numStudents;
    VendingMachine** m_registeredMachines; // registered machines in order of their
                                           // calls to VMregister
    unsigned int* m_studentTurns; // contains the machines students will access
    VendingMachine* m_tempMachine;  // client in VMregister copies data in this. Also used
                                    // for printing purposes in administrator
    unsigned int m_requestingStudent; // student calling getMachine copies his id in this
};

#endif // _NAMESERVER_H__

