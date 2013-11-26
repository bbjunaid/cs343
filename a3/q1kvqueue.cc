#include <iostream>
#include <uSemaphore.h>
#include "q1kvqueue.h"

using namespace std;

KVQueue::KVQueue( int size )
: m_numThrown(0)
, m_isClosed(false)
, m_isEmptyAndClosed(false)
, m_emptySem( size )
, m_fullSem( 0 )
, m_binSemQueue( 1 ) 
, m_binSemGeneral( 1 ) { 
}

void KVQueue::pushBack( const Mapper::KeyValue& item ) {
    osacquire(std::cout) << "About to push key: " << item.key << endl;
    osacquire(std::cout) << "empty semaphore count: " << m_emptySem.counter() << endl;
    m_emptySem.P(); // wait until empty spot appears
    
    // claim critical section
    m_binSemQueue.P();
    m_queue.push( item );
    osacquire(std::cout) << "Pushing key: " << item.key << endl;
    m_binSemQueue.V();
    
    m_fullSem.V(); // signal that item is produced 
}

Mapper::KeyValue KVQueue::popFront() {
    m_fullSem.P(); // wait for value to appear 

    // claim critical section
    m_binSemQueue.P();
    Mapper::KeyValue key_val = m_queue.front();
    m_queue.pop();
    if ( m_isClosed && m_queue.size() == 0 ) m_isEmptyAndClosed = true;
    m_binSemQueue.V();

    m_emptySem.V(); // signal that empty spot appeared 
    return key_val;
}

int KVQueue::peekFront( Mapper::KeyValue* val ) throw(EmptyAndClosed) {
    if ( m_isEmptyAndClosed ) {
        // claim critical section
        m_binSemGeneral.P();
        m_numThrown++;
        m_binSemGeneral.V();

        throw EmptyAndClosed(m_numThrown);
    }
    if ( m_queue.size() == 0 || val == NULL ) return -1;
    *val = m_queue.front(); 
    return 0;
}

void KVQueue::close() {
    m_isClosed = true;
}
