#include "parent.h"

#include "printer.h"
#include "bank.h"
#include "MPRNG.h"

extern MPRNG mprng;

void Parent::main() {
    for ( ;; ) {
        // Parent busy waits for the call to its destructor
        _Accept( ~Parent ) {
            break;
        } _Else {
            yield( m_parentalDelay );
            m_bank.deposit( mprng( m_numStudents - 1 ), mprng( 1, 3 ) );
        }

    }
}

Parent::Parent( Printer &prt, Bank &bank, unsigned int numStudents, unsigned int parentalDelay ) 
: m_prt(prt)
, m_bank(bank)
, m_numStudents(numStudents)
, m_parentalDelay(parentalDelay) {}

Parent::~Parent() {}

