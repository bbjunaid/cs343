#ifndef _NAMESERVER_H__
#define _NAMESERVER_H__

#include <vector>

// forward declarations
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
    Printer &m_prt;                         // printer
    unsigned int m_numVendingMachines;      // number of vending machines
    unsigned int m_numStudents;             // number of students 
    VendingMachine** m_registeredMachines;  // registered machines in order of their
                                            // calls to VMregister. Pointers to the actual
                                            // machines are stored in this array

    unsigned int* m_studentTurns;           // contains the machines students will access on
                                            // their next call to getMachine

    VendingMachine* m_tempMachine;          // client in VMregister copies data in this. Also used
                                            // for printing purposes main. This
                                            // allows the work to be done in the main of
                                            // this administrator task, so the client can
                                            // do other things

    unsigned int m_requestingStudent;       // student calling getMachine copies his id in this
                                            // so the nameserver can update the students turn
                                            // for future getMachine calls
};

#endif // _NAMESERVER_H__

