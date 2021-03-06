#include "printer.h"        // Header for Printer monitor

#include <iostream>         // cin, cout, endl
using namespace std;


// Printer constructor
// Initialize all monitor variables, print header, initialize states to empty
// Update the kind of all tasks in their respective buffer structs 
Printer::Printer( unsigned int numStudents, unsigned int numVendingMachines, unsigned int numCouriers )
: m_numStudents( numStudents )
, m_numVendingMachines( numVendingMachines )
, m_numCouriers( numCouriers )
, NUM_TASK_TOTAL( numStudents + numVendingMachines + numCouriers + 5 )
, m_finishedCnt(0) {

    m_buffer.resize( NUM_TASK_TOTAL );

    // Print header
    cout << "Parent\tWATOff\tNames\tTruck\tPlant\t";
    m_buffer[0].kind = Parent; 
    m_buffer[1].kind = WATCardOffice;
    m_buffer[2].kind = NameServer;
    m_buffer[3].kind = Truck;
    m_buffer[4].kind = BottlingPlant;

    unsigned int id;
    for ( id = 0; id < numStudents; id += 1 ) {
        cout << "Stud" << id << "\t";
        m_buffer[5 + id].kind = Student;
    }

    for ( id = 0; id < numVendingMachines; id += 1 ) {
        cout << "Mach" << id << "\t";
        m_buffer[5 + numStudents + id].kind = Vending; 
    }

    for ( id = 0; id < numCouriers; id += 1 ) {
        cout << "Cour" << id << "\t";
        m_buffer[5 + numStudents + numVendingMachines + id].kind = Courier;
    }
    cout << endl;

    for ( id = 0; id < NUM_TASK_TOTAL; id += 1 ) {
        cout << "*******" << "\t";
        m_buffer[id].state = ' ';
    }
    cout << endl;
}

// central print function for print calls with singular tasks that don't have lid
// other print functions that don't have lid call this function for actual printing
void Printer::print( Kind kind, char state ) {
    if ( state == 'F' ) {
        m_finishedCnt++;
        if ( m_isBufferEmpty == false ) {
            flush();
        }
        int id = getGlobalId( kind );
        flushFinished( id );
        clear();
        return;
    }

    if ( m_buffer[getGlobalId( kind )].state != ' ') {
        flush();
        clear();
    }

    m_buffer[getGlobalId( kind )].state = state;
    m_isBufferEmpty = false;
}

// updates value1 and call print with only kind and state for actual printing
void Printer::print( Kind kind, char state, int value1 ) {
    print( kind, state );
    m_buffer[getGlobalId( kind )].value1 = value1;
}

// updates value1 and value2 and call print with only kind and state for actual printing
void Printer::print( Kind kind, char state, int value1, int value2 ) {
    print( kind, state );
    m_buffer[getGlobalId( kind )].value1 = value1;
    m_buffer[getGlobalId( kind )].value2 = value2;
}

// central print function for print calls with tasks of same kind and different id
// other print functions with lid call this function for actual printing
void Printer::print( Kind kind, unsigned int lid, char state ) {
    if ( state == 'F' ) {
        m_finishedCnt++;
        if ( m_isBufferEmpty == false ) {
            flush();
        }
        flushFinished( getGlobalId( kind, lid ) );
        clear();
        return;
    }

    if ( m_buffer[getGlobalId( kind, lid )].state != ' ') {
        flush();
        clear();
    }

    m_buffer[getGlobalId( kind, lid )].state = state;
    m_isBufferEmpty = false;
}

// updates value1 and cal print with kind, lid, and state for actual printing
void Printer::print( Kind kind, unsigned int lid, char state, int value1 ) {
    print( kind, lid, state );
    m_buffer[getGlobalId( kind, lid )].value1 = value1;
}

// updates value1 and value2 and cal print with kind, lid, and state for actual printing
void Printer::print( Kind kind, unsigned int lid, char state, int value1, int value2 ) {
    print( kind, lid, state );
    m_buffer[getGlobalId( kind, lid )].value1 = value1;
    m_buffer[getGlobalId( kind, lid )].value2 = value2;
}

