#include "q2table.h"

// Constructor
Table::Table( const unsigned int NoOfPhil, Printer &prt ) 
: m_numPhil( NoOfPhil )
, m_prt( prt ) {
    m_forkTaken = new bool[m_numPhil];
#if defined( TABLETYPE_INT ) || defined( TABLETYPE_TASK )
    m_philConds = new uCondition[m_numPhil];
#endif
#if defined( TABLETYPE_TASK )
    m_currId = 0;
#endif

    for ( unsigned int i = 0; i < m_numPhil; i++ )
        m_forkTaken[i] = false;
} 

// Destructor
Table::~Table() {
    delete[] m_forkTaken;
#if defined( TABLETYPE_INT ) || defined( TABLETYPE_TASK )
    delete[] m_philConds;
#endif
}

// Given a philosopher id, return the left and right philosopher id
void Table::getAdjacentPhils( unsigned int id, 
                              int &leftId,   
                              int &rightId ){
    leftId = id-1;
    rightId = id+1;

    if ( leftId < 0 ) leftId = m_numPhil-1;

    if ( (unsigned int)rightId > m_numPhil-1 ) rightId = 0;
}
