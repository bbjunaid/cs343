#include "q2elf.h"
#include "MPRNG.h"

extern MPRNG mprng; // declared in q2driver.cc

void Elf::main() {
    bool consultSuccess;
    unsigned int numConsultations = 0;              // number of consultations done by elves
                                                    // use to break when done
    yield(mprng(10));                               // do not start simultaenously

    m_prt.print(m_id, Printer::Starting);           // start message
    
    while ( numConsultations != m_numConsultations ) {
        yield(mprng.operator()( 3 ));               // messages are not consecutive

        m_prt.print( m_id, Printer::Working );      // working message

        yield(mprng(5));                            // simulate time working

        m_prt.print( m_id, Printer::NeedHelp );     // help message
        
        // start consulting
        consultSuccess = m_wrk.consult( m_id );     // wait for consultation
        if ( !consultSuccess ) {                    // if failed, message and stop
            m_prt.print( m_id, Printer::ConsultingFailed );
            break;
        } 
        else {
            m_prt.print( m_id, Printer::Consulting ); // succeeded message 
            numConsultations++;
        }
        
        yield(mprng(3));                            // simulate consultation time

        m_wrk.doneConsulting( m_id );               // done consulting
        m_prt.print( m_id, Printer::DoneConsulting ); // done message
    }
    m_wrk.termination( m_id );                      // terminate and finished message
    m_prt.print( m_id, Printer::Finished );
}
