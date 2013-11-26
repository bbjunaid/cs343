#include "parent.h"

#include "printer.h"
#include "bank.h"

void Parent::main() {
    /*for ( ;; ) {
        yield( m_parentalDelay );
        m_bank.deposit( prng( m_numStudents ), prng( 1, 3 ) );

        // Parent busy waits for the call to its destructor
        _Accept( ~Parent ) {
            break;
        } _Else {
        }

    }*/
}

Parent::Parent( Printer &prt, Bank &bank, unsigned int numStudents, unsigned int parentalDelay ) :
    m_prt(prt), m_bank(bank), m_numStudents(numStudents), m_parentalDelay(parentalDelay) {
   
}

Parent::~Parent() {
}

