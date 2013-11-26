#include <iostream>
#include <fstream>
#include "q1mapper.h"
#include "q1kvqueue.h"


void Mapper::main() {
    ifstream fin;   
    string word;

    osacquire(cout) << "Inside mapper for file: " << m_filename << endl;
    fin.open( m_filename.c_str() ); // even though driver ensures valid file sent, do a check for sake of completeness
    if ( !fin.fail() ) {
        while ( fin >> word ) {
            osacquire(cout) << "pushing key: " << word << endl;
            q->pushBack( Mapper::KeyValue( word, 1 ) );
            m_pSignal->V();
        }
    }
    else {
        cout << "The file " << m_filename << " cannot be opened." << endl;
    }
    fin.close();
    q->close(); // close the queue
    m_pSignal->V();
}

Mapper::Mapper( const string& filename, int queue_len, uSemaphore* signal ) {
    m_filename = filename;
    m_pSignal = signal;
    q = new KVQueue( m_queueLen );
}

Mapper::~Mapper() {
    delete q;
}

uSemaphore* Mapper::getSignal() {
    return m_pSignal;
}


const string& Mapper::getFilename() {
    return m_filename;
}
