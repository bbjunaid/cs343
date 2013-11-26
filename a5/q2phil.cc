#include "q2phil.h"
#include "q2table.h"
#include "q2printer.h"
#include "MPRNG.h"

extern MPRNG mprng;         // declared externally in q2driver.cc

// Constructor
Philosopher::Philosopher( unsigned int id, unsigned int noodles, Table &table, Printer &prt )
: m_id( id )
, m_noodles( noodles )
, m_table( table )
, m_prt ( prt )
{}

void Philosopher::main() {
    unsigned int noodlesToEat;  // noodles to eat

    for ( ;; ) {
        m_prt.print( m_id, Hungry );    // hungry message

        yield( mprng(4) );              // simulate time to get hungry

        m_table.pickup( m_id );           // pickup the forks
   
        // eat noodles and display message
        noodlesToEat = mprng( 1, 5 );
        noodlesToEat = noodlesToEat > m_noodles ? m_noodles : noodlesToEat;
        m_noodles = noodlesToEat >= m_noodles ? 0 : m_noodles - noodlesToEat;
        m_prt.print( m_id, Eating, noodlesToEat, m_noodles );
        yield( mprng(4) );

        m_table.putdown( m_id );          // release the forks 
        
        if ( m_noodles == 0 ) break;    // finished eating all noodles
        
        m_prt.print( m_id, Thinking );  // thinking message
        yield( mprng(19 ) );              // simulate time to think  
    }
    m_prt.print( m_id, Finished );      // Finished message
}
