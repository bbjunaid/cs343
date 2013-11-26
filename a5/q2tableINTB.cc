#include "q2table.h"
#include "q2phil.h"     // to access State


// NOTE: This is incomplete. Please see test doc for explanation.
void Table::wait() {
    waiting.wait(); // wait until signalled
    while ( rand() % 5 == 0 ) { // multiple bargers allowed
        _Accept( pickup, putdown ) { // accept barging callers
        } _Else { // do not wait if no callers
        } // Accept
    } // if
}

void Table::signal() {
    waiting.signal();
}

void Table::pickup( unsigned int id ) {
    if ( m_isSignalled ) {
        m_prt.print( id, Philosopher::Barging );
        m_bargers.push_back( id ); 
        wait();
        if ( !waiting.empty() ) 
            waiting.signal();
        else
            m_isSignalled = false;
    }
    
    // if left or right fork are taken, block the philosopher
    if ( m_forkTaken[id] || m_forkTaken[(id+1) % m_numPhil] ) {
        m_prt.print( id, Philosopher::Waiting ); 
        wait();
        if ( !waiting.empty() ) 
            waiting.signal();
        else
            m_isSignalled = false;
    }

    // claim the left and right fork
    m_forkTaken[id] = m_forkTaken[(id+1) % m_numPhil] = true;
}

void Table::putdown( unsigned int id ) {
    int leftPhil, rightPhil;
    getAdjacentPhils( id, leftPhil, rightPhil );  
    
    // release left and right fork
    m_forkTaken[id] = m_forkTaken[(id+1) % m_numPhil] = false;

    if( !waiting.empty() ) {
        m_isSignalled = true;
        waiting.signal();
    }
    
} // Table::wait
