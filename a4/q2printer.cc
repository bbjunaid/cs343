#include <iostream>
#include "q2printer.h"
#include "q2reindeer.h"

using namespace std;

/*  Initialize variables in constructor.
    
    Print the initial two lines of the output.
*/
Printer::Printer( const unsigned int MAX_NUM_ELVES )
: m_numElves( MAX_NUM_ELVES ) 
, m_numStates( 1 + m_numElves + Reindeer::MAX_NUM_REINDEER )
, m_isEmpty( true ) {
    m_states = new char[m_numStates];
    m_stateNumbers = new int[m_numStates];
    unsigned int i = 0;
    
    for ( i = 0; i < m_numStates; i++ ) {
        m_states[i] = ' ';
        m_stateNumbers[i] = 0;
    }

    cout << "Sa";
    for ( i = 1; i <= m_numElves; i++ ) {
        cout << "\tE" << i;
    }
    for ( ; i < m_numStates; i++ ) {
        cout << "\tR" << i;
    }
    cout << endl;
    for ( i = 0; i < m_numStates; i++ ) {
        if ( i == m_numStates - 1 ) {
            cout << "--";
            break;
        }
        cout << "--\t";
    }
    cout << endl;
}

/*  Output the states in a line and then clear the buffers
*/
void Printer::flush ( ) {
    if ( m_isEmpty ) 
        return;

    for ( unsigned int i = 0; i < m_numStates; i++ ) {
        if ( m_states[i] == Blocked || m_states[i] == Unblocked ) {
            cout << m_states[i];
            if ( m_stateNumbers[i] != 0 )
                cout << " " << m_stateNumbers[i];
        }
        else if ( m_states[i] != ' ' ) {
            cout << m_states[i];
        }
        if ( i != m_numStates-1 )
            cout << "\t";

        m_states[i] = ' ';          // clear buffer
        m_stateNumbers[i] = 0;
    }
    cout << endl;
    m_isEmpty = true;
}

/*  Output the finish line 
*/
void Printer::finish ( unsigned int id ) {
    for ( unsigned int i = 0; i < m_numStates; i++ ) {
        if ( i == id ) {
            cout << "F";
        }
        else {
            cout << "...";
        }
        if ( i != m_numStates-1 )
            cout << "\t";
    }
    cout << endl;
}

/*  If it's finished, then flush the buffer and output Finish line.

    Otherwise, if its a change in the buffer, flush the buffer.

    At the end, overwrite the buffer with the new value.
*/
void Printer::print( unsigned int id, States state ) {
    if ( state == Finished ) {
        flush();
        finish( id );
        return;
    }
    else if ( m_states[id] != ' ' ) {
        flush();
    }
    m_states[id] = state;
    m_isEmpty = false;
}

/* Same as previous print but update the numbers buffer
*/
void Printer::print( unsigned int id, States state, unsigned int numBlocked ) {
    print( id, state );
    m_stateNumbers[id] = numBlocked;
}
