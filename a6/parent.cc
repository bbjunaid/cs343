#include "parent.h"

#include "printer.h"
#include "bank.h"
#include "MPRNG.h"

extern MPRNG mprng;

void Parent::main() {
    m_prt.print( Printer::Parent, 'S' );
    for ( ;; ) {
        // Parent busy waits for the call to its destructor
        _Accept( ~Parent ) {
            break;
        } _Else {
            yield( m_parentalDelay );
            unsigned int sid, amount;
            sid = mprng( m_numStudents - 1 );
            amount = mprng( 1, 3 );
            m_bank.deposit( sid, amount );
            m_prt.print( Printer::Parent, 'D', sid, amount );
        }
    }
    m_prt.print( Printer::Parent, 'F' );
    osacquire( cout ) << "Parent finished" << endl;
}

Parent::Parent( Printer &prt, Bank &bank, unsigned int numStudents, unsigned int parentalDelay ) 
: m_prt(prt)
, m_bank(bank)
, m_numStudents(numStudents)
, m_parentalDelay(parentalDelay) {}
