#include "nameserver.h"
#include "printer.h"
#include "vendingmachine.h"

/*  The NameServer is an administrator tasks. It does not begin until all the vending machines
    have registered with it. 

    Once the machines have registered, it assigns all the students their inital machines in
    a round robin fashion.

    Thereafter, the NameServer continuously serves the Truck and the Student. The truck
    gets priority over the student so that restocking can happen before a student attempts to buy.

    When a student calls getMachine, the administrator task updates the vending machine it will 
    assign to the student on his next getMachine call in a round robin fashion, cycling through
    all vending machines.
*/
void NameServer::main() {
    m_prt.print( Printer::NameServer, 'S' );    // start message

    // wait until all machines are registered   
    for ( unsigned int i = 0; i < m_numVendingMachines; i++ ) {
        _Accept( VMregister ) {
            m_prt.print( Printer::NameServer, 'R', m_tempMachine->getId() ); // register msg
            m_registeredMachines[i] = m_tempMachine;
        }
    }

    // assign initial vending machines students will access in round robin fashion
    for ( unsigned int i = 0; i < m_numStudents; i++ ) {
        m_studentTurns[i] = i % m_numVendingMachines;
    }

    // Serve the Truck and Students
    for ( ;; ) {
        // if destructor is called, finish the task
        _Accept ( ~NameServer ) {
            break;
        } 
        // Truck calls getMachineList so it can restock the vending machines
        or _Accept ( getMachineList ) {         // Truck gets priority over student
        }
        // Student calls getMachine, and once the student has obtained their machine, 
        // they are assigned the next machine they will access
        or _Accept ( getMachine ) {
            // print the vending machine id the student was given
            m_prt.print( Printer::NameServer, 'N', m_requestingStudent, m_tempMachine->getId() );
            // assign the next vending machine the student will access
            m_studentTurns[m_requestingStudent] = \ 
                ( m_studentTurns[m_requestingStudent] + 1 ) % m_numVendingMachines;
        } 
    } 
    m_prt.print( Printer::NameServer, 'F' );    // finish message
}

// Constructor
// Allocate an array to hold vending machine pointers to hold the registered vending machines
// Allocate an array to store which vending machine each student will access next
NameServer::NameServer( Printer &prt, unsigned int numVendingMachines, unsigned int numStudents )
: m_prt( prt )
, m_numVendingMachines( numVendingMachines )
, m_numStudents( numStudents )
{
    m_registeredMachines = new VendingMachine*[numVendingMachines];
    m_studentTurns = new unsigned int[numStudents];
}

// Destructor
// Delete allocated memory
NameServer::~NameServer() {
    delete[] m_registeredMachines;
    delete[] m_studentTurns;
}

// Vending machine registers itself. The pointer to the machine is copied over into the
// NameServer so it can access its id etc later
void NameServer::VMregister( VendingMachine *vendingMachine ) {
    m_tempMachine = vendingMachine;
}

// A student acquires a vending machine they will purchase soda from. This machine
// is also copied locally to allow the administrator NameServer to do relevant printing
VendingMachine *NameServer::getMachine( unsigned int id ) {
    m_requestingStudent = id;
    m_tempMachine = m_registeredMachines[m_studentTurns[id]];
    return m_tempMachine;
}

// A Truck calls this in order to restock vending machines with soda
VendingMachine **NameServer::getMachineList() {
    return m_registeredMachines;
}

