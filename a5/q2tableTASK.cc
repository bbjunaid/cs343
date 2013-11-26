#include "q2table.h"
#include "q2phil.h"     // to access State
#include <iostream>

using namespace std;

void Table::pickup( unsigned int id ) {
    // if left or right fork are taken, block the philosopher
    if ( m_forkTaken[id] || m_forkTaken[(id+1) % m_numPhil] ) {
        m_prt.print( id, Philosopher::Waiting ); 
        m_philConds[id].wait();
    }
    m_forkTaken[m_currId] = m_forkTaken[(m_currId+1) % m_numPhil] = true;
    m_currId = id; 
}

void Table::putdown( unsigned int id ) {
    m_currId = id;
}

void Table::main() {
    for ( ;; ) {
        _Accept( ~Table ) {
            break;
        } 
        or _Accept( pickup ) {  // having an accept pickup is necessary to restart the loop
        } 
        or _Accept( putdown ) {
            int leftPhil, rightPhil;
            bool leftSignalled = false;
            getAdjacentPhils( m_currId, leftPhil, rightPhil );  

            // release left and right fork
            m_forkTaken[m_currId] = m_forkTaken[(m_currId+1) % m_numPhil] = false;
            
            // if left philosopher is blocked, signal it if it's left fork is available
            if ( !m_philConds[leftPhil].empty() && !m_forkTaken[leftPhil] ) {
                m_philConds[leftPhil].signalBlock();
                leftSignalled = true;
            }

            // if right fork is blocked, signal it if its right fork is available
            if ( !m_philConds[rightPhil].empty() && !m_forkTaken[(rightPhil+1) % m_numPhil] ) {
                // if the left philosopher has been signalled, then check whether the right
                // philosophers right is the left philosopher. If so, then don't signal the
                // right philosopher since the left will claim the fork it needs
                if ( !leftSignalled || (leftSignalled && ((rightPhil+1) % (int)m_numPhil != leftPhil)) ) 
                    m_philConds[rightPhil].signalBlock();
            }

        }
    } 
}
