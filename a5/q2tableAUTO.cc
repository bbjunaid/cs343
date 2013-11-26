#include "q2table.h"
#include "q2phil.h"     // to access State

void Table::pickup( unsigned int id ) {
    WAITUNTIL( !m_forkTaken[id] && !m_forkTaken[(id+1) % m_numPhil],\
               m_prt.print( id, Philosopher::Waiting ), ); // empty after
               
    // claim the left and right fork
    m_forkTaken[id] = m_forkTaken[(id+1) % m_numPhil] = true;
    RETURN();
}

void Table::putdown( unsigned int id ) {
    int leftPhil, rightPhil;
    getAdjacentPhils( id, leftPhil, rightPhil );  
    
    // release left and right fork
    m_forkTaken[id] = m_forkTaken[(id+1) % m_numPhil] = false;
    
   RETURN();
}