// return the id of the sinugular tasks out of all the tasks in the program
unsigned int Printer::getGlobalId( Kind kind ) {
    unsigned int id = 0;
    switch( kind ) {
        case Parent:
            id = 0;
            break;
        case WATCardOffice:
            id = 1;
            break;
        case NameServer:
            id = 2;
            break;
        case Truck:
            id = 3;
            break;
        case BottlingPlant:
            id = 4;
            break;
        default:
            break;
    }
    return id;
}

// return the id of the multiple tasks out of all the tasks in the program
unsigned int Printer::getGlobalId( Kind kind, unsigned int lid ) {
    unsigned int id = lid;
    switch( kind ) {
        case Courier:
            id += m_numVendingMachines;
            // FALL THROUGH
        case Vending:
            id += m_numStudents;
            // FALL THROUGH
        case Student:
            id += 5;
            break;
        default:
            break;
    }
    return id;
}

void Printer::flush() {
    for ( unsigned int id = 0; id < NUM_TASK_TOTAL; id += 1 ) {
        cout << m_buffer[id].state;
        /*
         * As per the assignment requirements, value1 and value2 are printed
         * depending on the kind of task and the state being flushed
         */
        switch( m_buffer[id].kind ) {
            case Parent:
                if ( m_buffer[id].state == 'D' ) {
                    cout << m_buffer[id].value1 << "," << m_buffer[id].value2;
                }
                break;
            case WATCardOffice:
                if ( m_buffer[id].state == 'C' || m_buffer[id].state == 'T' ) {
                    cout << m_buffer[id].value1 << "," << m_buffer[id].value2;
                }
                break;
            case NameServer:
                if ( m_buffer[id].state == 'R' ) {
                    cout << m_buffer[id].value1;
                } 
                else if ( m_buffer[id].state == 'N' ) {
                    cout << m_buffer[id].value1 << "," << m_buffer[id].value2;
                }
                break;
            case Truck:
                if ( m_buffer[id].state == 'd' || 
                     m_buffer[id].state == 'U' || 
                     m_buffer[id].state == 'D' ) {
                    cout << m_buffer[id].value1 << "," << m_buffer[id].value2;
                }
                else if ( m_buffer[id].state == 'P' ) {
                    cout << m_buffer[id].value1;
                } 
                break;
            case BottlingPlant:
                if ( m_buffer[id].state == 'G' ) {
                    cout << m_buffer[id].value1;
                }
                break;
            case Student:
                if ( m_buffer[id].state == 'V' || m_buffer[id].state == 'B' ) {
                    cout << m_buffer[id].value1;
                }
                else if ( m_buffer[id].state == 'S' ) {
                    cout << m_buffer[id].value1 << "," << m_buffer[id].value2;
                }
                break;
            case Vending:
                if ( m_buffer[id].state == 'S' ) {
                    cout << m_buffer[id].value1;
                }
                else if ( m_buffer[id].state == 'B' ) {
                    cout << m_buffer[id].value1 << "," << m_buffer[id].value2;
                }
                break;
            case Courier:
                if ( m_buffer[id].state == 't' || m_buffer[id].state == 'T' ) {
                    cout << m_buffer[id].value1 << "," << m_buffer[id].value2;
                }
                break;
        }
        // Print standard tab between states except the last one
        if ( id < NUM_TASK_TOTAL - 1 ) cout << "\t";
    }
    cout << endl;
}

// Flushes the special row of states when a task has finished
// Print 'F' for task that is finished and '...' for all other tasks
void Printer::flushFinished( unsigned int id ) {
    for ( unsigned int i = 0; i < NUM_TASK_TOTAL; i += 1 ) {
        if ( i != id ) cout << "...";
        else cout << 'F';

        if ( i < NUM_TASK_TOTAL -1 ) cout << "\t";
    }
    cout << endl;
    if ( m_finishedCnt == NUM_TASK_TOTAL ) {        // if all tasks have finish, indicate with a special line at the end of program
        cout << "***********************" << endl;
    }
}

// Clear the states in the buffer
void Printer::clear() {
    for ( unsigned int id = 0; id < NUM_TASK_TOTAL; id += 1 ) {
        m_buffer[id].state = ' ';
    }
    m_isBufferEmpty = true;
}

