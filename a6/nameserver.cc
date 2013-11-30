#include "nameserver.h"
#include "printer.h"
#include "vendingmachine.h"

void NameServer::main() {
    m_prt.print( Printer::NameServer, 'S' );

    // wait until all machines are registered   
    for ( unsigned int i = 0; i < m_numVendingMachines; i++ ) {
        m_studentTurns[i] = i;
        _Accept( VMregister ) {
            m_prt.print( Printer::NameServer, 'R', m_tempMachine->getId() );
            m_registeredMachines[i] = m_tempMachine;
        }
    }

    for ( ;; ) {
        _Accept ( ~NameServer ) {
            m_prt.print( Printer::NameServer, 'F' );
            break;
        } 
        // Print student requesting new machine and assign the next machine index the student
        // will access
        or _Accept ( getMachine ) {
            m_prt.print( Printer::NameServer, 'N', m_requestingStudent, m_tempMachine->getId() );
            m_studentTurns[m_requestingStudent] = \
                ( m_studentTurns[m_requestingStudent] + 1 ) % m_numVendingMachines;
        }
    } 
}

NameServer::NameServer( Printer &prt, unsigned int numVendingMachines, unsigned int numStudents )
: m_prt( prt )
, m_numVendingMachines( numVendingMachines )
, m_numStudents( numStudents )
{
    m_registeredMachines = new VendingMachine*[numVendingMachines];
    m_studentTurns = new unsigned int[numStudents];
}

NameServer::~NameServer() {
    delete[] m_registeredMachines;
    delete[] m_studentTurns;
}

void NameServer::VMregister( VendingMachine *vendingMachine ) {
    m_tempMachine = vendingMachine;
}

VendingMachine *NameServer::getMachine( unsigned int id ) {
    m_requestingStudent = id;
    m_tempMachine = m_registeredMachines[m_studentTurns[id]];
    return m_tempMachine;
}

VendingMachine **NameServer::getMachineList() {
    return m_registeredMachines;
}

