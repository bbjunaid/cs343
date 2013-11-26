#include <iostream>
#include "q2printer.h"

using namespace std;

/*  Initialize variables in constructor.
    
    Print the initial two lines of the output.
*/
Printer::Printer( unsigned int numPhil )
: m_numPhil( numPhil ) 
, m_isBufferEmpty( true ) {
    m_buffer = new StateNode[m_numPhil];
    unsigned int i = 0;
    
    // initialize.isEmpty buffer
    for ( i = 0; i < m_numPhil; i++ ) {
        m_buffer[i].isEmpty = true;
    }

    // output header
    for ( i = 0; i < m_numPhil; i++ ) {
        cout << "Phil" << i;
        if ( i != m_numPhil - 1 )
            cout << "\t";
    }
    cout << endl;
    for ( i = 0; i < m_numPhil; i++ ) {
        cout << "******";
        if ( i != m_numPhil - 1 )
            cout << "\t";
    }
    cout << endl;
}

/*  Output the states in a line and then clear the buffers
*/
void Printer::flush ( ) {
    if ( m_isBufferEmpty ) 
        return;

    for ( unsigned int i = 0; i < m_numPhil; i++ ) {
        if ( !m_buffer[i].isEmpty ) {
            if ( m_buffer[i].state == Philosopher::Waiting )
                cout << m_buffer[i].state << i << "," << (i+1)%m_numPhil;
            else if ( m_buffer[i].state == Philosopher::Eating )
                cout << m_buffer[i].state << m_buffer[i].bite << "," << m_buffer[i].noodles;
            else 
                cout << m_buffer[i].state;
        }
        if ( i != m_numPhil-1 )
            cout << "\t";
        m_buffer[i].isEmpty = true;          // clear buffer
    }
    cout << endl;

    m_isBufferEmpty = true; // entire buffer is.isEmpty now
}

/*  Output the finish line 
*/
void Printer::finish ( unsigned int id ) {
    for ( unsigned int i = 0; i < m_numPhil; i++ ) {
        if ( i == id ) {
            cout << "F";
        }
        else {
            cout << "...";
        }
        if ( i != m_numPhil-1 )
            cout << "\t";
    }
    cout << endl;
}

/*  If it's finished, then flush the buffer and output Finish line.

    Otherwise, if its a change in the buffer, flush the buffer.

    At the end, overwrite the buffer with the new value.
*/
void Printer::print( unsigned int id, Philosopher::States state ) {
    if ( state == Philosopher::Finished ) {
        flush();
        finish( id );
        return;
    }
    else if ( !m_buffer[id].isEmpty ) {
        flush();
    }
    m_buffer[id].state = state;
    m_buffer[id].isEmpty = false;
    m_isBufferEmpty = false;
}

/*  Obtain left and right philosopher ids given an input philosopher */
void Printer::getAdjacentPhils( unsigned int id, 
                                int &leftId,   
                                int &rightId ){
    leftId = id-1;
    rightId = id+1;

    if ( leftId < 0 ) leftId = m_numPhil-1;

    if ( (unsigned int)rightId > m_numPhil-1 ) rightId = 0;
}

/* Same as previous print but update bite and noodle parameters
*/
void Printer::print( unsigned int id, Philosopher::States state,\
                     unsigned int bite, unsigned int noodles ) {
    print( id, state );
    m_buffer[id].bite = bite;
    m_buffer[id].noodles = noodles;
}
