#include <iostream>
#include "q1reducer.h"
#include "q1mapper.h"
#include "q1kvqueue.h"

void Reducer::main() {
    Mapper::KeyValue key_val; 
    int word_count;
    try {
        while ( true ) {
            m_pSignal->P(); 
            for ( unsigned int i = 0; i < m_mappers.size(); i++ ) {
                if ( m_mappers[i]->q->peekFront( &key_val ) == 0 ) {
                    if ( hash( key_val.key ) % m_numReducers == m_id ) {
                        key_val = m_mappers[i]->q->popFront();
                        if ( m_keyvals.find( key_val.key ) != m_keyvals.end() ) {
                            word_count = m_keyvals[ key_val.key ];          
                            m_keyvals[ key_val.key ] = word_count+1;
                        }
                        else {
                            m_keyvals[ key_val.key ] = 1;
                        }
                        break; 
                    } // hash matches
                } // peekFront
            } // for
        } // while
    }
    catch ( KVQueue::EmptyAndClosed e ) {
        if ( e.num_thrown < m_numReducers )
            m_pSignal->V(); 
    }
    map<string, int>::iterator iter;
    for ( iter = m_keyvals.begin(); iter != m_keyvals.end(); iter++ ) {
        osacquire(cout) << iter->first 
                             << " : " << iter->second << endl;
    }
}

Reducer::Reducer( int id, int num_reducers, uSemaphore* signal, const vector<Mapper*>& mappers ) 
: m_id( id )
, m_numReducers( num_reducers)
, m_pSignal( signal )
, m_mappers( mappers )
{ }

Reducer::~Reducer() {

}

uSemaphore* Reducer::getSignal() {
    return m_pSignal;
}

int Reducer::getID() {
    return m_id;
}

int Reducer::getNumReducers() {
    return m_numReducers;
}
